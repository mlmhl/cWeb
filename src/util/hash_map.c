/**
  * @file hash_map.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/ 8 09:25:16 2015
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"


static void IDebug(const char *key,unsigned int index) {
	#ifdef DEBUG
		printf("insert %s into buckets %u\n", key, index);
	#endif
}

static void RDebug(size_t old_cap, size_t new_cap) {
	#ifdef DEBUG
		printf("rehash from %lu to %lu\n", old_cap, new_cap);
	#endif
}


static unsigned int hash(const char *key) {
	unsigned int val = 0;
	while (*key != '\0') {
		val *= 256;
		val += *(key++);
	}
	return val;
}

static int insert_node(hash_map *map, node *n) {
	unsigned int index = hash(n->key) % map->capacity;
	for (node *itr = map->buckets[index]; itr != NULL; itr = itr->next) {
		if (strcmp(itr->key, n->key) == 0)
			return ERR_HASH_EXIST;
	}
	n->next = map->buckets[index];
	map->buckets[index] = n;
	IDebug(n->key, index);
	return 0;
}

static int rehash(hash_map *map, size_t cap) {
	RDebug(map->capacity, cap);
	node **old_buckets = map->buckets;
	map->buckets = (node**)malloc(sizeof(node*) * cap);
	if (map->buckets == NULL) {
		map->buckets = old_buckets;
		return ERR_HASH_ALLOC;
	}
	memset(map->buckets, 0, sizeof(node*) * cap);

	size_t old_cap = map->capacity;
	map->capacity = cap;
	for (int i = 0; i < old_cap; ++i) {
		node *head = old_buckets[i], *next;
		while (head != NULL) {
			next = head->next;
			// insert_node certainly will succeed, so we ignored the return value
			insert_node(map, head);
			head = next;
		}
	}
	free(old_buckets);
	return 0;
}


static int get_node(hash_map *map, const char *key, node **n) {
	unsigned int index = hash(key) % map->capacity;
	node *head = map->buckets[index];
	if (head == NULL)
		return ERR_HASH_NOTFOUND;

	if (strcmp(head->key, key) == 0) {
		*n = head;
		return 0;
	}
	
	while (head->next != NULL) {
		if (strcmp(head->next->key, key) == 0) {
			*n = head->next;
			return 0;
		}
		head = head->next;
	}
	return ERR_HASH_NOTFOUND;
}


int hash_init(hash_map *map, size_t capacity) {
	map->size = 0;
	map->capacity = capacity;
	
	if (capacity > 0) {
		map->buckets = (node**)malloc(sizeof(node*) * capacity);
		if (map->buckets == NULL)
			return ERR_HASH_ALLOC;
		memset(map->buckets, 0, sizeof(node*) * capacity);
	}
	else {
		map->buckets = NULL;
	}

	return 0;
}


int hash_insert(hash_map *map, const char *key, void *value) {
	int res = 0;
	if (map->size >= map->capacity << 1) {
		res = rehash(map, map->capacity << 1);
		if (res != 0)
			return res;
	}

	node *new_node = (node*)malloc(sizeof(node));
	if (new_node == NULL)
		return ERR_HASH_ALLOC;
	new_node->key = key;
	new_node->value = value;
	res = insert_node(map, new_node);
	if (res == 0)
		++map->size;
	return res;
}


int hash_remove(hash_map *map, const char *key) {
	unsigned int index = hash(key) % map->capacity;
	node *head = map->buckets[index];
	if (head == NULL)
		return ERR_HASH_NOTFOUND;
	
	int res = ERR_HASH_NOTFOUND;
	if (strcmp(head->key, key) == 0) {
		map->buckets[index] = head->next;
		free(head);
		res = 0;
	}
	else {
		while (head->next != NULL) {
			if (strcmp(head->next->key, key) == 0) {
				head->next = head->next->next;
				free(head->next);
				res = 0;
				break;
			}
			head = head->next;
		}
	}

	if (res == 0 && --map->size < map->capacity >> 1) {
		res = rehash(map, map->size);
	}
	return res;
}


int hash_update(hash_map *map, const char *key, void *value) {
	node *n = NULL;
	int res = get_node(map, key, &n);
	if (res != 0)
		return res;
	n->value = value;
	return 0;
}


int hash_get(hash_map *map, const char *key, void **value) {
	node *n = NULL;
	int res = get_node(map, key, &n);
	if (res != 0)
		return res;
	*value = n->value;
	return 0;
}


// current version, hash_destroy always return 0
int hash_destroy(hash_map *map) {
	node *head, *next;
	for (size_t i = 0; i < map->size; ++i) {
		head = map->buckets[i];
		while (head != NULL) {
			next = head->next;
			free(head);
			head = next;
		}
	}
	free(map->buckets);
	return 0;
}
