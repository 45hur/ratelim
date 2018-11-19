#ifndef RATELIM_LOG_H
#define RATELIM_LOG_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread_shared.h"

#define C_MOD_MUTEX "mutex.ratelim.kres.module\0"
#define C_MOD_LOGFILE "/var/log/whalebone/ratelim.log\0"

static void debugLog(const char *format, ...)
{
#ifdef DEBUG
	va_list dbgargptr;
	va_start(dbgargptr, format);
	vfprintf(stdout, format, dbgargptr);
	va_end(dbgargptr);
#endif

	pthread_mutex_lock(&(thread_shared->mutex));

	char text[256] = { 0 };
	va_list argptr;
	va_start(argptr, format);
	vsprintf(text, format, argptr);
	va_end(argptr);

	FILE *fh = 0;
	char message[286] = { 0 };
	char timebuf[30] = { 0 };
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timebuf, 26, "%Y/%m/%d %H:%M:%S", timeinfo);
	sprintf(message, "{\"timestamp\":\"%s\",%s}\n", timebuf, text);

	fprint(stdout, message);

	if (fh == 0)
	{
		fh = fopen(C_MOD_LOGFILE, "at");
		if (!fh)
			fh = fopen(C_MOD_LOGFILE, "wt");
		if (!fh)
		{
			pthread_mutex_unlock(&(thread_shared->mutex));
			return;
		}
	}

	fputs(message, fh);
	fflush(fh);
	fclose(fh);

	pthread_mutex_unlock(&(thread_shared->mutex));
}

#endif