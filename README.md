Static_CacheFrame
=================

A Framework for Static Caching in Web Search Engines.

Usage:

1) `SCacheFrame *SCF = new SCacheFrame(MEMORYSIZE, "dictname", &dict);`
// Note that `SCF` should be defined as a global variable

2) For each term, `ListHandler *LH = new ListHandler(termid, DictItem _item);`
// See the definition of `DictItem` in `MemoryDict.h`

3) For more details, please refer to `testSCF_threads.cpp`.

Besides, this module is multi-thread safe.
