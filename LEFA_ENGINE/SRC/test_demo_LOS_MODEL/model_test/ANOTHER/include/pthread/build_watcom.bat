@echo off

set GLOB= /6r -fp6 -ol
set CC=wcc386


@wcc386 %GLOB% "cleanup.c" -Fo="EXIT/"
echo.
@wcc386 %GLOB%	"create.c" -FO="EXIT/"
echo.
@wcc386 %GLOB%	"dll.c" -FO="EXIT/"
echo.
@wcc386 %GLOB%	"errno.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"global.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_destroy.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getaffinity_np.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getdetachstate.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getinheritsched.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getname_np.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getschedparam.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getschedpolicy.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getscope.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getstackaddr.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_getstacksize.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_init.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setaffinity_np.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setdetachstate.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setinheritsched.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setname_np.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setschedparam.c" -FO="EXIT/"
echo.
@wcc386  %GLOB%	"pthread_attr_setschedpolicy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB% "pthread_attr_setscope.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_attr_setstackaddr.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_attr_setstacksize.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrierattr_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrierattr_getpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrierattr_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrierattr_setpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrier_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrier_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_barrier_wait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_cancel.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_condattr_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_condattr_getpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_condattr_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_condattr_setpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_cond_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_cond_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_cond_signal.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_cond_wait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_delay_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_detach.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_equal.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_exit.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getconcurrency.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getname_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getschedparam.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getspecific.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getunique_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_getw32threadhandle_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_join.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_key_create.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_key_delete.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_kill.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_getkind_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_getpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_getrobust.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_gettype.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_setkind_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_setpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_setrobust.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutexattr_settype.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_consistent.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_lock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_timedlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_trylock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_mutex_unlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_num_processors_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_once.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlockattr_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlockattr_getpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlockattr_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlockattr_setpshared.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_rdlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_timedrdlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_timedwrlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_tryrdlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_trywrlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_unlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_rwlock_wrlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_self.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setaffinity.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setcancelstate.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setcanceltype.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setconcurrency.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setname_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setschedparam.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_setspecific.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_spin_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_spin_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_spin_lock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_spin_trylock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_spin_unlock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_testcancel.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_timechange_handler_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_timedjoin_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_tryjoin_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"pthread_win32_attach_detach_np.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_calloc.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_callUserDestroyRoutines.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_cond_check_need_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_getprocessors.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_is_attr.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_MCS_lock.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_mutex_check_need_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_new.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_processInitialize.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_processTerminate.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_relmillisecs.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_reuse.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_rwlock_cancelwrwait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_rwlock_check_need_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_semwait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_spinlock_check_need_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_threadDestroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_threadStart.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_throw.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_timespec.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_tkAssocCreate.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"ptw32_tkAssocDestroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_getscheduler.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_get_priority_max.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_get_priority_min.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_setaffinity.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_setscheduler.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sched_yield.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_close.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_destroy.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_getvalue.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_init.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_open.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_post.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_post_multiple.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_timedwait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_trywait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_unlink.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"sem_wait.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"signal.c" -FO="EXIT/"
echo.
@%CC%  %GLOB%	"w32_CancelableWait.c" -FO="EXIT/"
echo.

pause