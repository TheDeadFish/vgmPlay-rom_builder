#ifndef _CString_H_
#define _CString_H_
#ifndef ASSERT
#include <assert.h>
#ifdef _DEBUG
#define ASSERT(f) assert(f)
#else
#define ASSERT(f)
#endif
#define VERIFY(f) ASSERT(f)
#endif
#include <windows.h>
#include <string.h>
#include <tchar.h>
#include "SciPrn.h"

// undef windows API LoadString
#ifdef LoadString
#undef LoadString
inline int WINAPI LoadString(HINSTANCE h, UINT u, LPTSTR l, int n)
#ifdef UNICODE
	{	return LoadStringW(h, u, l, n); }
#else
	{	return LoadStringA(h, u, l, n); }
#endif
#endif

// CString implmentation
struct CStringData
{
	long nRefs;            // reference count
	int nDataLength;        // length of data (including terminator)
	int nAllocLength;       // length of allocation
	// TCHAR data[nAllocLength]

	TCHAR* data()           // TCHAR* to managed data
		{ return (TCHAR*)(this+1); }
};

class CString
{
public:
// new code to deal with numeric strings
	// constructors
	CString(int intIn);
	CString(double floIn);
	// overloaded assignment
	const CString& operator=(int intIn);
	const CString& operator=(double floIn);
	// string concatenation
	const CString& operator+=(int intIn);
	const CString& operator+=(double floIn);
	friend CString operator+(const CString& string, int intIn);
	friend CString  operator+(int intIn, const CString& string);
	friend CString operator+(const CString& string, double floIn);
	friend CString  operator+(double floIn, const CString& string);
	// conversion  from string to number
	operator int() const;
	operator double() const;

// Constructors

	// constructs empty CString
	CString();
	// copy constructor
	CString(const CString& stringSrc);
	// from a single character
	CString(TCHAR ch, int nRepeat = 1);
	// from an ANSI string (converts to TCHAR)
	CString(LPCSTR lpsz);
	// from a UNICODE string (converts to TCHAR)
	CString(LPCWSTR lpsz);
	// subset of characters from an ANSI string (converts to TCHAR)
	CString(LPCSTR lpch, int nLength);
	// subset of characters from a UNICODE string (converts to TCHAR)
	CString(LPCWSTR lpch, int nLength);
	// from unsigned characters
	CString(const unsigned char* psz);

// Attributes & Operations

	// get data length
	int GetLength() const;
	// TRUE if zero length
	BOOL IsEmpty() const;
	// clear contents to empty
	void Empty();

	// return single character at zero-based index
	TCHAR GetAt(int nIndex) const;
	// return single character at zero-based index
	TCHAR operator[](int nIndex) const;
	// set a single character at zero-based index
	void SetAt(int nIndex, TCHAR ch);
	// return pointer to const string
	operator LPCTSTR() const;

	// overloaded assignment

	// ref-counted copy from another CString
	const CString& operator=(const CString& stringSrc);
	// set string content to single character
	const CString& operator=(TCHAR ch);
#ifdef _UNICODE
	const CString& operator=(char ch);
#endif
	// copy string content from ANSI string (converts to TCHAR)
	const CString& operator=(LPCSTR lpsz);
	// copy string content from UNICODE string (converts to TCHAR)
	const CString& operator=(LPCWSTR lpsz);
	// copy string content from unsigned chars
	const CString& operator=(const unsigned char* psz);

	// string concatenation

	// concatenate from another CString
	const CString& operator+=(const CString& string);

	// concatenate a single character
	const CString& operator+=(TCHAR ch);
#ifdef _UNICODE
	// concatenate an ANSI character after converting it to TCHAR
	const CString& operator+=(char ch);
#endif
	// concatenate a UNICODE character after converting it to TCHAR
	const CString& operator+=(LPCTSTR lpsz);

	friend CString  operator+(const CString& string1,
			const CString& string2);
	friend CString  operator+(const CString& string, TCHAR ch);
	friend CString  operator+(TCHAR ch, const CString& string);
#ifdef _UNICODE
	friend CString  operator+(const CString& string, char ch);
	friend CString  operator+(char ch, const CString& string);
#endif
	friend CString  operator+(const CString& string, LPCTSTR lpsz);
	friend CString  operator+(LPCTSTR lpsz, const CString& string);

	// string comparison

