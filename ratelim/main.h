#ifndef RATELIM_MAIN_H
#define RATELIM_MAIN_H

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "thread_shared.h"
#include "log.h"

int create();
int destroy();
int usage();
int userInput();

int ftruncate(int fd, off_t length);

#endif