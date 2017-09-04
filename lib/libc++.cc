#include <new>
#include <stdlib.h>
void* xmalloc_(int size);

extern "C" void* emulate_nt_new(unsigned len, const std::nothrow_t& junk)
	{	return malloc(len);	}
extern "C" void* emulate_cc_new(unsigned len)
	{	return xmalloc_(len); }
extern "C" void emulate_cc_delete(void* p)
	{	free(p); }

void* operator new(std::size_t, const std::nothrow_t&) __attribute__((alias("emulate_nt_new")));
void* operator new[](std::size_t, const std::nothrow_t&) __attribute__((alias("emulate_nt_new")));
void* operator new  (unsigned len) __attribute__((alias("emulate_cc_new")));
void* operator new[](unsigned len) __attribute__((alias("emulate_cc_new")));   
void  operator delete  (void* p)   __attribute__((alias("emulate_cc_delete")));
void  operator delete[](void* p)   __attribute__((alias("emulate_cc_delete")));
