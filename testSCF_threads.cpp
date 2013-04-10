// Last modified: 2013-04-10 15:49:09
 
/**
 * @file: testSCF_threads.cpp
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-04-09 17:09:07
 * @brief: 
 **/
 
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <pthread.h>
#include "function.h"
#include "ListHandler.h"

using namespace std;

#define THREAD_NUM 1000
#define TERM_NUM 100000

FILE *pIndex;
MemoryDict dict;
SCacheFrame *SCF;
pthread_mutex_t ot_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t sr_mutex = PTHREAD_MUTEX_INITIALIZER;

void LoadDict()
{
	string strSrc = "/home/ac/Data/GOV-";
	string strDictDir = strSrc + "dict/";
	string strDictName = "GOV2.Rand";
	bool bRet = dict.Load(strDictDir, strDictName);

	if(!bRet) {
		cout << "Error in reading the dict" << endl;
		return ;
	}
	string file =strSrc + "index/" + strDictName + ".index";
	pIndex = fopen(file.c_str(), "rb");
	assert(pIndex != NULL);
}

void *run(void *arg)
{
	unsigned int range = 1024;
	int tno = *(int *)arg;
	
	for (int i = 0; i < TERM_NUM / THREAD_NUM; ++i)
	{
		int termid =  TERM_NUM / THREAD_NUM * tno + i;
		int docid = 1023;
		ListHandler *LH = new ListHandler(termid, dict.m_vecDict[termid]);
		if (LH->GetLength() >= range)
		{
			pthread_mutex_lock(&ot_mutex);
			cout << termid << " - " << docid << " --> " 
				<< LH->GetItem(docid) << "\t" << LH->GetLength() << endl;
			pthread_mutex_unlock(&ot_mutex);
		}

		delete LH;
	}
	return (void *)0;
}

int main()
{
    LoadDict();
	
	SCF = new SCacheFrame(MEMORYSIZE, "test", &dict);
	
	pthread_t tid[THREAD_NUM];
	int arg[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		arg[i] = i;
		pthread_create(&tid[i], NULL, run, &arg[i]);
	}
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		pthread_join(tid[i], NULL);
	}
	
	int range = 1024;
	int docid = 1023;
	int listLength, offset;
	int *space = (int *)malloc(1024);
	for (int termid = 0; termid < TERM_NUM; ++termid)
	{
		listLength = dict.m_vecDict[termid].m_nFreq;
		offset = dict.m_vecDict[termid].m_nOffset;
		space = (int *)realloc(space, listLength * sizeof(int));
		fseek(pIndex, offset, SEEK_SET);
		fread(space, sizeof(int), listLength, pIndex);
		if (listLength >= range)
		{
			cerr << termid << " - " << docid << " --> " 
				<< space[docid] << "\t" << listLength << endl;
		}

	}
	freeResource(space);

	delete SCF;
	closeResource(pIndex);
    
	return 0;
}
