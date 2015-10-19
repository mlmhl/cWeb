/**
  * @file str.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 09:18:59 2015
  */


#ifndef STR_H
#define STR_H

#include "../util/vector.h"

int str_contains(const char *str, char c);

char *itoa(int value, char *str, int base);

void str_split(char *str, const char *delim, vector *res);


#endif // STR_H
