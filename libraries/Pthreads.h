#include <pthread.h>

void Pthread_mutex_lock(pthread_mutex_t *mtx);
void Pthread_mutex_unlock(pthread_mutex_t * mtx);
void Pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mtx);
void Pthread_cond_signal(pthread_cond_t * cond);
void Pthread_create(pthread_t *thread_id, const pthread_attr_t *attr, void* (*start_fcn) (void *), void* arg);
void Pthread_join(pthread_t threadId,void** status_ptr);
void cleanup_function(void *arg);