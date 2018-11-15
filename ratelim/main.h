#ifndef RATELIM_MAIN_H
#define RATELIM_MAIN_H

#define C_MOD_MUTEX "mutex.ratelim.kres.module\0"
#define C_MOD_LOGFILE "/var/log/whalebone/ratelim.log\0"

#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "thread_shared.h"
#include "log.h"

int create();
int destroy();
int usage();
int userInput();

#endif