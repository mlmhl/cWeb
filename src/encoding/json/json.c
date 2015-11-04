/**
  * @file json.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 五 10/23 15:01:59 2015
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "../../algo/str.h"
#include "../../util/buffer.h"


json_node *new_json_node() {
	json_node *n = malloc(sizeof(json_node));
	if (n != NULL) {
		memset(n, 0, sizeof(json_node));
	}
	return n;
}

void destroy_json_node(json_node *n) {
	if (n == NULL) {
		return;
	}

	if (n->key != NULL) {
		free((void*)n->key);
	}

	if (n->type == String) {
		if (n->str != NULL) {
			free((void*)(n->str));
		}
	}
	else if (n->type == Object) {
		json_node *child = n->object, *next;
		while (child != NULL) {
			next = child->next;
			destroy_json_node(child);
			child = next;
		}
	}
	else if (n->type == Array) {
		if (n->arr != NULL) {
			int len = vector_length(n->arr);
			for (int i = 0; i < len; ++i) {
				void *p;
				if (v_get_ptr(n->arr, i, &p) > 0) {
					break;
				}
				destroy_json_node((json_node*)p);
			}
			vector_destroy(n->arr);
			free((void*)(n->arr));
		}
	}
	free(n);
}


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
	sprintf(str, "%g", n->fnumber);
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
	int retCode;
	if ((retCode = buffer_append(buf, "\"", 1)) > 0) {
		return retCode;
	}
	if ((retCode = buffer_append(buf, (void*)(n->str), strlen(n->str))) > 0) {
		return retCode;
	}
	return buffer_append(buf, "\"", 1);
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
static marshal_handle handles[] = {NULL, marshal_int, marshal_float, marshal_str,
	marshal_bool, marshal_array, marshal_object, marshal_null};

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


static int unmarshal_recursive(const char **str_ptr, json_node *n);

static int unmarshal_tag(const char **str_ptr, json_node *n) {
	const char *str = *str_ptr, *cur = str;
	while (*cur != ':' && *cur != '\0') {
		++cur;
	}
	if (*cur == '\0') {
		return ERR_JSON_INVALID_TAG;
	}

	int len = cur - str;
	n->key = malloc(len + 1);
	sstrncpy((char*)(n->key), str, len);
	*str_ptr = cur + 1;
	return 0;
}

static int unmarshal_string(const char **str_ptr, json_node *n) {
	const char *tmp = *str_ptr + 1; // skip '"'
	while (*tmp != '\"' && *tmp != '\0') {
		++tmp;
	}
	if (*tmp == '\0') {
		return ERR_JSON_INVALID_STRING;
	}

	n->type = String;
	int len = tmp - *str_ptr - 1;
	n->str = malloc(len + 1);
	sstrncpy((char*)(n->str), *str_ptr + 1, len);
	*str_ptr += len + 2;	// update start of data
	return 0;
}

static int unmarshal_number(const char **str_ptr, json_node *n) {
	char *end_ptr;
	n->inumber = strtol(*str_ptr, &end_ptr, 10);
	if (*end_ptr == '\0' || *end_ptr == ',') {
		n->type = Int;
	}
	else {
		n->fnumber = strtod(*str_ptr, &end_ptr);
		if (*end_ptr != '\0' && *end_ptr != ',') {
			printf("%s\n", end_ptr);
			return ERR_JSON_INVALID_NUMBER;
		}
		n->type = Float;
	}
	*str_ptr = end_ptr;		// update start of data
	return 0;
}

static int unmarshal_object(const char **str_ptr, json_node *n) {
	const char *str = *str_ptr + 1; // skip '{'
	if (*str == '}') {
		n->type = Object;
		n->object = NULL;
		return 0;
	}

	int retCode;
	json_node *cur = new_json_node();
	n->object = cur;
	
	while (1) {
		if ((retCode = unmarshal_tag(&str, cur)) > 0) {
			return retCode;
		}
		if ((retCode = unmarshal_recursive(&str, cur)) > 0) {
			return retCode;
		}
		
		if (*str == '}') {
			break;
		}
		else if (*str == ',') {
			cur->next = new_json_node();
			cur = cur->next;
			++str;
		}
		else {
			return ERR_JSON_INVALID_OBJECT;
		}
	}

	n->type = Object;
	*str_ptr = str + 1;
	return 0;
}

static int unmarshal_array(const char **str_ptr, json_node *n) {
	int retCode;
	const char *str = *str_ptr + 1; // skip '['
	if (*str == ']') {	
		n->type = Array;
		n->arr = NULL;
		return 0;
	}
	n->arr = malloc(sizeof(vector));
	vector_init(n->arr, 0);
	
	while (1) {
		json_node *elem = new_json_node();
		if ((retCode = unmarshal_tag(&str, elem)) > 0) {
			return retCode;
		}

		if ((retCode = unmarshal_recursive(&str, elem)) > 0) {
			return retCode;
		}
		if ((retCode = v_push_ptr(n->arr, (void*)elem)) > 0) {
			return retCode;
		}
		
		if (*str == ']') {
			break;
		}
		else if (*str != ',') {
			return ERR_JSON_INVALID_ARRAY;
		}
		++str;
	}

	n->type = Array;
	*str_ptr = str + 1;
	return 0;
}

static int unmarshal_recursive(const char **str_ptr, json_node *n) {
	int retCode;
	const char *str = *str_ptr;
	
	if (*str == '{') {
		if ((retCode = unmarshal_object(&str, n)) > 0) {
			return retCode;
		}
	}
	else if (*str == '[') {
		if ((retCode = unmarshal_array(&str, n)) > 0) {
			return retCode;
		}
	}
	else if (*str == '\"') {
		if ((retCode = unmarshal_string(&str, n)) > 0) {
			return retCode;
		}
	}
	else if (strncmp(str, "null", 4) == 0) {
		n->type = Null;
		str += 4;
	}
	else if (strncmp(str, "true", 4) == 0) {
		n->type = Bool;
		n->value = 1;
		str += 4;
	}
	else if (strncmp(str, "false", 5) == 0) {
		n->type = Bool;
		n->value = 0;
		str += 5;
	}
	else {
		if ((retCode = unmarshal_number(&str, n)) > 0) {
			return retCode;
		}
	}

	*str_ptr = str;
	return 0;
}

int json_unmarshal(const char *str, json_node **n) {
	if (str == NULL || *str == '\0') {
		*n = NULL;
		return 0;
	}

	int retCode;
	*n = new_json_node();
	if ((retCode = unmarshal_recursive(&str, *n)) > 0) {
		// roll back
		destroy_json_node(*n);
		*n = NULL;
		return retCode;
	}

	return 0;
}
