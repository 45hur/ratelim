#ifndef RATELIM_LOG_H
#define RATELIM_LOG_H

#include <pthread.h>
#include <stdio.h>

#include "thread_shared.h"

#define C_MOD_MUTEX "mutex.ratelim.kres.module\0"
#define C_MOD_LOGFILE "/var/log/whalebone/ratelim.log\0"

void debugLog(char *text);

#endif