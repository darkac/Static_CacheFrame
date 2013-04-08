// Last modified: 2013-04-08 19:23:11
 
/**
 * @file: hash.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 16:40:47
 * @brief: 
 **/
 
#ifndef _HASH_H_
#define _HASH_H_
 
#define MAX_HASH 450000

typedef struct hashnode {
	unsigned int m_key;
	unsigned long long m_offset;
	// offset in the cache space, in terms of int
	unsigned int m_length;
	struct hashnode *h_next;
} hashnode_t;
//extern hashnode_t *hashTable[MAX_HASH];

inline int HashKey(const unsigned int key)
{
	return key % MAX_HASH;
}

void HT_Init();

void HT_Free();

void HT_Add(unsigned int termid,
			unsigned long long offset,
			unsigned int length);

//int existInHash(unsigned int termid);

hashnode_t *HT_GetNode(unsigned int termid);
 
#endif
