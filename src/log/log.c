/**
  * @file log.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 16:59:19 2015
  */


#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"


int logger_init(logger *log, int flag, const char *out) {
	log->flag = flag;
	
	log->out = open(out, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (log->out == -1) {
		return ERR_CREATE_LOG_FILE;
	}
	
	// initail buffer
	int retCode = buffer_init(&(log->buf), 0);
	if (retCode > 0) {
		close(log->out);
		return retCode;
	}

	// create and initial the attributes for lock
	pthread_mutexattr_t mtx_attr;
	retCode = pthread_mutexattr_init(&mtx_attr);
	if (retCode > 0) {
		return retCode;
	}
	retCode = pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK);
	if (retCode > 0) {
		return retCode;
	}

	// initial lock
	retCode = pthread_mutex_init(&(log->lock), &mtx_attr);
	pthread_mutexattr_destroy(&mtx_attr);
	if (retCode > 0) {
		close(log->out);
		return retCode;
	}

	return 0;
}


int logger_destroy(logger *log) {
	int out_ret_code = close(log->out);
	if (out_ret_code == -1) {
		return ERR_DESTROY_OUT_FILE;
	}

	int buffer_ret_code = buffer_destroy(&(log->buf));
	if (buffer_ret_code > 0) {
		return buffer_ret_code;
	}

	int lock_ret_code = pthread_mutex_destroy(&(log->lock));
	if (lock_ret_code > 0) {
		return ERR_DESTROY_MUTEX;
	}
	
	return 0;
}


static int formatTime(logger *log) {
	const char *time_str = ctime(&(time(NULL)));
}

static int output(logger *log, const char *tag, const char *content) {
	return 0;
}
