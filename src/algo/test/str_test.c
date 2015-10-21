/**
  * @file str_test.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 日 10/18 10:23:13 2015
  */


#include <stdio.h>
#include <limits.h>

#include "../str.h"
#include "../../util/vector.h"


void test_itoa() {
	char buf[20];
	int nums[] = {3, 0, 31, INT_MIN, INT_MAX};

	for (int i = 0; i < sizeof(nums) / sizeof(int); ++i) {
		printf("%s\n", itoa(nums[i], buf, 10));
		printf("%s\n", itoa(nums[i], buf, 16));
	}
}

void test_split() {
	char str[] = "hello world! I'm ml";
	vector v;
	vector_init(&v, 0);
	str_split(str, " ", &v);
	for (int i = 0; i < vector_length(&v); ++i) {
		void *res = NULL;
		v_get_ptr(&v, i, &res);
		printf("%s\n", (const char*)res);
	}
	vector_destroy(&v);
}


int main(int argc, char *argv[]) {
	test_itoa();
	//test_split();

	return 0;
}
