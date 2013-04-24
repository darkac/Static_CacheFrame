// Last modified: 2013-04-24 16:49:00
 
/**
 * @file: function.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 17:07:29
 * @brief: 
 **/
 
/* Talk is cheap, show me the code. */
/* Good luck and have fun. */
 
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <cstdio>

extern int fIndex;

void checkPointer(void *p, const char *filename, const int line);

void checkResource(void *p, const char *source_name);

void freeResource(void *pointer);

void closeResource(FILE *fHandler);

void testPoint(int line);

#endif
