// Last modified: 2013-04-24 17:21:32
 
/**
 * @file: ListHandler.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-08 18:52:28
 * @brief: 
 **/
 
#ifndef _LISTHANDLER_H_
#define _LISTHANDLER_H_
 
#include "hash.h"
#include "MemoryDict.h"
#include "CacheFrame.h"
#include <pthread.h>

extern pthread_mutex_t sr_mutex; // mutex for [seek + read] combined operation

class ListHandler
{
private:
	unsigned int termid;
	unsigned int length;
	unsigned long long offset; // in terms of byte
	
	/*****for DIRECT IO*****/
	// In order to utilize DIRECT IO:
	// (1) the 'offset' for lseek() should be a multiple of 512
	// (2) the 'length' for read() shoud be a multiple of 512 too
	unsigned long long read_oft; // in terms of byte
	unsigned long long oft_in_block; // in terms of byte
	unsigned int block_num; // the number of blocks that should be read
	// A block contains 512 byte, which is, 128 int
	/*****for DIRECT IO*****/
	
	unsigned int *pAlloc;
	unsigned int *pList;
	bool IsFetch;
public:
	ListHandler(
			unsigned _termid,
			DictItem _item);
			//SCacheFrame *SCF);
	~ListHandler();

	int GetItem(unsigned int itemID);

	unsigned int GetLength();
};
 
#endif
