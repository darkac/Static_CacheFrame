// Last modified: 2013-04-08 21:07:51
 
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

ListHandler::ListHandler(
		unsigned _termid,
		DictItem _item)
		//SCacheFrame *SCF)
{
	termid = _termid;
	length = _item.m_nFreq;
	offset = _item.m_nOffset;
	pList = NULL;
	IsFetch = 0;

	SCF->update_total_info(length);
	hashnode_t *res = HT_GetNode(termid);
	if (res != NULL)
	{
		SCF->update_hits_info(length);
		pList = SCF->getCachePointer() + res->m_offset;
	}
	else
	{
		fseek(pIndex, offset, SEEK_SET);
		size_t nread = fread(pList, sizeof(int), length, pIndex);
		assert(static_cast<unsigned int>(nread) == length);
		// do not need to insert into the HashTable
		IsFetch = 1;
	}
	assert(pList != NULL);
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
