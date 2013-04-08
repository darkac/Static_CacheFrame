// Last modified: 2013-04-08 02:51:58
 
/**
 * @file: CacheFrame.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 17:13:34
 * @brief: 
 **/
 
#ifndef _CACHEFRAME_H_
#define _CACHEFRAME_H_
 
#include <string>
#include "hash.h"
#include "MemoryDict.h"

#define POLICYNUM 4
enum PolicyType {SCP_QTF = 0, SCP_QTFDF, SCP_IPM, SCP_BLOCK};
extern string Policy_Name[POLICYNUM];
const int MEMORYSIZE 1024; // in terms of Byte

class SCacheFrame
{
protected:
	PolicyType policy_name;

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
	//virtual ~SCacheFrame();
	
	void update_hits_info(int len);
	void update_total_info(int len);
	
	void print_term_hit_ratio() const;
	void print_byte_hit_ratio() const;
	void print_io_stat() const;

	virtual bool ExistInCache(const unsigned int temrid) const;

	void SC_Init(int size);
	void SC_Load(const char *name, MemoryDict *dict);
};
extern SCacheFrame *SCF;
 
#endif