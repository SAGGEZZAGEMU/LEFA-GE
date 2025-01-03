/* Lips of Suna
 * Copyright© 2007-2010 Lips of Suna development team.
 *
 * Lips of Suna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Lips of Suna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Lips of Suna. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \addtogroup LISys System
 * @{
 * \addtogroup LISysSemaphore Semaphore
 * @{
 */

//#include<semaphore.h>
#include"../include/pthread/semaphore.h"
#include"system_memory.h"
#include"system_semaphore.h"

struct _LISysSemaphore
{
	sem_t semaphore;
};

LISysSemaphore* lisys_semaphore_new()
{
	LISysSemaphore* self;

	self = lisys_calloc(1, sizeof (LISysSemaphore));
	if (self == NULL)
		return NULL;
	sem_init(&self->semaphore, 0, 0);

	return self;
}

void lisys_semaphore_free(LISysSemaphore* self)
{
	sem_destroy (&self->semaphore);
	lisys_free (self);
}

void lisys_semaphore_signal(LISysSemaphore* self)
{
	sem_post (&self->semaphore);
}

void lisys_semaphore_wait(LISysSemaphore* self)
{
	sem_wait (&self->semaphore);
}

/** @} */
/** @} */
