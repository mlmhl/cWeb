/**
  * @file hash_map.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 五  9/ 4 22:31:23 2015
  * @brief
  * hash_map is a hash table for string key, implemeted by 拉链法
  */

#ifndef HASH_MAP_H
#define HASH_MAP_H


#include <stddef.h>


#define ERR_HASH_ALLOC 0x1;
#define ERR_HASH_REHASH 0x2;
#define ERR_HASH_EXIST 0x4;
#define ERR_HASH_NOTFOUND 0x8;


typedef struct Node {
	const char *key;
	void *value;
	struct Node *next;
}node;


typedef struct {
	node **buckets;
	size_t size;
	size_t capacity;

}hash_map;


// all functions below return 0 when successed
// otherwise return a positive integer specify the error

int hash_init(hash_map *map, size_t capacity);

int hash_insert(hash_map *map, const char *key, void *value);

int hash_remove(hash_map *map, const char *key);

int hash_update(hash_map *map, const char *key, void *value);

int hash_get(hash_map *map, const char *key, void **value);

int hash_destroy(hash_map *map);

#endif // HASH_MAP_H
