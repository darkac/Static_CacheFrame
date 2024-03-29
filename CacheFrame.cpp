// Last modified: 2013-04-26 15:32:58
 
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
#include <sys/stat.h>

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

	if (pthread_mutex_init(&cache_stat_mutex, NULL) != 0)
	{
		printf("cache_stat_mutex init failed.\n");
		exit(-1);
	}
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

	struct stat sb;
	if (fstat(fIndex, &sb) == -1)
	{
		fprintf (stderr, "fstat: failed\n");
		exit(-1);
	}
	unsigned long long index_size = static_cast<unsigned long long>(sb.st_size);
	// sb.st_size is the size of the index file, in bytes

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
		
		unsigned long long read_oft = offset - offset % BYTE_PER_BLOCK; // in terms of byte
		// read_oft : the value passed to lseek(), which is aligned with 512
		
		unsigned long long oft_in_block = offset - read_oft; // in terms of byte
		// the required data begins at this address: (char *)&pTemp + oft_in_block
		
		int byte_in_1st_block = BYTE_PER_BLOCK - oft_in_block; // in terms of byte
		unsigned int block_num = ((length << 2) - byte_in_1st_block + BYTE_PER_BLOCK - 1) / BYTE_PER_BLOCK + 1;
		// block_num : the number of blocks that should be read
		
		unsigned long long bytes_to_read = block_num * BYTE_PER_BLOCK;

		unsigned int *pTemp;
		int ret = posix_memalign((void **)&pTemp, 512, bytes_to_read);
		if (ret) {
			fprintf (stderr, "posix_memalign: %s\n", strerror (ret));
			exit(-1);
		}
		
		lseek(fIndex, read_oft, SEEK_SET);
		size_t nread = read(fIndex, pTemp, bytes_to_read);
		//assert(static_cast<unsigned int>(nread) == block_num * BYTE_PER_BLOCK);
		// Note that {block_num * BYTE_PER_BLOCK >= length * sizeof(int)}
		
		// Note that if the termid is in the end of the index file,
		// the bytes actually read ('nread') might be less than bytes_to_read
		if ((index_size - read_oft) >= bytes_to_read)
			assert(static_cast<unsigned int>(nread) == bytes_to_read);
		else
			assert(static_cast<unsigned int>(nread) == (index_size - read_oft));
		
		memcpy(pStaticCache + GlobalOffset, pTemp + (oft_in_block >> 2), length * sizeof(int));
		// Note that the type of 'pTemp' is unsigned int *,
		// while 'oft_in_block' is counted in terms of byte
		freeResource(pTemp);
		
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

	pthread_mutex_destroy(&cache_stat_mutex);
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
