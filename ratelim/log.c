#include "log.h"

void debugLog(char *text)
{
	pthread_mutex_lock(&(thread_shared->mutex));

	FILE *fh = 0;
	char message[255] = { 0 };
	char timebuf[30] = { 0 };
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timebuf, 26, "%Y/%m/%d %H:%M:%S", timeinfo);
	sprintf(message, "{\"timestamp\":\"%s\",%s}\n", timebuf, text);

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
