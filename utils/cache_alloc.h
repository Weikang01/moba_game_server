#ifndef __CACHE_ALLOC_H__
#define __CACHE_ALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif
	struct CacheAllocator* create_cache_allocator(int capacity, int element_size);
	void destroy_cache_allocator(struct CacheAllocator* allocator);

	void* cache_alloc(struct CacheAllocator* allocator, int elem_size);
	void cache_free(struct CacheAllocator* allocator, void* ptr);
#ifdef __cplusplus
}
#endif


#endif // !__CACHE_ALLOC_H__
