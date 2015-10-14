/**
  * @file buffer.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/ 8 21:58:57 2015
  */


#ifndef BUFFER_H
#define BUFFER_H


#include <stddef.h>


#define ERR_BUFFER_ALLOC 0x01;


typedef struct {
	size_t size;
	size_t capacity;
	void *pool;
}buffer;


// all functions below return 0 when successed
// otherwise return a positive integer specify the error

int buffer_init(buffer *buf, size_t capacity);

int buffer_append(buffer *buf, void *data, size_t size);

int buffer_reduce(buffer *buf, size_t size);

int buffer_destroy(buffer *buf);

const char *buf_to_str(buffer *buf);


#endif // BUFFER_H
