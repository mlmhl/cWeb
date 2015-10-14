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
	logger *log = (logger*)malloc(sizeof(logger));
	logger_init(log, STDFLAG, "log.01");
	logger_destroy(log);

	return 0;
}

