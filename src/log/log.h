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


static const int DATE = 0x01;
static const int TIME = 0x02;
static const int LFILE = 0x04;
static const int SFILE = 0x08;
static const int STDFLAG = DATE | TIME | SFILE;

static const int ERR_LOGGER_ALLOC = 0x01;
static const int ERR_LOGGER_CREATE_FILE = 0x02;
static const int ERR_LOGGER_DESTROY_FILE = 0x04;


static const int THR = 1024;


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

int logger_flush();

// return the flag on success, or -1 on error
int logger_flag(logger *log);
// return the old flag
int logger_set_flag(logger *log, int flag);

int logger_set_output(logger *log, const char *out);


#endif // LOG_H
