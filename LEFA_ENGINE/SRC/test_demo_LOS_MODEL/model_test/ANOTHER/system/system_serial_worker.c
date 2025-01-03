#include"system_serial_worker.h"
#include"system_mutex.h"
#include"system_semaphore.h"
#include"system_thread.h"

typedef struct _LISysSerialWorkerTask LISysSerialWorkerTask;

struct _LISysSerialWorkerTask
{
	LISysSerialWorkerTask* next;
	void* data;
};

typedef struct _LISysSerialWorkerQueue LISysSerialWorkerQueue;
struct _LISysSerialWorkerQueue
{
	LISysMutex* mutex;
	LISysSerialWorkerTask* pending;
};

struct _LISysSerialWorker
{
	int quit;
	LISysThread* thread;
	LISysSemaphore* tasks_semaphore;
	LISysSerialWorkerTaskHandleFunc task_handle;
	LISysSerialWorkerTaskFreeFunc task_free;
	LISysSerialWorkerResultFreeFunc result_free;
	LISysSerialWorkerQueue tasks;
	LISysSerialWorkerQueue results;
};

static void private_worker_thread(LISysThread* thread,
								  void*        data);

/*****************************************************************************/

LISysSerialWorker* 
lisys_serial_worker_new(LISysSerialWorkerTaskHandleFunc task_handle,
						LISysSerialWorkerTaskFreeFunc   task_free,
						LISysSerialWorkerResultFreeFunc result_free)
{
	LISysSerialWorker* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LISysSerialWorker));
	if (self == NULL)
		return NULL;
	self->task_handle = task_handle;
	self->task_free = task_free;
	self->result_free = result_free;

	/* Create the queue mutexes. */
	self->tasks.mutex = lisys_mutex_new ();
	if (self->tasks.mutex == NULL)
	{
		lisys_serial_worker_free (self);
		return NULL;
	}
	self->results.mutex = lisys_mutex_new ();
	if (self->results.mutex == NULL)
	{
		lisys_serial_worker_free (self);
		return NULL;
	}

	/* Create the semaphore. */
	self->tasks_semaphore = lisys_semaphore_new ();
	if (self->tasks_semaphore == NULL)
	{
		lisys_serial_worker_free (self);
		return NULL;
	}

	/* Start the worker thread. */
	self->thread = lisys_thread_new (private_worker_thread, self);
	if (self->thread == NULL)
	{
		lisys_serial_worker_free (self);
		return NULL;
	}

	return self;
}

void lisys_serial_worker_free(LISysSerialWorker* self)
{
	LISysSerialWorkerTask* ptr;
	LISysSerialWorkerTask* ptr_next;

	/* Terminate the thread. */
	self->quit = 1;
	if (self->thread != NULL)
	{
		lisys_semaphore_signal (self->tasks_semaphore);
		lisys_thread_join (self->thread);
		lisys_thread_free (self->thread);
	}

	/* Free remaining tasks. */
	for (ptr = self->tasks.pending ; ptr != NULL ; ptr = ptr_next)
	{
		ptr_next = ptr->next;
		self->task_free (ptr->data);
		lisys_free (ptr);
	}

	/* Free remaining results. */
	for (ptr = self->results.pending ; ptr != NULL ; ptr = ptr_next)
	{
		ptr_next = ptr->next;
		self->result_free (ptr->data);
		lisys_free (ptr);
	}

	/* Free the mutexes. */
	if (self->tasks.mutex != NULL)
		lisys_mutex_free (self->tasks.mutex);
	if (self->results.mutex != NULL)
		lisys_mutex_free (self->results.mutex);

	/* Free the semaphore. */
	if (self->tasks_semaphore != NULL)
		lisys_semaphore_free (self->tasks_semaphore);
	lisys_free (self);
}

void* lisys_serial_worker_pop_result(LISysSerialWorker* self)
{
	void* data;
	LISysSerialWorkerTask* result;

	/* Pop a result from the queue. */
	lisys_mutex_lock (self->results.mutex);
	result = self->results.pending;
	if (result != NULL)
		self->results.pending = result->next;
	lisys_mutex_unlock (self->results.mutex);

	/* Extract the data and free the result. */
	if (result == NULL)
		return NULL;
	data = result->data;
	lisys_free (result);

	return data;
}

int lisys_serial_worker_push_task(LISysSerialWorker* self,
								  void*              data)
{
	LISysSerialWorkerTask* ptr;
	LISysSerialWorkerTask* task;

	/* Allocate a new task. */
	task = lisys_calloc (1, sizeof (LISysSerialWorkerTask));
	if (task == NULL)
		return 0;
	task->data = data;

	/* Add the task to the pending queue. */
	lisys_mutex_lock (self->tasks.mutex);
	if (self->tasks.pending != NULL)
	{
		for (ptr = self->tasks.pending ; ptr->next != NULL ; ptr = ptr->next) {}
		ptr->next = task;
	}
	else
		self->tasks.pending = task;
	lisys_mutex_unlock (self->tasks.mutex);

	/* Notify the worker. */
	lisys_semaphore_signal (self->tasks_semaphore);

	return 1;
}

/*****************************************************************************/

static void private_worker_thread(LISysThread* thread,
								  void*        data)
{
	void* result;
	LISysSerialWorkerTask* ptr;
	LISysSerialWorkerTask* task;
	LISysSerialWorker* self = data;

	lisys_semaphore_wait (self->tasks_semaphore);
	while (!self->quit)
	{
		/* Get the next task. */
		lisys_mutex_lock (self->tasks.mutex);
		task = self->tasks.pending;
		if (task == NULL)
			continue;
		self->tasks.pending = task->next;
		task->next = NULL;
		lisys_mutex_unlock (self->tasks.mutex);

		/* Process the task. */
		result = self->task_handle (task->data);
		self->task_free (task->data);

		/* Publish the result. */
		if (result != NULL)
		{
			task->data = result;
			lisys_mutex_lock (self->results.mutex);
			if (self->results.pending != NULL)
			{
				for (ptr = self->results.pending ; ptr->next != NULL ; ptr = ptr->next) {}
				ptr->next = task;
			}
			else
				self->results.pending = task;
			lisys_mutex_unlock (self->results.mutex);
		}
		else
			lisys_free (task);

		/* Start the next round. */
		lisys_semaphore_wait (self->tasks_semaphore);
	}
}
