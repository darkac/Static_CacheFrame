// Last modified: 2013-04-10 20:06:19
 
/**
 * @file: ListHandler.cpp
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-08 19:09:31
 * @brief: 
 **/
 
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "function.h"
#include "ListHandler.h"

pthread_mutex_t sr_mutex = PTHREAD_MUTEX_INITIALIZER;

ListHandler::ListHandler(
		unsigned _termid,
		DictItem _item)
		//SCacheFrame *SCF)
{
	termid = _termid;
	length = _item.m_nFreq;
	offset = _item.m_nOffset;
	IsFetch = 0;

	SCF->scf_lock_mutex();
	SCF->update_total_info(length);
	SCF->scf_unlock_mutex();
	
	hashnode_t *res = HT_GetNode(termid);
	if (res != NULL)
	{
		SCF->scf_lock_mutex();
		SCF->update_hits_info(length);
		SCF->scf_unlock_mutex();
		
		pList = SCF->getCachePointer() + res->m_offset;
	}
	else
	{
		pList = (unsigned int *)malloc(length * sizeof(unsigned int));
		checkPointer(pList, __FILE__, __LINE__);
		
		pthread_mutex_lock(&sr_mutex);
		fseek(pIndex, offset, SEEK_SET);
		size_t nread = fread(pList, sizeof(unsigned int), length, pIndex);
		pthread_mutex_unlock(&sr_mutex);
		assert(static_cast<unsigned int>(nread) == length);
		// do not need to insert into the HashTable
		IsFetch = 1;
	}
}

ListHandler::~ListHandler()
{
	if (IsFetch)
		freeResource(pList);
	else
		pList = NULL;
}

int ListHandler::GetItem(unsigned int itemID)
{
	return pList[itemID];
}

unsigned int ListHandler::GetLength()
{
	return length;
}
