#ifndef __CACHE_ALLOC_H__
#define __CACHE_ALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif
	struct cache_allocator* create_cache_allocator(int capacity, int element_size);
	void destroy_cache_allocator(struct cache_allocator* allocator);

	void* cache_alloc(struct cache_allocator* allocator, int elem_size);
	void cache_free(struct cache_allocator* allocator, void* ptr);
#ifdef __cplusplus
}
#endif


#endif // !__CACHE_ALLOC_H__
