/**
  * @file json_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 日 10/25 15:33:07 2015
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"


void marshal_test() {
	json_node *node = new_json_node();
	node->type = Object;
	node->key = "Root";
	node->next = NULL;

	json_node *elem = malloc(sizeof(json_node));
	node->object = elem;
	
	elem->type = Int;
	elem->key = "Int";
	elem->inumber = 3;
	elem->next = malloc(sizeof(json_node));
	elem = elem->next;

	elem->type = Float;
	elem->key = "Float";
	elem->fnumber = 12345.6789;
	elem->next = malloc(sizeof(json_node));
	elem = elem->next;

	elem->type = String;
	elem->key = "String";
	elem->str = "hello world";
	elem->next = malloc(sizeof(json_node));
	elem = elem->next;

	elem->type = Bool;
	elem->key = "Bool";
	elem->value = 1;
	elem->next = malloc(sizeof(json_node));
	elem = elem->next;

	elem->type = Array;
	elem->key = "Array";
	elem->arr = malloc(sizeof(vector));
	vector_init(elem->arr, 2);
	json_node *a1 = malloc(sizeof(json_node));
	a1->type = Int;
	a1->key = "Array1";
	a1->inumber = 1024;
	a1->next = NULL;
	json_node *a2 = malloc(sizeof(json_node));
	a2->type = String;
	a2->key = "Array2";
	a2->str = "hello 世界";
	a2->next = NULL;
	v_push_ptr(elem->arr, a1);
	v_push_ptr(elem->arr, a2);
	elem->next = malloc(sizeof(json_node));
	elem = elem->next;

	elem->type = Object;
	elem->key = "Object";
	json_node *o1 = malloc(sizeof(json_node));
	elem->object = o1;
	elem->next = NULL;
	o1->type = Int;
	o1->key = "Object1";
	o1->inumber = 123;
	json_node *o2 = malloc(sizeof(json_node));
	o1->next = o2;
	o2->type = String;
	o2->key = "Object2";
	o2->str = "object test";
	o2->next = NULL;


	const char *data = NULL;
	int retCode = json_marshal(node, &data);
	if (retCode > 0) {
		printf("%d\n", retCode);
	}
	else {
		printf("%s\n", data);
		free((void*)data);
		// As node is not allocated in the heap totally, so we can't destroy it
		//destroy_json_node(node);
	}
}


void unmarshal_test() {
	const char *data = "{Int:3,Float:12345.7,String:\"hello world\",Bool:true,Array:[Array1:1024,Array2:\"hello 世界\"],Object:{Object1:123,Object2:\"object test\"}}";
	json_node *n;

	int retCode = json_unmarshal(data, &n);
	if (retCode > 0) {
		printf("unmarshal error: %d\n", retCode);
	}
	else {
		const char *rdata;
		retCode = json_marshal(n, &rdata);
		if (retCode > 0) {
			printf("marshal error: %d\n", retCode);
		}
		else {
			if (strcmp(data, rdata) != 0) {
				printf("%s unequal %s\n", rdata, data);
			}
			else {
				printf("successed\n");
			}
			free((void*)rdata);
			destroy_json_node(n);
		}
	}
}


int main(int argc, char *argv[]) {
	// test marshal
	marshal_test();

	// test unmarshal
	unmarshal_test();
	
	return 0;
}
