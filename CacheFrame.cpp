// Last modified: 2013-04-10 20:04:50
 
/**
 * @file: CacheFrame.cpp
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 19:55:41
 * @brief: 
 **/
 
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "function.h"
#include "CacheFrame.h"

SCacheFrame::SCacheFrame(int _size, const char *_name, MemoryDict *_dict)
{
	printf("policy : %s, cache size = %d MB\n", _name, _size);
	SC_Init(_size);
	printf("begin load cache ... \n");
	SC_Load(_name, _dict);
	printf("finish load cache ... \
			term count = %d, GlobalOffset = %llu, capacity = %llu, left = %llu\n",
			term_count, GlobalOffset, cacheCapacity, cacheCapacity - GlobalOffset);
}

void SCacheFrame::SC_Init(int size)
{
	HT_Init();

	cacheCapacity = (unsigned long long)size * 1024 * 1024 / sizeof(int);
	//cacheUnUsed = cacheCapacity;

	pStaticCache = (unsigned int *)malloc(cacheCapacity * sizeof(int));
	checkPointer(pStaticCache, __FILE__, __LINE__);
	
	term_hits = 0; term_total = 0;
	byte_hits = 0; byte_total = 0;
	io_number = 0; io_amount = 0;

	term_count = 0; GlobalOffset = 0;
}

void SCacheFrame::SC_Load(const char *name, MemoryDict *dict)
{
	char pathname[64];
	snprintf(pathname, sizeof(pathname), "data/%s.dict", name);
	FILE *fDict = fopen(pathname, "r");
	checkResource(fDict, pathname);
	
	char line[64];
	unsigned termid, fre, len;
	while (fgets(line, sizeof(line), fDict) != NULL && 
			GlobalOffset < cacheCapacity)
	{
		sscanf(line, "%u %u %u\n", &termid, &fre, &len);
		if (GlobalOffset + len > cacheCapacity)
			break;
		
		unsigned int length = dict->m_vecDict[termid].m_nFreq;
		unsigned long long offset = dict->m_vecDict[termid].m_nOffset;
		assert(len == length);
		fseek(pIndex, offset, SEEK_SET);
		size_t nread = fread(pStaticCache + GlobalOffset, sizeof(int), length, pIndex);
		assert(static_cast<unsigned int>(nread) == length);
		
		HT_Add(termid, GlobalOffset, len);
		
		GlobalOffset += len;
		term_count++;
	}
	closeResource(fDict);
}

SCacheFrame::~SCacheFrame()
{
	print_term_hit_ratio();
	print_byte_hit_ratio();
	print_io_stat();

	freeResource(pStaticCache);
	HT_Free();
}

unsigned int* SCacheFrame::getCachePointer() const
{
	return pStaticCache;
}

void SCacheFrame::update_hits_info(int len)
{
	term_hits++;
	byte_hits += len << 2;
}

void SCacheFrame::update_total_info(int len)
{
	term_total++;
	byte_total += len << 2;
}

void SCacheFrame::print_term_hit_ratio() const
{
	printf("term_hit_ratio = %f\nhits = %f\ttotal = %f\n",
			term_hits / term_total, term_hits, term_total);
}

void SCacheFrame::print_byte_hit_ratio() const
{
	printf("byte_hit_ratio = %f\nhits = %f\ttotal = %f\n",
			byte_hits / byte_total, byte_hits, byte_total);
}

void SCacheFrame::print_io_stat() const
{
	printf("io_number = %f\tio_amount = %f\n", io_number, io_amount);
}

void SCacheFrame::scf_lock_mutex()
{
	pthread_mutex_lock(&cache_stat_mutex);
}

void SCacheFrame::scf_unlock_mutex()
{
	pthread_mutex_unlock(&cache_stat_mutex);
}
