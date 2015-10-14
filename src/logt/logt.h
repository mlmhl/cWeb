/**
  * @file logt.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 19:47:42 2015
  */


#ifndef LOGT_H
#define LOGT_H


typedef struct {
	int a;
}logger;


int logger_init(logger *log, int flag, const char *out);

int logger_destroy(logger *log);


#endif // LOGT_H
