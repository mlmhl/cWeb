/**
  * @file log_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 17:44:02 2015
  */

#include <stdio.h>
#include <stdlib.h>

#include "log.h"


int main(int argc, char *argv[]) {
	int retCode = 0;
	logger *log = (logger*)malloc(sizeof(logger));
	
	retCode = logger_init(log, STDFLAG, "log.01");
	if (retCode > 0) {
		printf("init logger error: %d\n", retCode);
	}

	retCode = logger_printf(log, "My name is %s, I'm %d years old.", "malin", 25);
	if (retCode > 0) {
		printf("print error: %d\n", retCode);
	}
	retCode = logger_printf(log, "My girl friend is %s, I love her.", "guoshuai");
	if (retCode > 0) {
		printf("print error: %d\n", retCode);
	}

	retCode = logger_flush(log);
	if (retCode > 0) {
		printf("flush error: %d\n", retCode);
	}

	logger_destroy(log);	
	free(log);

	return 0;
}

