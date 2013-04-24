// Last modified: 2013-04-24 20:19:18
 
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
		// for the details of the following four variables,
		// please refer to the comments in CacheFrame.cpp
		read_oft = offset - offset % BYTE_PER_BLOCK;
		oft_in_block = offset - read_oft;
		int byte_in_1st_block = BYTE_PER_BLOCK - oft_in_block;
		block_num = ((length << 2) - byte_in_1st_block + BYTE_PER_BLOCK - 1) / BYTE_PER_BLOCK + 1;
		
		int ret = posix_memalign((void **)&pAlloc, 512, block_num * BYTE_PER_BLOCK);
		if (ret) {
			fprintf (stderr, "posix_memalign: %s\n", strerror (ret));
			exit(-1);
		}
		
		pthread_mutex_lock(&sr_mutex);
		lseek(fIndex, read_oft, SEEK_SET);
		size_t nread = read(fIndex, pAlloc, block_num * BYTE_PER_BLOCK);
		pthread_mutex_unlock(&sr_mutex);
		assert(static_cast<unsigned int>(nread) == block_num * BYTE_PER_BLOCK);

		pList = pAlloc + (oft_in_block >> 2);
		// Note that the type of 'pTemp' is unsigned int *,
		// while 'oft_in_block' is counted in terms of byte

		// do not need to insert into the HashTable
		IsFetch = 1;
	}
}

ListHandler::~ListHandler()
{
	if (IsFetch)
		//freeResource(pList);
		freeResource(pAlloc);
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
