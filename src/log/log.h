/**
  * @file log.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 10:00:37 2015
  */

#ifndef LOG_H
#define LOG_H


#include <pthread.h>

#include "../util/buffer.h"


#define DATE 0x01
#define TIME 0x02
#define LFILE 0x04
#define SFILE 0x08
#define STDFLAG 0x10

#define ERR_LOGGER_ALLOC 0x01
#define ERR_CREATE_LOG_FILE 0x02
#define ERR_DESTROY_OUT_FILE 0x04
#define ERR_DESTROY_MUTEX 0x08


typedef struct {
	int flag;				// properties
	int out;				// destination for output
	buffer buf;				// for accumulating text to write
	pthread_mutex_t lock;	// ensure atomic writes
}logger;


// return 0 on success, or a positive error number on error

int logger_init(logger *log, int flag, const char *out);

int logger_destroy(logger *log);

int logger_printf(logger *log, const char *format, ...);

int logger_warnf(logger *log, const char *format, ...);

int logger_errorf(logger *log, const char *format, ...);

int logger_debugf(logger *log, const char *format, ...);

int logger_flush();

// return the flag on success, or -1 on error
int logger_flag();
int logger_set_flag(int flag);

int logger_set_output(const char *out);


#endif // LOG_H
