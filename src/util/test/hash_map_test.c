/**
  * @file hash_map_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二  9/ 8 22:58:13 2015
  */


#include <stdio.h>
#include <stdlib.h>

#include "../hash_map.h"


int main(int argc, char *argv[])
{
	hash_map *map = (hash_map*)malloc(sizeof(hash_map));
	hash_init(map, 1);

	int n = 10;
	char **keys = malloc(n * sizeof(char*));
	for (int i = 0; i < n; ++i) {
		keys[i] = malloc(2);
		keys[i][0] = '0' + i;
		keys[i][1] = '\0';
	}

	// test insert
	for (int i = 0; i < n; ++i) {
		hash_insert(map, keys[i], keys[i]);
		printf("current size: %lu\tcapacity: %lu\n", map->size, map->capacity);
	}

	// test get and update
	printf("before update:\n");
	for (int i = 0; i < n; ++i) {
		void *value = NULL;
		hash_get(map, keys[i], &value);
		printf("%s: %s\n", keys[i], (const char*)value);
	}
	hash_update(map, keys[2], "two");
	hash_update(map, keys[7], "seven");
	printf("after update:\n");
	for (int i = 0; i < n; ++i) {
		void *value = NULL;
		hash_get(map, keys[i], &value);
		printf("%s: %s\n", keys[i], (const char*)value);
	}

	// test remove
	for (int i = 0; i < n; ++i) {
		hash_remove(map, keys[i]);
		printf("current size: %lu\tcapacity: %lu\n", map->size, map->capacity);
	}

	hash_destroy(map);

	return 0;
}
