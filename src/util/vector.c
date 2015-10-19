/**
  * @file vector.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 10:11:36 2015
  */

#include <stdlib.h>
#include <string.h>

#include "vector.h"


#define max(x, y) (x) > (y) ? (x) : (y)


static int adjust_capacity(vector *v, size_t capacity) {
	void *new_data = malloc(capacity);
	if (new_data == NULL) {
		return ERR_VECTOR_ALLOC;
	}

	memcpy(new_data, v->data, v->size);
	free(v->data);
	v->data = new_data;
	v->capacity = capacity;

	return 0;
}


int vector_init(vector *v, size_t capacity) {
	v->num = 0;
	v->size = 0;
	v->capacity = capacity;
	
	if (capacity > 0) {
		v->data = malloc(capacity);
		if (v->data == NULL) {
			return ERR_VECTOR_ALLOC;
		}
	}
	else {
		v->data = NULL;
	}

	return 0;
}


int vector_destroy(vector *v) {
	free(v->data);
	return 0;
}


int vector_length(vector *v) {
	return v->num;
}


int v_push_ptr(vector *v, void *p) {
	if (v->size == v->capacity) {
		int retCode = adjust_capacity(v, max(v->capacity << 1, sizeof(p)));
		if (retCode > 0) {
			return retCode;
		}
	}

	((void**)(v->data))[v->num++] = p;
	v->size += sizeof(p);

	return 0;
}


int v_pop_ptr(vector *v, void **p) {
	if (v->num == 0) {
		return ERR_VECTOR_UNDERFLOW;
	}
	
	if (p != NULL) {
		int retCode = v_get_ptr(v, v->num - 1, p);
		if (retCode > 0) {
			return retCode;
		}
	}
	--v->num;
	v->size -= sizeof(p);

	if (v->size < v->capacity >> 2) {
		int retCode = adjust_capacity(v, max(v->capacity >> 1, sizeof(p)));
		if (retCode > 0) {
			return retCode;
		}
	}

	return 0;
}


int v_get_ptr(vector *v, size_t index, void **p) {
	if (index >= v->num) {
		return ERR_VECTOR_OVERFLOW;
	}
	
	*p = ((void**)(v->data))[index];
	
	return 0;
}


int v_set_ptr(vector *v, size_t index, void *p) {
	if (index >= v->num) {
		return ERR_VECTOR_OVERFLOW;
	}

	((void**)(v->data))[index] = p;

	return 0;
}
