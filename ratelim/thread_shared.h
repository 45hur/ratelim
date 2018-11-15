#ifndef RATELIM_THREAD_SHARED_H
#define RATELIM_THREAD_SHARED_H

struct shared
{
	pthread_mutex_t mutex;
	int sharedResource;
};

struct shared *thread_shared;

#endif