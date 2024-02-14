#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache_alloc.h"
#include "small_alloc.h"

#define SMALL_ELEM_COUNT 1024 * 10
#define SMALL_ELEM_SIZE 128
static struct CacheAllocator* small_allocator = NULL;

void* small_alloc(unsigned int size)
{
    if (small_allocator == NULL) {
        small_allocator = create_cache_allocator(SMALL_ELEM_COUNT, SMALL_ELEM_SIZE);
    }

    return cache_alloc(small_allocator, size);
}

void small_free(void* p)
{
    if (small_allocator == NULL) {
        small_allocator = create_cache_allocator(SMALL_ELEM_COUNT, SMALL_ELEM_SIZE);
    }

    cache_free(small_allocator, p);
}
