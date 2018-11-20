#pragma once
 
#include <fcntl.h> 
#include <pthread.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
 
#include "log.h"
#include "thread_shared.h" 
#include "counter.h"
 
int ftruncate(int fd, off_t length); 
 
static int create() 
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
 
	pthread_mutexattr_t shared; 
	if ((err = pthread_mutexattr_init(&shared)) != 0) 
		return err; 
 
	if ((err = pthread_mutexattr_setpshared(&shared, PTHREAD_PROCESS_SHARED)) != 0) 
		return err; 
 
	if ((err = pthread_mutex_init(&(thread_shared->mutex), &shared)) != 0) 
		return err; 
 
	createVector(100000);

	debugLog("\"%s\":\"%s\"", "message", "created");

	return err; 
} 
 
static int destroy()
{ 
	int err = 0; 
	if ((err = munmap(thread_shared, sizeof(struct shared*))) == 0) 
		return err; 
 
	if ((err = shm_unlink(C_MOD_MUTEX)) == 0) 
		return err; 

	destroyVector();
 
	debugLog("\"%s\":\"%s\"", "message", "destroyed");

	return err; 
} 
 
static int usage()
{ 
	fprintf(stdout, "Available commands: "); 
	fprintf(stdout, "\n");
	fprintf(stdout, "exit\n");
	fprintf(stdout, "increment\n");
	fprintf(stdout, "insert\n");
	fprintf(stdout, "print\n");
	return 0; 
} 

static int increment()
{
	int err = 0;
	char command[80] = { 0 };
	fprintf(stdout, "\nEnter ip address: ");
	scanf("%79s", command);

	if ((err = vectorIncrement(command)) == 0)
	{
		fprintf(stdout, "\nAddress %s incremented.", command);
	}
	else
	{
		fprintf(stdout, "\nAddress %s not incremented.", command);
	}

	return err;
}

static int insert()
{
	int err = 0;
	char command[80] = { 0 };
	fprintf(stdout, "\nEnter ip address: ");
	scanf("%79s", command);

	if ((err = vectorAdd(command)) == 0)
	{
		fprintf(stdout, "\nAddress %s added.", command);
	}
	else
	{
		fprintf(stdout, "\nAddress %s not added.", command);
	}

	return err;
}

static int print()
{
	int err = 0;

	vectorPrint();

	return err;
}
 
static int userInput()
{ 
	char command[80] = { 0 }; 
	fprintf(stdout, "\nType command:");
	scanf("%79s", command); 
 
	if (strcmp("exit", command) == 0)
		return 0;
	else if (strcmp("increment", command) == 0)
		increment();
	else if (strcmp("insert", command) == 0)
		insert();
	else if (strcmp("print", command) == 0)
		print();
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
		debugLog("\"%s\":\"%s\"", "message", "error in create");
		return err; 
	} 
 
	usage(); 
	while (userInput()); 
 
	if ((err = destroy()) != 0) 
	{ 
		debugLog("\"%s\":\"%s\"", "message", "error in destroy");
		return err; 
	} 
 
    return err; 
} 

#endif