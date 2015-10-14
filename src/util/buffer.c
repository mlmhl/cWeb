/**
  * @file buffer.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 日 10/11 08:19:50 2015
  */


#include <stdlib.h>
#include <string.h>

#include "buffer.h"


static int expand(buffer *buf) {
	buf->capacity *= 2;
	void *old_pool = buf->pool;
	buf->pool = malloc(buf->capacity);
	if (buf->pool == NULL)
		return ERR_BUFFER_ALLOC;
	memcpy(buf->pool, old_pool, buf->size);
	free(old_pool);
	return 0;
}


static int shrink(buffer *buf) {
	void *old_pool = buf->pool;
	buf->capacity >>= 1;
	buf->pool = malloc(buf->capacity);
	if (buf->pool == NULL) {
		return ERR_BUFFER_ALLOC;
	}
	memcpy(buf->pool, old_pool, buf->size);
	return 0;
}


int buffer_init(buffer *buf, size_t capacity) {
	buf->size = 0;
	buf->capacity = capacity;
	buf->pool = (char*)malloc(capacity);
	if (buf->pool == NULL) {
		return ERR_BUFFER_ALLOC;
	}
	return 0;
}


int buffer_append(buffer *buf, void *data, size_t size) {
	if (size > buf->capacity - size) {
		int ret_code = expand(buf);
		if (ret_code != 0)
			return ret_code;
	}

	memcpy(buf->pool + buf->size, data, size);
	buf->size += size;

	return 0;
}


int buffer_reduce(buffer *buf, size_t size) {
	if (size > buf->size)
		buf->size = 0;
	else
		buf->size -= size;

	if (buf->size < buf->capacity >> 2) {
		int ret_code = shrink(buf);
		if (ret_code != 0)
			return ret_code;
	}
	return 0;
}


// current version, buffer_destroy always return 0.
int buffer_destroy(buffer *buf) {
	free(buf->pool);
	return 0;
}


const char *buf_to_str(buffer *buf) {
	if (buf->size == buf->capacity) {
		expand(buf);
	}
	*((char*)(buf->pool + buf->size)) = '\0';

	return (const char*)(buf->pool);
}
