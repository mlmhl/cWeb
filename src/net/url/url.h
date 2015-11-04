/*
  * @file url.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 11/ 3 11:02:21 2015
  */


#ifndef URL_H
#define URL_H


#include "../../util/hash_map.h"


static const int ERR_URL_INVALID_URL_PATH = 0x01;


typedef hash_map values;

int values_init(values *v);
const char *get_value(const values *v, const char *key);
int add_value(values *v, const char *key, const char *val);
void del_value(values *v, const char *key);
int set_value(values *v, const char *key, const char *val);
int values_destroy(values *v);


typedef struct {
	const char *scheme;
	const char *host;
	const char *raw_path;
	const char *raw_query;
}http_url;

int parse_url(const char *raw_url, http_url *url);
void destroy_url(http_url *url);


#endif // URL_H
