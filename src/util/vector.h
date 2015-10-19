/**
  * @file vector.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 09:50:55 2015
  */


#ifndef VECTOR_H
#define VECTOR_H


static const int ERR_VECTOR_ALLOC = 0x01;
static const int ERR_VECTOR_OVERFLOW = 0x02;
static const int ERR_VECTOR_UNDERFLOW = 0x04;


typedef struct {
	void *data;			// memory space
	size_t num;			// elements numbers
	size_t size;		// size(byte)
	size_t capacity;	// capacity(byte)
}vector;


int vector_init(vector *v, size_t capacity);
int vector_destroy(vector *v);

int vector_length(vector *v);

int v_push_ptr(vector *v, void *p);
int v_pop_ptr(vector *v, void **p);
int v_get_ptr(vector *v, size_t index, void **p);
int v_set_ptr(vector *v, size_t index, void *p);


#endif // VECTOR_H
