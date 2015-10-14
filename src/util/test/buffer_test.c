/**
  * @file buffer_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 10/13 09:35:39 2015
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../buffer.h"


int main(int argc, char *argv[]) {
	buffer *buf = (buffer*)malloc(sizeof(buffer));
	buffer_init(buf, 0);

	// test append
	printf("before append, size: %lu\n", buf->size);
	const char *str = "hello world!";
	buffer_append(buf, (void*)str, strlen(str));
	printf("after append, size: %lu, data: %s\n", buf->size, buf_to_str(buf));

	// test reduce
	printf("before reduce, size: %lu\n", buf->size);
	buffer_reduce(buf, 7);
	printf("after reduce, size: %lu, data: %s\n", buf->size, buf_to_str(buf));

	buffer_destroy(buf);

	return 0;
}
