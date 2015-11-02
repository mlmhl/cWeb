/**
  * @file json.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/22 14:58:56 2015
  */


#ifndef JSON_H
#define JSON_H


#include "../../util/vector.h"


static const int ERR_JSON_INVALID_TYPE = 0x01;
static const int ERR_JSON_INVALID_TAG = 0x02;
static const int ERR_JSON_INVALID_STRING = 0x04;
static const int ERR_JSON_INVALID_NUMBER = 0x08;
static const int ERR_JSON_INVALID_OBJECT = 0x10;
static const int ERR_JSON_INVALID_ARRAY = 0x20;


typedef enum {
	Int = 1,
	Float,
	String,
	Bool,
	Array,
	Object,
	Null,
}json_type;


typedef struct json_node {
	json_type type;
	const char *key;

	union {
		vector*		       arr;	    // Array
		__int8_t		   value;	// Bool
		__int64_t		   inumber; // Number
		double			   fnumber; // Float
		const char*		   str;	    // String
		struct json_node*  object;	// Object
	};

	struct json_node *next;	

}json_node;


// json_node should be created in the heap totally
// invoker should invoke destroy_json_node
json_node *new_json_node();
void destroy_json_node(json_node *n);
int json_marshal(json_node *n, const char **str);
int json_unmarshal(const char *str, json_node **n);


#endif // JSON_H
