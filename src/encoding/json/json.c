/**
  * @file json.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 五 10/23 15:01:59 2015
  */


#include <string.h>

#include "json.h"
#include "../../algo/str.h"
#include "../../util/buffer.h"



static int marshal_recursive(json_node *n, buffer *buf);


static int marshal_null(json_node *n, buffer *buf) {
	return buffer_append(buf, "null", 4);
}

static int marshal_int(json_node *n, buffer *buf) {
	int retCode;
	char str[20];
	itoa(n->inumber, str, 10);
	if ((retCode = buffer_append(buf, str, strlen(str))) > 0) {
		return retCode;
	}
	return 0;
}

static int marshal_float(json_node *n, buffer *buf) {
	int retCode;
	char str[50];
	ftoa(n->fnumber, str, 10, SCIENCE);
	if ((retCode = buffer_append(buf, str, strlen(str))) > 0) {
		return retCode;
	}
	return 0;
}

static int marshal_bool(json_node *n, buffer *buf) {
	int retCode;
	char *str = n->value == 0 ? "false" : "true";
	if ((retCode = buffer_append(buf, str, strlen(str))) > 0) {
		return retCode;
	}
	return 0;
}

static int marshal_str(json_node *n, buffer *buf) {
	return buffer_append(buf, (void*)(n->str), strlen(n->str));
}

static int marshal_object(json_node *n, buffer *buf) {
	int retCode;
	if (n->object == NULL) {
		if ((retCode = buffer_append(buf, "null", 4)) > 0) {
			return retCode;
		}
	}
	else {
		if ((retCode = buffer_append(buf, "{", 1)) > 0) {
			return retCode;
		}
		if ((retCode = marshal_recursive(n->object, buf)) > 0) {
			return retCode;
		}
		if ((retCode = buffer_append(buf, "}", 1)) > 0) {
			return retCode;
		}
	}
	return 0;
}

static int marshal_array(json_node *n, buffer *buf) {
	int retCode;
	if (n->arr == NULL) {
		if ((retCode = buffer_append(buf, "null", 4)) > 0) {
			return retCode;
		}
	}
	else {
		if ((retCode = buffer_append(buf, "[", 1)) > 0) {
			return retCode;
		}
		size_t len = vector_length(n->arr);
		for (int i = 0; i < len; ++i) {
			void *p = NULL;
			if ((retCode = v_get_ptr(n->arr, i, &p)) > 0) {
				return retCode;
			}
			if ((retCode = marshal_recursive((json_node*)p, buf)) > 0) {
				return retCode;
			}
			if (i < len - 1 && (retCode = buffer_append(buf, ",", 1)) > 0) {
				return retCode;
			}
		}
		if ((retCode = buffer_append(buf, "]", 1)) > 0) {
			return retCode;
		}
	}
	return 0;
}


static int marshal_tag(json_node *n, buffer *buf) {
	int retCode;
	if ((retCode = buffer_append(buf, (void*)(n->key), strlen(n->key))) > 0) {
		return retCode;
	}
	if ((retCode = buffer_append(buf, ":", 1)) > 0) {
		return retCode;
	}

	return 0;
}

typedef int(*marshal_handle)(json_node *n, buffer *buf);
static marshal_handle handles[] = {marshal_int, marshal_float, marshal_str, marshal_bool, marshal_array, marshal_object, marshal_null};


static int marshal_recursive(json_node *n, buffer *buf) {
	int retCode;
	while (1) {
		// append tag
		if ((retCode = marshal_tag(n, buf)) > 0) {
			return retCode;
		}
		
		// append data
		if ((retCode = handles[n->type](n, buf)) > 0) {
			return retCode;
		}			

		n = n->next;
		if (n == NULL) {
			break;
		}
		else {
			if ((retCode = buffer_append(buf, ",", 1)) > 0) {
				return retCode;
			}
		}
	}
	return 0;
}


int json_marshal(json_node *n, const char **str) {
	buffer buf;
	int retCode;
	retCode = buffer_init(&buf, 0);
	if (retCode > 0) {
		return retCode;
	}

	if (n->type == Object) {
		retCode = marshal_object(n, &buf);
	}
	else if (n->type == Array) {
		retCode = marshal_array(n, &buf);
	}
	else {
		retCode = handles[n->type](n, &buf);
	}

	if (retCode > 0) {
		return retCode;
	}

	*str = buf_to_str(&buf);
	return 0;
}


static int unmarshal_recursive(const char *str, json_node **node_ptr) {
	int retCode;
	json_node *n = malloc(sizeof(json_node));

	if (*str == '{') {
		n->type = Object;
		if ((retCode = unmarshal_object(++str, &(n->object))) > 0) {
			free(n);
			return retCode;
		}
	}
	else if ()
}


int json_unmarshal(const char *str, json_node **n) {
	
}
