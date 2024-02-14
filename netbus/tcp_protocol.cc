#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcp_protocol.h"

#include "../utils/cache_alloc.h"
extern CacheAllocator* wbuf_allocator;

bool TCPProtocol::read_header(const unsigned char* data, int data_len, int* header_len, int* package_len)
{
	if (data_len < 2) {
		return false;
	}

	*package_len = (data[0] | (data[1] << 8));
	*header_len = 2;

	return true;
}

unsigned char* TCPProtocol::package(const unsigned char* data, int data_len, int* package_len)
{
	unsigned char* package = (unsigned char*)cache_alloc(wbuf_allocator, data_len + 2);
	package[0] = data_len & 0xff;
	package[1] = (data_len >> 8) & 0xff;
	memcpy(package + 2, data, data_len);
	*package_len = data_len + 2;
	return package;
}

void TCPProtocol::free_package(unsigned char* package)
{
	cache_free(wbuf_allocator, package);
}
