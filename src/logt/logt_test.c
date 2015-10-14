/**
  * @file logt_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 19:50:49 2015
  */


#include "logt.h"


int main(int argc, char *argv[]) {
	logger log;
	logger_init(&log, 1, "hi");
	logger_destroy(&log);

	return 0;
}
