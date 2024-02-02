#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache_alloc.h"

struct node {
	struct node* next;
};

struct cache_allocator {
	unsigned char* cache_mem;
	int capacity;
	struct node* free_list;
	int element_size;
};

struct cache_allocator* create_cache_allocator(int capacity, int element_size)
{
	struct cache_allocator* allocator = (struct cache_allocator*)malloc(sizeof(struct cache_allocator));
	memset(allocator, 0, sizeof(struct cache_allocator));
	element_size = (element_size < sizeof(struct node)) ? sizeof(struct node) : element_size;
	allocator->capacity = capacity;
	allocator->element_size = element_size;
	allocator->cache_mem = (unsigned char*)malloc(capacity * element_size);
	memset(allocator->cache_mem, 0, capacity * element_size);

	allocator->free_list = NULL;

	for (int i = 0; i < capacity; i++) {
		struct node* n = (struct node*)(allocator->cache_mem + i * element_size);
		n->next = allocator->free_list;
		allocator->free_list = n;
	}

	return allocator;
}

void destroy_cache_allocator(struct cache_allocator* allocator)
{
	if (allocator->cache_mem != NULL) {
		free(allocator->cache_mem);
	}
	free(allocator);
}

void* cache_alloc(struct cache_allocator* allocator, int elem_size)
{
	if (allocator->element_size < elem_size) {
		return NULL;
	}

	if (allocator->free_list == NULL) {
		return malloc(elem_size);
	}

	struct node* n = allocator->free_list;
	allocator->free_list = n->next;
	return n;
}

void cache_free(struct cache_allocator* allocator, void* ptr)
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
