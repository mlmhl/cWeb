/**
  * @file log_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 17:44:02 2015
  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "log.h"


typedef struct {
	logger *log;
	int cnt;
}arg;

void *multi_thread_log(void *p) {
	arg *a = (arg*)p;
	int retCode = logger_printf(a->log, "This is the %d invoke", a->cnt);
	if (retCode > 0) {
		return (void*)(long long)retCode;
	}
	
	return (void*)(long long)(logger_flush(a->log));
}


int main(int argc, char *argv[]) {
	int retCode = 0;
	logger *log = (logger*)malloc(sizeof(logger));
	
	
	// normal test
	retCode = logger_init(log, STDFLAG, "log.01");
	if (retCode > 0) {
		printf("init logger error: %d\n", retCode);
	}

	retCode = logger_printf(log, "My name is %s, I'm %d years old.", "malin", 25);
	if (retCode > 0) {
		printf("print error: %d\n", retCode);
	}
	retCode = logger_printf(log, "My girl friend is %s, I love her.", "menghanlin");
	if (retCode > 0) {
		printf("print error: %d\n", retCode);
	}

	retCode = logger_flush(log);
	if (retCode > 0) {
		printf("flush error: %d\n", retCode);
	}


	// multithread test
	arg a;
	a.log = log;

	for (int i = 0; i < 5; ++i) {
		pthread_t pid;
		
		a.cnt = i;
		retCode = pthread_create(&pid, NULL, multi_thread_log, &a);
		if (retCode > 0) {
			printf("create pthread %d error\n", i);
			continue;
		}
		pthread_join(pid, NULL);
	}

	logger_destroy(log);	
	free(log);

	return 0;
}

