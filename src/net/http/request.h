/**
  * @file request.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 11/ 3 17:21:12 2015
  */


#ifndef REQUEST_H
#define REQUEST_H


#include "../url/url.h"


typedef struct {
	const char *method;
	http_url *url;

	
}htttp_request;


#endif // REQUEST_H
