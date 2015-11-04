/**
  * @file url_test.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 二 11/ 3 16:40:52 2015
  */


#include <stdio.h>

#include "url.h"


void url_test(const char *raw_url, http_url *url) {
	parse_url(raw_url, url);
	printf("scheme: %s\n", url->scheme);
	printf("host: %s\n", url->host);
	printf("raw_path: %s\n", url->raw_path);
	printf("raw_query: %s\n", url->raw_query);
}


int main(int argc, char *argv[]) {
	http_url url;

	url_test("http:/", &url);
	printf("\n");
	url_test("http://wwww.baidu.com", &url);
	printf("\n");
	url_test("http://www.baidu.com/index.html", &url);
	printf("\n");
	url_test("http://www.baidu.com/index.html?name=malin&password=123456", &url);

	destroy_url(&url);

	return 0;
}
