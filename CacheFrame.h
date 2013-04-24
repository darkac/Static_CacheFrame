// Last modified: 2013-04-24 17:21:05
 
/**
 * @file: CacheFrame.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 17:13:34
 * @brief: 
 **/
 
#ifndef _CACHEFRAME_H_
#define _CACHEFRAME_H_
 
#include <string>
#include <pthread.h>
#include "hash.h"
#include "MemoryDict.h"

const int MEMORYSIZE = 1024; // in terms of MB
const int BYTE_PER_BLOCK = 512;

class SCacheFrame
{
protected:
	pthread_mutex_t cache_stat_mutex;

	unsigned long long cacheCapacity; // in terms of int
	unsigned long long cacheUnUsed;   // in terms of int

	unsigned long long GlobalOffset;  // in terms of int
	unsigned int *pStaticCache;
	unsigned int term_count;
	
	double term_hits; // in terms of number
	double term_total;
	
	double byte_hits; // in terms of byte
	double byte_total;
	
	double io_number; // in terms of number
	double io_amount; // in terms of int

public:
	SCacheFrame(int _size, const char *_name, MemoryDict *_dict);
	~SCacheFrame();
	
	void update_hits_info(int len);
	void update_total_info(int len);
	
	void print_term_hit_ratio() const;
	void print_byte_hit_ratio() const;
	void print_io_stat() const;

	void SC_Init(int size);
	void SC_Load(const char *name, MemoryDict *dict);

	unsigned int *getCachePointer() const;
	
	void scf_lock_mutex();
	void scf_unlock_mutex();
};
extern SCacheFrame *SCF;
 
#endif
