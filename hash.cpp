// Last modified: 2013-04-08 18:27:45
 
/**
 * @file: hash.cpp
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-07 17:04:59
 * @brief: 
 **/
 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "hash.h"
#include "function.h"
 
void HT_Init()
{
	for (unsigned int i = 0; i < MAX_HASH; ++i)
	{
		hashTable[i] = (hashnode_t *)malloc(sizeof(hashnode_t));
		checkPointer(hashTable[i], __LINE__);
		memset(hashTable[i], 0, sizeof(hashnode_t));
	}
}

void HT_Free()
{
	for (unsigned int i = 0; i < MAX_HASH; ++i)
	{
		hashnode_t *temp = hashTable[i]->h_next;
		hashnode_t *todel;
		while (temp != NULL)
		{
			todel = temp;
			temp = temp->h_next;
			freeResource(todel);
		}
		freeResource(hashTable[i]);
	}
}

void HT_Add(unsigned int termid,
			unsigned long long offset,
			unsigned int length)
{
	int slot = HashKey(termid);
	
	hashnode_t *hnode = (hashnode_t *)malloc(sizeof(hashnode_t));
	checkPointer(hnode, __FILE__, __LINE__);
	memset(hnode, 0, sizeof(hashnode_t));

	hnode->m_key = termid;
	hnode->m_offset = offset;
	hnode->m_length = length;
	hnode->h_next = hashTable[slot]->next;
	hashTable[slot]->next = hnode;
}

hashnode_t *HT_GetNode(unsigned int termid)
{
	int slot = HashKey(termid);
	hashnode_t *p = hashTable[slot];
	hashnode_t *q;
	while ((q = p->h_next) != NULL)
	{
		if (q->m_key == termid)
			break;
		p = p->h_next;
	}
	return q;
}
/*
int existInHash(unsigned int termid)
{
	hashnode_t *tmp = getHashNode(termid);
	return (tmp != NULL) ? 1 : 0;
}
*/
