/**
  * @file vector_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 五 10/16 09:26:10 2015
  */

#include <stdio.h>
#include <stdlib.h>

#include "../vector.h"
#include "../../algo/str.h"


static void vector_print(vector *v) {
	int len = vector_length(v);
	for (int i = 0; i < len; ++i) {
		void *p = NULL;
		v_get_ptr(v, i, &p);
		printf("%s\n", (char*)p);
	}
}


int main(int argc, char *argv[]) {
	vector a;
	
	vector_init(&a, 0);

	// test for ptr operations
	int n = 5;
	for (int i = 0; i < n; ++i) {
		char *str = malloc(2);
		v_push_ptr(&a, itoa(i, str, 10));
	}
	vector_print(&a);
	
	for (int i = 0; i < n / 2; ++i) {
		void *p = NULL;
		v_get_ptr(&a, i, &p);
		void *q  =NULL;
		v_get_ptr(&a, n - i - 1, &q);
		v_set_ptr(&a, i, q);
		v_set_ptr(&a, n - i - 1, p);
	}
	vector_print(&a);

	while (vector_length(&a) > 0) {
		v_pop_ptr(&a, NULL);
	}

	return 0;
}

