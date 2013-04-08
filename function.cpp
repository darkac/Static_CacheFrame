// Last modified: 2013-04-07 20:08:48
 
/**
 * @file: function.cpp
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2012-08-27 11:27:15
 * @brief: 
 **/
 
/* Talk is cheap, show me the code. */
/* Good luck and have fun. */

#include <cstdio>
#include <cstdlib>

#include "function.h"

void checkPointer(void *p, const char *filename, const int line);
{
	if (p == NULL)
	{
		printf("null pointer in file %s, line %d\n", filename, line);
		exit(-1);
	}
}

void checkResource(void *p, const char *source_name)
{
	if (p == NULL)
	{
		printf("Failed to open `%s`\n", source_name);
		exit(-1);
	}
}

void freeResource(void *pointer)
{
	if (pointer)
	{
		free(pointer);
		pointer = 0;
	}
}

void closeResource(FILE *fHandler)
{
	if (fHandler)
		fclose(fHandler);
}

void testPoint(int line)
{
	printf("Arrive here, line %d\n", line);
}
