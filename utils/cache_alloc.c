#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache_alloc.h"

struct node {
	struct node* next;
};

struct CacheAllocator {
	unsigned char* cache_mem;
	int capacity;
	struct node* free_list;
	int element_size;
};

struct CacheAllocator* create_cache_allocator(int capacity, int element_size)
{
	int i;

	struct CacheAllocator* allocator = (struct CacheAllocator*)malloc(sizeof(struct CacheAllocator));
	memset(allocator, 0, sizeof(struct CacheAllocator));
	element_size = (element_size < sizeof(struct node)) ? sizeof(struct node) : element_size;
	allocator->capacity = capacity;
	allocator->element_size = element_size;
	allocator->cache_mem = (unsigned char*)malloc(capacity * element_size);
	memset(allocator->cache_mem, 0, capacity * element_size);

	allocator->free_list = NULL;

	for (i = 0; i < capacity; i++) {
		struct node* n = (struct node*)(allocator->cache_mem + i * element_size);
		n->next = allocator->free_list;
		allocator->free_list = n;
	}

	return allocator;
}

void destroy_cache_allocator(struct CacheAllocator* allocator)
{
	if (allocator->cache_mem != NULL) {
		free(allocator->cache_mem);
	}
	free(allocator);
}

void* cache_alloc(struct CacheAllocator* allocator, int elem_size)
{
	if (allocator->element_size < elem_size) {
		return malloc(elem_size);
	}

	if (allocator->free_list == NULL) {
		return malloc(elem_size);
	}

	struct node* n = allocator->free_list;
	allocator->free_list = n->next;
	return n;
}

void cache_free(struct CacheAllocator* allocator, void* ptr)
{
	if (ptr == NULL) {
		return;
	}

	if ((unsigned char*)ptr >= allocator->cache_mem && (unsigned char*)ptr < allocator->cache_mem + allocator->capacity * allocator->element_size) {
		struct node* n = (struct node*)ptr;
		n->next = allocator->free_list;
		allocator->free_list = n;
	}
	else {
		free(ptr);
	}
}
