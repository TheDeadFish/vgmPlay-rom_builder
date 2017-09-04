#ifndef _STDSHIT_CC_
#define _STDSHIT_CC_
#include "stdshit.h"

// Basic types
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

// Error handling
void comnError(HWND hwnd, bool fatal,
	const char* fmt, va_list args)
{
	char caption[64];
	char text[2048];
	sprintf(caption, "%s: %s", progName,
		fatal ? "Fatal Error" : "Error");
	vsprintf(text, fmt, args);
	MessageBoxA(hwnd, text, caption, MB_OK);
	if(fatal) ExitProcess(1);
}

#define ERRORM(hwnd, fatal, x) {				\
	va_list args; va_start (args, fmt);		\
	comnError(hwnd, fatal, fmt, args);		\
	va_end (args); x; }
HWND errWindow = NULL;
void fatalError(const char* fmt, ...) ERRORM(errWindow, true, UNREACH)
void fatalError(HWND hwnd, const char* fmt, ...) ERRORM(hwnd, true, UNREACH)
void contError(HWND hwnd, const char* fmt, ...) ERRORM(hwnd, false, )

// Standard errors
void errorAlloc() { fatalError("Out of memory/resources"); }
void errorMaxPath() { fatalError("MAX_PATH exceeded"); }
void errorDiskSpace() { fatalError("Out of disk space"); }
void errorDiskFail() { fatalError("file IO failure"); }
void errorBadFile() { fatalError("invalid file format"); }
void errorDiskWrite() { (errno == ENOSPC) ? errorDiskSpace() : errorDiskFail(); }

// Memory allocation
void* xmalloc_(int size){
	return errorAlloc( malloc(size) ); }
void* xrealloc(void* ptr, int size){
	return errorAlloc( realloc(ptr, size) ); }
char* xstrdup(const char* str){
	return errorAlloc( strdup(str) ); }
wchar_t* xstrdup(const wchar_t* str){
	return errorAlloc( wcsdup(str) ); }
uint snapNext(uint val)
{	
	if(val & (val - 1))
		return 0;
	if( val >= 2 )
		return val<<1;
	return val + 1;
}
	
// File handling
void xfread(void* ptr, size_t size, size_t num, FILE* fp)
{
	size_t result = fread(ptr, size, num, fp);
	if(result != num)
	{
		if(ferror(fp))
			errorDiskFail();
		else
			errorBadFile();
	}
}

void xfwrite(const void* ptr, size_t size, size_t num, FILE* fp)
{
	size_t result = fwrite(ptr, size, num, fp);
	if(result != num)
		errorDiskWrite();
}

void xchsize(FILE* fp, long size)
{
	int fd = fileno(fp);
	if(_chsize(fd, size))
		errorDiskWrite();
}

int fsize(FILE* fp)
{
	int curPos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int endPos = ftell(fp);
	fseek(fp, curPos, SEEK_SET);
	return endPos-curPos;
}

// ANSI/Unicode
#define NCHAR char
#include "stdshit.cc"
#undef NCHAR	
#define NCHAR wchar_t
#define NWIDE 1
#endif
	
// File handling
FILE* xfopen(const NCHAR* fName, const NCHAR* mode)
{
	// prepare mode
	bool chkOpen = false;
	if(mode[0] == '!')
	{
		chkOpen = true;
		mode++;
	}
	
	// open file
LRETRY:
	FILE* fp = fopen(fName, mode);
	if(fp == NULL)
	{
		switch(errno)
		{
		case ENOENT:
		case EACCES:
		case EISDIR:
		case EINVAL:
			if(chkOpen)
		#ifdef NWIDE
			fatalError("Cannot open file: \"%S\"", fName);
		#else
			fatalError("Cannot open file: \"%s\"", fName);
		#endif
			break;
		
		case ENOSPC:
			errorDiskSpace();
			goto LRETRY;
		
		default:
			errorAlloc();
		}
	}
	return fp;
}

NCHAR* xfgets(NCHAR* str, int num, FILE* fp)
{
	NCHAR* tmp = fgets(str, num, fp);
	if((!tmp)&&(ferror(fp)))
		errorDiskFail();
	return tmp;
}

// String Handling
int strEcmp(const NCHAR* str1, const NCHAR* str2)
{
	int diff = strlen(str1)-strlen(str2);
	if(diff < 0)
		return 1;
	return strcmp(str1+diff, str2);
}

int strEicmp(const NCHAR* str1, const NCHAR* str2)
{
	int diff = strlen(str1)-strlen(str2);
	if(diff < 0)
		return 0;
	return stricmp(str1+diff, str2);
}

int strNcpy(NCHAR* dst, const NCHAR* src, int num)
{
	for(int i = 0; i < num; i++)
	{
		if(!(dst[i] = src[i]))
			return i;
	}
	dst[num] = '\0';
	return num;
}

int removeCrap(NCHAR* str)
{
	int len = strlen(str);
	while(len--)
		if(unsigned(str[len]) > ' ')
			break;
	str[len+1] = '\0';
	return len+1;
}

// Path Handling
int getPathLen(const NCHAR* fName)
{
	int i = strlen(fName);
	while(i--)
	{
		if((fName[i] == '\\')
		||(fName[i] == '/'))
			return i+1;
	}
	return 0;
}

int getPath(NCHAR* fName)
{
	int len = getPathLen(fName);
	fName[len] = '\0';
	return len;
}

NCHAR* getName(const NCHAR* fName)
{	return (NCHAR*)fName+getPathLen(fName); }

int getName(NCHAR* dst, const NCHAR* src, size_t max)
{
	NCHAR* name = getName(src);
	return strNcpy(dst, src, max-1);
}

bool isFullPath(const NCHAR* path)
{
	if( path
	&&((isPathSep(path[0]))
	||(path[1] == ':')))
		return true;
	return false;
}

// MSVCRT fixes
#if 0
int snprintf(NCHAR* s, size_t n, const NCHAR* fmt, ... )
{
	va_list args; va_start (args, fmt);
#ifdef NWIDE
	uint result = _vsnwprintf(s, n, fmt, args);
#else
	uint result = _vsnprintf(s, n, fmt, args);
#endif
	va_end (args);
	if(result < n)
		return result;
	if(n == 0)
		return 0;
	s[n-1] = '\0';
	return n;
}
#endif