	// straight character comparison
	int Compare(LPCTSTR lpsz) const;
	// compare ignoring case
	int CompareNoCase(LPCTSTR lpsz) const;
	// NLS aware comparison, case sensitive
	int Collate(LPCTSTR lpsz) const;
	// NLS aware comparison, case insensitive
	int CollateNoCase(LPCTSTR lpsz) const;

	// simple sub-string extraction

	// return nCount characters starting at zero-based nFirst
	CString Mid(int nFirst, int nCount) const;
	// return all characters starting at zero-based nFirst
	CString Mid(int nFirst) const;
	// return first nCount characters in string
	CString Left(int nCount) const;
	// return nCount characters from end of string
	CString Right(int nCount) const;

	//  characters from beginning that are also in passed string
	CString SpanIncluding(LPCTSTR lpszCharSet) const;
	// characters from beginning that are not also in passed string
	CString SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion

	// NLS aware conversion to uppercase
	void MakeUpper();
	// NLS aware conversion to lowercase
	void MakeLower();
	// reverse string right-to-left
	void MakeReverse();

	// trimming whitespace (either side)

	// remove whitespace starting from right edge
	void TrimRight();
	// remove whitespace starting from left side
	void TrimLeft();

	// trimming anything (either side)

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCTSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets);

	// advanced manipulation

	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(TCHAR chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, TCHAR ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCTSTR pstr);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching

	// find character starting at left, -1 if not found
	int Find(TCHAR ch) const;
	// find character starting at right
	int ReverseFind(TCHAR ch) const;
	// find character starting at zero-based index and going right
	int Find(TCHAR ch, int nStart) const;
	// find first instance of any character in passed string
	int FindOneOf(LPCTSTR lpszCharSet) const;
	// find first instance of substring
	int Find(LPCTSTR lpszSub) const;
	// find first instance of substring starting at zero-based index
	int Find(LPCTSTR lpszSub, int nStart) const;

	// simple formatting

	// printf-like formatting using passed string
	void Format(LPCTSTR lpszFormat, ...);
	// printf-like formatting using referenced string resource
	void Format(UINT nFormatID, ...);
	// printf-like formatting using variable arguments parameter
	void FormatV(LPCTSTR lpszFormat, va_list argList);

	// formatting for localization (uses FormatMessage API)

	// format using FormatMessage API on passed string
	void FormatMessage(LPCTSTR lpszFormat, ...);
	// format using FormatMessage API on referenced string resource
	void FormatMessage(UINT nFormatID, ...);

	// load from string resource
	BOOL LoadString(UINT nID);

#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)

	// convert string from ANSI to OEM in-place
	void AnsiToOem();
	// convert string from OEM to ANSI in-place
	void OemToAnsi();
#endif
	// Access to string implementation buffer as "C" character array

	// get pointer to modifiable buffer at least as long as nMinBufLength
	LPTSTR GetBuffer(int nMinBufLength);
	// release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBuffer(int nNewLength = -1);
	// get pointer to modifiable buffer exactly as long as nNewLength
	LPTSTR GetBufferSetLength(int nNewLength);
	// release memory allocated to but unused by string
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off

	// turn refcounting back on
	LPTSTR LockBuffer();
	// turn refcounting off
	void UnlockBuffer();

// Implementation
public:
	~CString();
	int GetAllocLength() const;

protected:
	LPTSTR m_pchData;   // pointer to ref counted string data

	// implementation helpers
	CStringData* GetData() const;
	void Init();
	void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	static void PASCAL Release(CStringData* pData);
	static int PASCAL SafeStrlen(LPCTSTR lpsz);
	static void FreeData(CStringData* pData) { free(pData); }
};

// Compare helpers
bool  operator==(const CString& s1, const CString& s2);
bool  operator==(const CString& s1, LPCTSTR s2);
bool  operator==(LPCTSTR s1, const CString& s2);
bool  operator!=(const CString& s1, const CString& s2);
bool  operator!=(const CString& s1, LPCTSTR s2);
bool  operator!=(LPCTSTR s1, const CString& s2);
bool  operator<(const CString& s1, const CString& s2);
bool  operator<(const CString& s1, LPCTSTR s2);
bool  operator<(LPCTSTR s1, const CString& s2);
bool  operator>(const CString& s1, const CString& s2);
bool  operator>(const CString& s1, LPCTSTR s2);
bool  operator>(LPCTSTR s1, const CString& s2);
bool  operator<=(const CString& s1, const CString& s2);
bool  operator<=(const CString& s1, LPCTSTR s2);
bool  operator<=(LPCTSTR s1, const CString& s2);
bool  operator>=(const CString& s1, const CString& s2);
bool  operator>=(const CString& s1, LPCTSTR s2);
bool  operator>=(LPCTSTR s1, const CString& s2);

