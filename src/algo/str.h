/**
  * @file str.h
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 09:18:59 2015
  */


#ifndef STR_H
#define STR_H

#include "../util/vector.h"


static const int NORMAL = 0x01;
static const int SCIENCE = 0x02;


int str_contains(const char *str, char c);

char *itoa(int value, char *str, int base);

char *ftoa(double value, char *str, int precision, int flag);

void str_split(char *str, const char *delim, vector *res);


#endif // STR_H
