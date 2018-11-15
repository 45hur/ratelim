#include "main.h"

int create()
{
	int err = 0;
	int fd = shm_open(C_MOD_MUTEX, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd == -1)
		return fd;

	if ((err = ftruncate(fd, sizeof(struct shared))) != 0)
		return err;

	thread_shared = (struct shared*)mmap(0, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (thread_shared == NULL)
		return -1;

	thread_shared->sharedResource = 0;

	// Make sure it can be shared across processes
	pthread_mutexattr_t shared;
	if ((err = pthread_mutexattr_init(&shared)) != 0)
		return err;

	if ((err = pthread_mutexattr_setpshared(&shared, PTHREAD_PROCESS_SHARED)) != 0)
		return err;

	if ((err = pthread_mutex_init(&(thread_shared->mutex), &shared)) != 0)
		return err;

	return err;
}

int destroy()
{
	int err = 0;
	if ((err = munmap(thread_shared, sizeof(struct shared*))) == 0)
		return err;

	if ((err = shm_unlink(C_MOD_MUTEX)) == 0)
		return err;

	return err;
}

int usage()
{
	printf("Available commands:");
	printf("\n");
	printf("exit\n");
	return 0;
}

int userInput()
{
	char command[80] = { 0 };
	printf("\nType command:");
	scanf("%79s", command);

	if (strcmp("exit", command) == 0)
		return 0;
	else
		usage();

	return 1;
}

#ifdef NOKRES
int main()
{
	int err = 0;
	if ((err = create()) != 0)
	{
		debugLog("error in create()");
		return err;
	}

	usage();
	while (userInput());

	if ((err = destroy()) != 0)
	{
		debugLog("error in destroy()");
		return err;
	}

    return err;
}
#endif