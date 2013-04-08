// Last modified: 2013-04-08 19:45:46
 
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

class ListHandler
{
private:
	unsigned int termid;
	unsigned int length;
	unsigned long long offset;
	unsigned int* pList;
	bool IsFetch;
public:
	ListHandler(
			unsigned _termid,
			DictItem _item)
			//SCacheFrame *SCF);
	~ListHandler();

	int GetItem(unsigned int itemID);

	unsigned long long GetLength();
};
 
#endif
