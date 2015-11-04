/**
  * @file url.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 11/ 3 15:32:10 2015
  */


#include <stdlib.h>
#include <string.h>

#include "url.h"
#include "../../algo/str.h"
#include "../../util/vector.h"


int values_init(values *v) {
	return hash_init(v, 0);
}

const char *get_value(const values *v, const char *key) {
	vector *values_set;
	if (hash_get(v, key, (void**)(&values_set)) > 0 || values_set == NULL || vector_length(values_set) == 0) {
		return NULL;
	}

	const char *ans;
	if (v_get_ptr(values_set, 0, (void**)(&ans)) > 0) {
		return NULL;
	}
	return ans;
}

int add_value(values *v, const char *key, const char *value) {
	vector *values_set;
	if (hash_get(v, key, (void**)(&values_set)) == ERR_HASH_NOTFOUND) {
		values_set = malloc(sizeof(vector));
		vector_init(values_set, 1);
		int retCode;
		if ((retCode = hash_insert(v, key, values_set)) > 0) {
			return retCode;
		}
	}

	return v_push_ptr(values_set, (void*)value);
}

void del_value(values *v, const char *key) {
	vector *values_set;
	if (hash_get(v, key, (void**)(&values_set)) == ERR_HASH_NOTFOUND) {
		return;
	}
	
	vector_destroy(values_set);
	return;
}

int set_value(values *v, const char *key, const char *val) {
	vector *values_set;
	if (hash_get(v, key, (void**)(&values_set)) == 0) {
		vector_clear(values_set);
	}
	else {
		vector_init(values_set, 1);
	}

	return v_push_ptr(values_set, (void*)val);
}

int values_destroy(values *v) {
	return hash_destroy(v);
}


int parse_url(const char *raw_url, http_url *url) {
	const char *start = raw_url, *cur = start;
	
	url->scheme = NULL;
	url->host = NULL;
	url->raw_path = NULL;
	url->raw_query = NULL;

	// extract scheme
	while (*cur != '\0' && *cur != ':') {
		++cur;
	}
	if (*cur == '\0') {
		return ERR_URL_INVALID_URL_PATH;
	}
	url->scheme = malloc(cur - start + 1);
	sstrncpy((char*)(url->scheme), start, cur - start);

	// extract host
	if (*(++cur) != '/' || *(++cur) != '/') {
		return ERR_URL_INVALID_URL_PATH;
	}
	start = ++cur;
	while (*cur != '\0' && *cur != '/') {
		++cur;
	}
	url->host = malloc(cur - start + 1);
	sstrncpy((char*)(url->host), start, cur - start);

	if (*cur == '/') {
		// extract path
		start = ++cur;
		while (*cur != '?' && *cur != '\0') {
			++cur;
		}
		url->raw_path = malloc(cur - start + 1);
		sstrncpy((char*)(url->raw_path), start, cur - start);

		if (*cur == '?') {
			// extract query
			start = ++cur;
			while (*cur != '\0') {
				++cur;
			}
			url->raw_query = malloc(cur - start + 1);
			sstrncpy((char*)(url->raw_query), start, cur - start);
		}
	}

	return 0;
}

void destroy_url(http_url *url) {
	if (url != NULL) {
		free((void*)(url->scheme));
		free((void*)(url->host));
		free((void*)(url->raw_path));
		free((void*)(url->raw_query));
	}
}
