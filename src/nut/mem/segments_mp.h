﻿
#ifndef ___HEADFILE_DE2E0013_ABB5_49D4_AB22_F859CCAFCFA5_
#define ___HEADFILE_DE2E0013_ABB5_49D4_AB22_F859CCAFCFA5_

#include <nut/debugging/destroy_checker.h>
#include <nut/threading/lockfree/atomic.h>

#include "memory_allocator.h"

namespace nut
{

/**
 * 分段缓存内存池
 */
class segments_mp : public memory_allocator
{
    enum
    {
        // 粒度
        GRANULARITY = 8,
        // free list 数
        FREE_LIST_COUNT = 128,
        // 最大 free list 长度
        MAX_FREE_LIST_LENGTH = 50,
    };
    static_assert(GRANULARITY >= sizeof(void*), "Granularity should greater then or equal to a pointer");
    
    typedef struct
    {
        TagedPtr<void> head;
        uint16_t volatile length = 0;
    } FreeList;

    const rc_ptr<memory_allocator> _alloc;
    FreeList _freelist[FREE_LIST_COUNT];
    NUT_DEBUGGING_DESTROY_CHECKER

private:
    segments_mp(const segments_mp&);
    segments_mp& operator=(const segments_mp&);

public:
    segments_mp(memory_allocator *ma=NULL);
    ~segments_mp();

    void clear();

    virtual void* alloc(size_t sz) override;
    virtual void* realloc(void *p, size_t old_sz, size_t new_sz) override;
    virtual void free(void *p, size_t sz) override;
};


}

#endif
