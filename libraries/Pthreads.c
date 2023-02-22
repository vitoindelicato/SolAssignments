#include <pthread.h>
#include <stdio.h>
#include <errno.h>

void Pthread_mutex_lock(pthread_mutex_t *mtx) {
    int err;
    if ( ( err=pthread_mutex_lock(mtx)) != 0 ) {errno=err; perror("lock"); pthread_exit( (void*)errno);}
}

void Pthread_mutex_unlock(pthread_mutex_t * mtx) {
    int err;
    if ( ( err = pthread_mutex_unlock(mtx)) != 0 ) {errno = err; perror("unlock"); pthread_exit((void*) errno);}
}

void Pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mtx) {
    int err;
    if ( ( err = pthread_cond_wait(cond, mtx)) != 0 ) {errno = err; perror("wait"); pthread_exit((void*) errno);}
}

void Pthread_cond_signal(pthread_cond_t * cond) {
    int err;
    if ( ( err = pthread_cond_signal(cond)) != 0 ) {errno = err; perror("signal"); pthread_exit((void*) errno);}
}

void Pthread_create(pthread_t *thread_id, const pthread_attr_t *attr, void* (*start_fcn) (void *), void* arg) {
    int err;
    if ( ( err = pthread_create(thread_id, attr, start_fcn, arg)) != 0 ) {errno = err; perror("create"); pthread_exit((void*) errno);}
}

void Pthread_join(pthread_t threadId,void** status_ptr){
    int err;
    if ( ( err = pthread_join(threadId, status_ptr)) != 0 ) {errno = err; perror("join"); pthread_exit((void*) errno);}
}

void cleanup_function(void *arg) {
    pthread_mutex_unlock((pthread_mutex_t *)arg);
}
