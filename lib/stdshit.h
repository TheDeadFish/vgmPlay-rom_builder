#ifndef _STDSHIT_H_
#define _STDSHIT_H_
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

// Macro fest
#define NORETURN  __attribute__((noreturn))
#define UNREACH __builtin_unreachable()
#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#define ZINIT memset(this, 0, sizeof(*this))
#ifndef _UNICODE
 #define TSTRPC "%s"
 #define WSTRPC "%S"
#else
 #define TSTRPC "%S"
 #define WSTRPC "%s"
#endif

// Data handling
typedef unsigned char byte;
typedef unsigned int uint;
static uint bswap32(uint val) 
	{	return __builtin_bswap32(val); }
static uint snapUpSize(uint val)
	{	return 2 << (__builtin_clz(val-1)^31); }

// Error handling
extern const char progName[];
extern HWND errWindow;
void contError(HWND hwnd, const char* fmt, ...);
NORETURN void fatalError(const char* fmt, ...);
NORETURN void fatalError(HWND hwnd, const char* fmt, ...);


NORETURN void errorAlloc(); 
NORETURN void errorMaxPath();
NORETURN void errorDiskSpace();
NORETURN void errorDiskWrite();
NORETURN void errorDiskFail();
NORETURN void errorBadFile();
template <class T> T* errorAlloc(T* ptr)
{ if(!ptr) errorAlloc(); return ptr; }

// Memory allocation
void* xmalloc_(int size);
void* xrealloc(void* ptr, int size);
uint snapNext(uint val);
struct xmalloc
{
	xmalloc(int size) { this->size = size; }
	operator void*() { return xmalloc_(size); }
	template <class T>
	operator T*() { return (T*)xmalloc_(sizeof(T)*size); }
private:
	int size;
};
template <class T>
void xrealloc(T*& ptr, int size)
{	ptr = (T*)xrealloc((void*)ptr, size*sizeof(T)); }
template <class T>
void xNextAlloc(T*& ptr, int size)
{	if(int next = snapNext(size))
		xrealloc(ptr, next); 
}

// File handling
FILE* xfopen(const char*, const char*);
FILE* xfopen(const wchar_t*, const wchar_t*);
char* xfgets(char*, int, FILE*);
wchar_t* xfgets(wchar_t*, int, FILE*);
void xfread(void*, size_t, size_t, FILE*);
void xfwrite(const void*, size_t, size_t, FILE*);
void xchsize(FILE* fp, long size);
int fsize(FILE* fp);

template <class T> void xfread(T* ptr, size_t size, FILE* fp) {	xfread(ptr, sizeof(T), size, fp); }
template <class T> void xfwrite(const T* ptr, size_t size, FILE* fp) {	xfwrite(ptr, sizeof(T), size, fp); }
template <class T> void xfread(T& ptr, FILE* fp) {	xfread(&ptr, 1, fp);	}
template <class T> void xfwrite(const T& ptr, FILE* fp) {	xfwrite(&ptr, 1, fp);	}
template<typename T, int size> void xfread(T(& ptr)[size], FILE* fp) {	xfread(ptr, size, fp); }
template<typename T, int size> void xfwrite(T(& ptr)[size], FILE* fp) {	xfwrite(ptr, size, fp); }

// String overloads
inline FILE* fopen(const wchar_t* fName, const wchar_t* mode)
	{ return _wfopen(fName, mode); }
inline wchar_t* fgets (wchar_t* str, int num, FILE* fp)
	{ return fgetws(str, num, fp); }
inline size_t strlen (const wchar_t * str)
	{ return wcslen(str); }
inline int strcmp (const wchar_t* str1, const wchar_t* str2)
	{ return wcscmp(str1, str2); }
inline int stricmp (const wchar_t* str1, const wchar_t* str2)
	{ return wcsicmp(str1, str2); }
inline int strncmp (const wchar_t* str1, const wchar_t* str2, size_t num)
	{ return wcsncmp(str1, str2, num); }
inline int strnicmp (const wchar_t* str1, const wchar_t* str2, size_t num)
	{ return wcsnicmp(str1, str2, num); }

// String handling
char* xstrdup(const char*);
wchar_t* xstrdup(const wchar_t*);
template<typename T, int size>
int strncmp(const T* str1, const T(& str2)[size])
	{ return strncmp(str1, str2, size-1); }
template<typename T, int size>
int strnicmp(const T* str1, const T(& str2)[size])
	{ return strnicmp(str1, str2, size-1); }
int strEcmp(const char*, const char*);
int strEcmp(const wchar_t*, const wchar_t*);
int strEicmp(const char*, const char*);
int strEicmp(const wchar_t*, const wchar_t*);
int strNcpy(char*, const char*, int);
int strNcpy(wchar_t*, const wchar_t*, int);
int removeCrap(char*);
int removeCrap(wchar_t*);

// Path Handling
int getPathLen(const char*);
int getPathLen(const wchar_t*);
int getPath(char*);
int getPath(wchar_t);
char* getName(const char*);
wchar_t* getName(const wchar_t*);
int getName(char* dst, const char* src, size_t max);
int getName(wchar_t* dst, const wchar_t* src, size_t max);
bool isFullPath(const char*);
bool isFullPath(const wchar_t*);
inline bool isPathSep(int ch)
{	return (ch == '\\')||(ch == '/'); }

// MSVCRT fixes
int snprintf(char* s, size_t n, const char* fmt, ... );
int snprintf(wchar_t* s, size_t n, const wchar_t* fmt, ... );

#endif