// conversion helpers
int _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count);
int _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count);

// Globals
extern LPCTSTR _afxPchNil;
#define afxEmptyString ((CString&)*(CString*)&_afxPchNil)

// new numder processing inline methods
inline CString::CString(int intIn)
	{ Init(); *this = intIn; }
inline CString::CString(double floIn)
	{ Init(); *this = floIn; }
inline const CString& CString::operator=(int intIn)
	{ TCHAR tmp[26]; _itot(intIn, tmp, 10); *this = tmp; return *this; }
inline const CString& CString::operator=(double floIn)
	{ *this = SciPrn(floIn); return *this; }
inline const CString& CString::operator+=(int intIn)
	{ TCHAR tmp[26]; _itot(intIn, tmp, 10); *this += tmp; return *this;}
inline const CString& CString::operator+=(double floIn)
	{ *this += SciPrn(floIn); return *this; }
inline CString::operator int() const
	{ return _ttoi(m_pchData); }
inline CString::operator double() const
	{ return atof(m_pchData); }

// CSting inline methods
inline CStringData* CString::GetData() const
	{ ASSERT(m_pchData != NULL); return ((CStringData*)m_pchData)-1; }
inline void CString::Init()
	{ m_pchData = afxEmptyString.m_pchData; }
inline CString::CString()
	{ m_pchData = afxEmptyString.m_pchData; }
inline CString::CString(const unsigned char* lpsz)
	{ Init(); *this = (LPCSTR)lpsz; }
inline const CString& CString::operator=(const unsigned char* lpsz)
	{ *this = (LPCSTR)lpsz; return *this; }
#ifdef _UNICODE
inline const CString& CString::operator+=(char ch)
	{ *this += (TCHAR)ch; return *this; }
inline const CString& CString::operator=(char ch)
	{ *this = (TCHAR)ch; return *this; }
inline CString  operator+(const CString& string, char ch)
	{ return string + (TCHAR)ch; }
inline CString  operator+(char ch, const CString& string)
	{ return (TCHAR)ch + string; }
#endif

inline int CString::GetLength() const
	{ return GetData()->nDataLength; }
inline int CString::GetAllocLength() const
	{ return GetData()->nAllocLength; }
inline BOOL CString::IsEmpty() const
	{ return GetData()->nDataLength == 0; }
inline CString::operator LPCTSTR() const
	{ return m_pchData; }
inline int PASCAL CString::SafeStrlen(LPCTSTR lpsz)
	{ return (lpsz == NULL) ? 0 : lstrlen(lpsz); }

// CString support (windows specific)
inline int CString::Compare(LPCTSTR lpsz) const
	{ ASSERT(lpsz != NULL); return _tcscmp(m_pchData, lpsz); }    // MBCS/Unicode aware
inline int CString::CompareNoCase(LPCTSTR lpsz) const
	{ ASSERT(lpsz != NULL); return _tcsicmp(m_pchData, lpsz); }   // MBCS/Unicode aware
// CString::Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
inline int CString::Collate(LPCTSTR lpsz) const
	{ ASSERT(lpsz != NULL); return _tcscoll(m_pchData, lpsz); }   // locale sensitive
inline int CString::CollateNoCase(LPCTSTR lpsz) const
	{ ASSERT(lpsz != NULL); return _tcsicoll(m_pchData, lpsz); }   // locale sensitive

inline TCHAR CString::GetAt(int nIndex) const
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}
inline TCHAR CString::operator[](int nIndex) const
{
	// same as GetAt
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetData()->nDataLength);
	return m_pchData[nIndex];
}
inline bool  operator==(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) == 0; }
inline bool  operator==(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) == 0; }
inline bool  operator==(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) == 0; }
inline bool  operator!=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) != 0; }
inline bool  operator!=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) != 0; }
inline bool  operator!=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) != 0; }
inline bool  operator<(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) < 0; }
inline bool  operator<(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) < 0; }
inline bool  operator<(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) > 0; }
inline bool  operator>(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) > 0; }
inline bool  operator>(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) > 0; }
inline bool  operator>(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) < 0; }
inline bool  operator<=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) <= 0; }
inline bool  operator<=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) <= 0; }
inline bool  operator<=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) >= 0; }
inline bool  operator>=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) >= 0; }
inline bool  operator>=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) >= 0; }
inline bool  operator>=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) <= 0; }

#endif
