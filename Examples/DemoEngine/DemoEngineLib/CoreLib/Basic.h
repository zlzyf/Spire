﻿/***********************************************************************

CoreLib - The MIT License (MIT)
Copyright (c) 2016, Yong He

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

***********************************************************************/

/***********************************************************************
WARNING: This is an automatically generated file.
***********************************************************************/

/***********************************************************************
COMMON.H
***********************************************************************/
#ifndef CORE_LIB_COMMON_H
#define CORE_LIB_COMMON_H

#include <cstdint>

#ifdef __GNUC__
#define CORE_LIB_ALIGN_16(x) x __attribute__((aligned(16)))
#else
#define CORE_LIB_ALIGN_16(x) __declspec(align(16)) x
#endif

#define VARIADIC_TEMPLATE

namespace CoreLib
{
	typedef int64_t Int64;
	typedef unsigned short Word;
#ifdef _M_X64
	typedef int64_t PtrInt;
#else
	typedef int PtrInt;
#endif
	namespace Basic
	{
		class Object
		{
		public:
			virtual ~Object()
			{}
		};

		template <typename T>
		inline T&& _Move(T & obj)
		{
			return static_cast<T&&>(obj);
		}

		template <typename T>
		inline void Swap(T & v0, T & v1)
		{
			T tmp = _Move(v0);
			v0 = _Move(v1);
			v1 = _Move(tmp);
		}
	}
}

#endif

/***********************************************************************
WIDECHAR.H
***********************************************************************/
#ifndef WIDE_CHAR_H
#define WIDE_CHAR_H

void MByteToWideChar(wchar_t * buffer, int bufferSize, const char * str, int length);
char * WideCharToMByte(const wchar_t * buffer, int length);
wchar_t * MByteToWideChar(const char * buffer, int length);

#endif

/***********************************************************************
SMARTPOINTER.H
***********************************************************************/
#ifndef FUNDAMENTAL_LIB_SMART_POINTER_H
#define FUNDAMENTAL_LIB_SMART_POINTER_H

namespace CoreLib
{
	namespace Basic
	{
		class RefPtrDefaultDestructor
		{
		public:
			template<typename T>
			void operator ()(T * ptr)
			{
				delete ptr;
			}
		};

		class RefPtrArrayDestructor
		{
		public:
			template<typename T>
			void operator() (T * ptr)
			{
				delete [] ptr;
			}
		};

		template<typename T, typename Destructor = RefPtrDefaultDestructor>
		class RefPtr
		{
			template<typename T1, typename Destructor1>
			friend class RefPtr;
		private:
			T * pointer;
			int * refCount;
			
		public:
			RefPtr()
			{
				pointer = 0;
				refCount = 0;
			}
			RefPtr(T * ptr)
				: pointer(0), refCount(0)
			{
				this->operator=(ptr);
			}
			template<typename T1>
			RefPtr(T1 * ptr)
				: pointer(0), refCount(0)
			{
				this->operator=(ptr);
			}
			RefPtr(const RefPtr<T, Destructor> & ptr)
				: pointer(0), refCount(0)
			{
				this->operator=(ptr);
			}
			RefPtr(RefPtr<T, Destructor> && str)
				: pointer(0), refCount(0)
			{
				this->operator=(static_cast<RefPtr<T, Destructor> &&>(str));
			}
			RefPtr<T,Destructor>& operator=(T * ptr)
			{
				Dereferance();

				pointer = ptr;
				if(ptr)
				{
					refCount = new int;
					(*refCount) = 1;
				}
				else
					refCount = 0;
				return *this;
			}
			template<typename T1>
			RefPtr<T,Destructor>& operator=(T1 * ptr)
			{
				Dereferance();

				pointer = dynamic_cast<T*>(ptr);
				if(ptr)
				{
					refCount = new int;
					(*refCount) = 1;
				}
				else
					refCount = 0;
				return *this;
			}
			RefPtr<T,Destructor>& operator=(const RefPtr<T, Destructor> & ptr)
			{
				if(ptr.pointer != pointer)
				{
					Dereferance();
					pointer = ptr.pointer;
					refCount = ptr.refCount;
					if (refCount)
						(*refCount)++;
				}
				return *this;
			}

			template<typename T1>
			RefPtr(const RefPtr<T1> & ptr)
				: pointer(0), refCount(0)
			{
				this->operator=(ptr);
			}
			template<typename T1>
			RefPtr<T,Destructor> & operator = (const RefPtr<T1, Destructor> & ptr)
			{
				if(ptr.pointer != pointer)
				{
					Dereferance();
					pointer = dynamic_cast<T*>(ptr.pointer);
					if (ptr.pointer && !pointer)
						throw L"RefPtr assignment: type cast failed.";
					refCount = ptr.refCount;
					(*refCount)++;
				}
				return *this;
			}
			bool operator == (const T * ptr) const
			{
				return pointer == ptr;
			}
			bool operator != (const T * ptr) const
			{
				return pointer != ptr;
			}
			bool operator == (const RefPtr<T, Destructor> & ptr) const
			{
				return pointer == ptr.pointer;
			}
			bool operator != (const RefPtr<T, Destructor> & ptr) const
			{
				return pointer != ptr.pointer;
			}

			T* operator +(int offset) const
			{
				return pointer+offset;
			}
			T& operator [](int idx) const
			{
				return *(pointer + idx);
			}
			RefPtr<T,Destructor>& operator=(RefPtr<T, Destructor> && ptr)
			{
				if(ptr.pointer != pointer)
				{
					Dereferance();
					pointer = ptr.pointer;
					refCount = ptr.refCount;
					ptr.pointer = 0;
					ptr.refCount = 0;
				}
				return *this;
			}
			T* Release()
			{
				if(pointer)
				{
					if((*refCount) > 1)
					{
						(*refCount)--;
					}
					else
					{
						delete refCount;
					}
				}
				auto rs = pointer;
				refCount = 0;
				pointer = 0;
				return rs;
			}
			~RefPtr()
			{
				Dereferance();
			}

			void Dereferance()
			{
				if(pointer)
				{
					if((*refCount) > 1)
					{
						(*refCount)--;
					}
					else
					{
						Destructor destructor;
						destructor(pointer);
						delete refCount;
					}
				}
			}
			T & operator *() const
			{
				return *pointer;
			}
			T * operator->() const
			{
				return pointer;
			}
			T * Ptr() const
			{
				return pointer;
			}
		public:
			explicit operator bool() const 
			{
				if (pointer)
					return true;
				else
					return false;
			}
		};
	}
}

#endif

/***********************************************************************
SECURECRT.H
***********************************************************************/
#ifndef _MSC_VER
#ifndef CORE_LIB_SECURE_CRT_H
#define CORE_LIB_SECURE_CRT_H
#include <stdarg.h>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#ifndef _ismbblead

inline bool _ismbblead(char c)
{
	return (0x80<= c) && (c < 0xa0 || 0xe0 <= c);
}

#endif

inline void memcpy_s(void *dest, size_t numberOfElements, const void * src, size_t count)
{
	memcpy(dest, src, count);
}

#define _TRUNCATE ((size_t)-1)
#define _stricmp strcasecmp

inline void fopen_s(FILE**f, const char * fileName, const char * mode)
{
	*f = fopen(fileName, mode);
}

inline void wcstombs_s(size_t * pReturnValue, char *mbstr, size_t sizeInWords, const wchar_t *wcstr, size_t count)
{
	if (count == _TRUNCATE)
		count = sizeInWords;
	*pReturnValue = wcstombs(mbstr, wcstr, count);
}

inline void mbstowcs_s(size_t * pReturnValue, wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count)
{
	if (count == _TRUNCATE)
		count = sizeInWords;
	*pReturnValue = mbstowcs(wcstr, mbstr, count);
}

inline size_t fread_s(void * buffer, size_t bufferSize, size_t elementSize, size_t count, FILE * stream)
{
	return fread(buffer, elementSize, count, stream);
}

inline int _itow_s(int value, wchar_t * buffer, size_t sizeInCharacters, int radix)
{
	std::wstringstream s;
	s<<value;
	auto str = s.str();
	memset(buffer, 0, sizeInCharacters * sizeof(wchar_t));
	memcpy(buffer, str.c_str(), str.length() * sizeof(wchar_t));
	return 0;
}

inline int _i64tow_s(long long value, wchar_t * buffer, size_t sizeInCharacters, int radix)
{
	std::wstringstream s;
	s<<value;
	auto str = s.str();
	memset(buffer, 0, sizeInCharacters * sizeof(wchar_t));
	memcpy(buffer, str.c_str(), str.length() * sizeof(wchar_t));
	return 0;
}

inline size_t wcsnlen_s(const wchar_t * str, size_t numberofElements)
{
	return wcsnlen(str, numberofElements);
}

inline size_t strnlen_s(const char * str, size_t numberofElements)
{
	return strnlen(str, numberofElements);
}

inline int swprintf_s(wchar_t * buffer, size_t sizeOfBuffer, const wchar_t * format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	int rs = swprintf(buffer, sizeOfBuffer, format, argptr);
	va_end(argptr);
	return rs;
}

inline void wcscpy_s(wchar_t * strDestination, size_t numberOfElements, const wchar_t * strSource)
{
	wcscpy(strDestination, strSource);
}

inline void wcsncpy_s(wchar_t * strDestination, size_t numberOfElements, const wchar_t * strSource, size_t count)
{
	wcsncpy(strDestination, strSource, count);
	//wcsncpy(strDestination, strSource, count);
}

#endif
#endif

/***********************************************************************
LIBSTRING.H
***********************************************************************/
#ifndef FUNDAMENTAL_LIB_STRING_H
#define FUNDAMENTAL_LIB_STRING_H
#include <string.h>
#include <cstdlib>
#include <stdio.h>

namespace CoreLib
{
	namespace Basic
	{
		class _EndLine
		{};
		extern _EndLine EndLine;
		class String
		{
			friend class StringBuilder;
		private:
			RefPtr<wchar_t, RefPtrArrayDestructor> buffer;
			char * multiByteBuffer;
			int length;
			void Free()
			{
				if (buffer)
					buffer = 0;
				if (multiByteBuffer)
					delete [] multiByteBuffer;
				buffer = 0;
				multiByteBuffer = 0;
				length = 0;
			}
		public:
			static String FromBuffer(RefPtr<wchar_t, RefPtrArrayDestructor> buffer, int len)
			{
				String rs;
				rs.buffer = buffer;
				rs.length = len;
				return rs;
			}
			String()
				:buffer(0), multiByteBuffer(0), length(0)
			{
			}
			String(const wchar_t * str) :buffer(0), multiByteBuffer(0), length(0)
			{
				this->operator=(str);
			}
			String(const wchar_t ch)
				:buffer(0), multiByteBuffer(0), length(0)
			{
				wchar_t arr[] = {ch, 0};
				*this = String(arr);
			}
			const wchar_t * begin() const
			{
				return buffer.Ptr();
			}
			const wchar_t * end() const
			{
				return buffer.Ptr() + length;
			}
			String(int val, int radix = 10)
				:buffer(0), multiByteBuffer(0), length(0)
			{
				buffer = new wchar_t[33];
				_itow_s(val, buffer.Ptr(), 33, radix);
				length = (int)wcsnlen_s(buffer.Ptr(), 33);
			}
			String(long long val, int radix = 10)
				:buffer(0), multiByteBuffer(0), length(0)
			{
				buffer = new wchar_t[65];
				_i64tow_s(val, buffer.Ptr(), 65, radix);
				length = (int)wcsnlen_s(buffer.Ptr(), 65);
			}
			String(float val, const wchar_t * format = L"%g")
				:buffer(0), multiByteBuffer(0), length(0)
			{
				buffer = new wchar_t[128];
				swprintf_s(buffer.Ptr(), 128, format, val);
				length = (int)wcsnlen_s(buffer.Ptr(), 128);
			}
			String(double val, const wchar_t * format = L"%g")
				:buffer(0), multiByteBuffer(0), length(0)
			{
				buffer = new wchar_t[128];
				swprintf_s(buffer.Ptr(), 128, format, val);
				length = (int)wcsnlen_s(buffer.Ptr(), 128);
			}
			String(const char * str)
				:buffer(0), multiByteBuffer(0), length(0)
			{
				if (str)
				{
					buffer = MByteToWideChar(str, (int)strlen(str));
					if (buffer)
						length = (int)wcslen(buffer.Ptr());
					else
						length = 0;
				}
			}
			String(const String & str)
				:buffer(0), multiByteBuffer(0), length(0)
			{				
				this->operator=(str);
			}
			String(String&& other)
				:buffer(0), multiByteBuffer(0), length(0)
			{
				this->operator=(static_cast<String&&>(other));
			}
			~String()
			{
				Free();
			}
			String & operator=(const wchar_t * str)
			{
				Free();
				if (str)
				{
					length = (int)wcslen(str);
					buffer = new wchar_t[length + 1];
					wcscpy_s(buffer.Ptr(), length + 1, str);
				}
				return *this;
			}
			String & operator=(const String & str)
			{
				if (str.buffer == buffer)
					return *this;
				Free();
				if (str.buffer)
				{
					length = str.length;
					buffer = str.buffer;
					multiByteBuffer = 0;
				}
				return *this;
			}
			String & operator=(String&& other)
			{
				if (this != &other)
				{
					Free();
					buffer = _Move(other.buffer);
					length = other.length;
					multiByteBuffer = other.multiByteBuffer;
					other.buffer = 0;
					other.length = 0;
					other.multiByteBuffer = 0;
				}
				return *this;
			}
			wchar_t operator[](int id) const
			{
#if _DEBUG
				if (id < 0 || id >= length)
					throw "Operator[]: index out of range.";
#endif
				return buffer.Ptr()[id];
			}

			friend String StringConcat(const wchar_t * lhs, int leftLen, const wchar_t * rhs, int rightLen);
			friend String operator+(const wchar_t*op1, const String & op2);
			friend String operator+(const String & op1, const wchar_t * op2);
			friend String operator+(const String & op1, const String & op2);

			String TrimStart() const
			{
				if(!buffer)
					return *this;
				int startIndex = 0;
				while (startIndex < length && 
					(buffer[startIndex] == L' ' || buffer[startIndex] == L'\t' || buffer[startIndex] == L'\r' || buffer[startIndex] == L'\n'))
						startIndex++;
				return String(buffer + startIndex);
			}

			String TrimEnd() const
			{
				if(!buffer)
					return *this;

				int endIndex = length - 1;
				while (endIndex >= 0 &&
					(buffer[endIndex] == L' ' || buffer[endIndex] == L'\t' || buffer[endIndex] == L'\r' || buffer[endIndex] == L'\n'))
					endIndex--;
				String res;
				res.length = endIndex + 1;
				res.buffer = new wchar_t[endIndex + 2];
				wcsncpy_s(res.buffer.Ptr(), endIndex + 2, buffer.Ptr(), endIndex + 1);
				return res;
			}

			String Trim() const
			{
				if(!buffer)
					return *this;

				int startIndex = 0;
				while (startIndex < length && 
					(buffer[startIndex] == L' ' || buffer[startIndex] == L'\t'))
						startIndex++;
				int endIndex = length - 1;
				while (endIndex >= startIndex &&
					(buffer[endIndex] == L' ' || buffer[endIndex] == L'\t'))
					endIndex--;

				String res;
				res.length = endIndex - startIndex + 1;
				res.buffer = new wchar_t[res.length + 1];
				memcpy(res.buffer.Ptr(), buffer + startIndex, sizeof(wchar_t) * res.length);
				res.buffer[res.length] = L'\0';
				return res;
			}

			String SubString(int id, int len) const
			{
				if (len == 0)
					return L"";
				if (id + len > length)
					len = length - id;
#if _DEBUG
				if (id < 0 || id >= length || (id + len) > length)
					throw "SubString: index out of range.";
				if (len < 0)
					throw "SubString: length less than zero.";
#endif
				String res;
				res.buffer = new wchar_t[len + 1];
				res.length = len;
				wcsncpy_s(res.buffer.Ptr(), len + 1, buffer + id, len);
				res.buffer[len] = 0;
				return res;
			}

			wchar_t * Buffer() const
			{
				if (buffer)
					return buffer.Ptr();
				else
					return (wchar_t*)L"";
			}

			char * ToMultiByteString(int * len = 0) const
			{
				if (!buffer)
					return (char*)"";
				else
				{
					if (multiByteBuffer)
						return multiByteBuffer;
					((String*)this)->multiByteBuffer = WideCharToMByte(buffer.Ptr(), length);
					if (len)
						*len = (int)strnlen_s(multiByteBuffer, length*2);
					return multiByteBuffer;
					/*if (multiByteBuffer)
						return multiByteBuffer;
					size_t requiredBufferSize;
					requiredBufferSize = WideCharToMultiByte(CP_OEMCP, NULL, buffer.Ptr(), length, 0, 0, NULL, NULL)+1;
					if (len)
						*len = requiredBufferSize-1;
					if (requiredBufferSize)
					{
						multiByteBuffer = new char[requiredBufferSize];
						WideCharToMultiByte(CP_OEMCP, NULL, buffer.Ptr(), length, multiByteBuffer, requiredBufferSize, NULL, NULL);
						multiByteBuffer[requiredBufferSize-1] = 0;
						return multiByteBuffer;
					}
					else
						return "";*/
				}
			}

			bool Equals(const String & str, bool caseSensitive = true)
			{
				if (!buffer)
					return (str.buffer == 0);
				if (caseSensitive)
					return (wcscmp(buffer.Ptr(), str.buffer.Ptr()) == 0);
				else
				{
#ifdef _MSC_VER
					return (_wcsicmp(buffer.Ptr(), str.buffer.Ptr()) == 0);
#else
					return (wcscasecmp(buffer.Ptr(), str.buffer.Ptr()) == 0);
#endif
				}
			}

			bool operator==(const String & str) const
			{
				if (!buffer)
					return (str.buffer == 0 || wcscmp(str.buffer.Ptr(), L"")==0);
				if (!str.buffer)
					return buffer == nullptr || wcscmp(buffer.Ptr(), L"") == 0;
				return (wcscmp(buffer.Ptr(), str.buffer.Ptr()) == 0);
			}
			bool operator!=(const String & str) const
			{
				if (!buffer)
					return (str.buffer != 0 && wcscmp(str.buffer.Ptr(), L"") != 0);
				if (str.buffer.Ptr() == 0)
					return length != 0;
				return (wcscmp(buffer.Ptr(), str.buffer.Ptr()) != 0);
			}
			bool operator>(const String & str) const
			{
				if (!buffer)
					return false;
				if (!str.buffer)
					return buffer.Ptr() != nullptr && length != 0;
				return (wcscmp(buffer.Ptr(), str.buffer.Ptr()) > 0);
			}
			bool operator<(const String & str) const
			{
				if (!buffer)
					return (str.buffer != 0);
				if (!str.buffer)
					return false;
				return (wcscmp(buffer.Ptr(), str.buffer.Ptr()) < 0);
			}
			bool operator>=(const String & str) const
			{
				if (!buffer)
					return (str.buffer == 0);
				if (!str.buffer)
					return length == 0;
				int res = wcscmp(buffer.Ptr(), str.buffer.Ptr());
				return (res > 0 || res == 0);
			}
			bool operator<=(const String & str) const
			{
				if (!buffer)
					return true;
				if (!str.buffer)
					return length > 0;
				int res = wcscmp(buffer.Ptr(), str.buffer.Ptr());
				return (res < 0 || res == 0);
			}

			String ToUpper() const
			{
				if(!buffer)
					return *this;
				String res;
				res.length = length;
				res.buffer = new wchar_t[length + 1];
				for (int i = 0; i <= length; i++)
					res.buffer[i] = (buffer[i] >= L'a' && buffer[i] <= L'z')? 
									(buffer[i] - L'a' + L'A') : buffer[i];
				return res;
			}

			String ToLower() const
			{
				if(!buffer)
					return *this;
				String res;
				res.length = length;
				res.buffer = new wchar_t[length + 1];
				for (int i = 0; i <= length; i++)
					res.buffer[i] = (buffer[i] >= L'A' && buffer[i] <= L'Z')? 
									(buffer[i] - L'A' + L'a') : buffer[i];
				return res;
			}
			
			int Length() const
			{
				return length;
			}

			int IndexOf(const wchar_t * str, int id) const // String str
			{
				if(!buffer)
					return -1;
				if (id < 0 || id >= length)
					return -1;
				auto findRs = wcsstr(buffer + id, str);
				int res = findRs ? (int)(findRs - buffer.Ptr()) : -1;
				if (res >= 0)
					return res;
				else
					 return -1;
			}
			
			int IndexOf(const String & str, int id) const
			{
				return IndexOf(str.buffer.Ptr(), id);
			}

			int IndexOf(const wchar_t * str) const
			{
				return IndexOf(str, 0);
			}

			int IndexOf(const String & str) const
			{
				return IndexOf(str.buffer.Ptr(), 0);
			}

			int IndexOf(wchar_t ch, int id) const
			{
#if _DEBUG
				if (id < 0 || id >= length)
					throw "SubString: index out of range.";
#endif
				if(!buffer)
					return -1;
				for (int i = id; i < length; i++)
					if (buffer[i] == ch)
						return i;
				return -1;
			}

			int IndexOf(wchar_t ch) const
			{
				return IndexOf(ch, 0);
			}

			int LastIndexOf(wchar_t ch) const
			{
				for (int i = length-1; i>=0; i--)
					if (buffer[i] == ch)
						return i;
				return -1;
			}

			bool StartsWith(const wchar_t * str) const // String str
			{
				if(!buffer)
					return false;
				int strLen =(int) wcslen(str);
				if (strLen > length)
					return false;
				for (int i = 0; i < strLen; i++)
					if (str[i] != buffer[i])
						return false;
				return true;
			}

			bool StartsWith(const String & str) const
			{
				return StartsWith((const wchar_t*)str.buffer.Ptr());
			}

			bool EndsWith(wchar_t * str)  const // String str
			{
				if(!buffer)
					return false;
				int strLen = (int)wcslen(str);
				if (strLen > length)
					return false;
				for (int i = strLen - 1; i >= 0; i--)
					if (str[i] != buffer[length - strLen + i])
						return false;
				return true;
			}

			bool EndsWith(const String & str) const
			{
				return EndsWith(str.buffer.Ptr());
			}

			bool Contains(const wchar_t * str) const // String str
			{
				if(!buffer)
					return false;
				return (IndexOf(str) >= 0)? true : false;
			}

			bool Contains(const String & str) const
			{
				return Contains(str.buffer.Ptr());
			}

			int GetHashCode() const
			{
				if (!buffer)
					return 0;
				int hash = 0;
				int c;
				wchar_t * str = buffer.Ptr();
				c = *str++;
				while (c)
				{
					hash = c + (hash << 6) + (hash << 16) - hash;
					c = *str++;
				}
				return hash;
			}
			String PadLeft(wchar_t ch, int length);
			String PadRight(wchar_t ch, int length);
			String MD5() const;
		};

		class StringBuilder
		{
		private:
			wchar_t * buffer;
			int length;
			int bufferSize;
			static const int InitialSize = 512;
		public:
			StringBuilder(int bufferSize = 1024)
				:buffer(0), length(0), bufferSize(0)
			{
				buffer = new wchar_t[InitialSize]; // new a larger buffer 
				buffer[0] = L'\0';
				length = 0;
				bufferSize = InitialSize;
			}
			~StringBuilder()
			{
				if(buffer)
					delete [] buffer;
			}
			void EnsureCapacity(int size)
			{
				if(bufferSize < size)
				{
					wchar_t * newBuffer = new wchar_t[size + 1];
					if(buffer)
					{
						wcscpy_s(newBuffer, size + 1, buffer);
						delete [] buffer;
					}
					buffer = newBuffer;
					bufferSize = size;
				}
			}

			//void Append(wchar_t * str)
			//{
			//	length += wcslen(str);
			//	if(bufferSize < length + 1)
			//	{
			//		int newBufferSize = InitialSize;
			//		while(newBufferSize < length + 1)
			//			newBufferSize <<= 1;
			//		wchar_t * newBuffer = new wchar_t[newBufferSize];
			//		if (buffer)
			//		{
			//			wcscpy_s(newBuffer, newBufferSize, buffer);
			//			delete [] buffer;
			//		}
			//		wcscat_s(newBuffer, newBufferSize, str); // use memcpy, manually deal with zero terminator
			//		buffer = newBuffer;
			//		bufferSize = newBufferSize;
			//	}
			//	else
			//	{
			//		wcscat_s(buffer, bufferSize, str); // use memcpy, manually deal with zero terminator
			//	}
			//}
			StringBuilder & operator << (const wchar_t * str)
			{
				Append(str, (int)wcslen(str));
				return *this;
			}
			StringBuilder & operator << (const String & str)
			{
				Append(str);
				return *this;
			}
			StringBuilder & operator << (const _EndLine)
			{
				Append(L'\n');
				return *this;
			}
			void Append(wchar_t ch)
			{
				Append(&ch, 1);
			}
			void Append(int value, int radix = 10)
			{
				wchar_t vBuffer[33];
				_itow_s(value, vBuffer, 33, radix);
				Append(vBuffer);
			}
			void Append(const String & str)
			{
				Append(str.Buffer(), str.Length());
			}
			void Append(const wchar_t * str)
			{
				Append(str, (int)wcslen(str));
			}
			void Append(const wchar_t * str, int strLen)
			{
				int newLength = length + strLen;
				if(bufferSize < newLength + 1)
				{
					int newBufferSize = InitialSize;
					while(newBufferSize < newLength + 1)
						newBufferSize <<= 1;
					wchar_t * newBuffer = new wchar_t[newBufferSize];
					if (buffer)
					{
						//wcscpy_s(newBuffer, newBufferSize, buffer);
						memcpy(newBuffer, buffer, sizeof(wchar_t) * length);
						delete [] buffer;
					}
					//wcscat_s(newBuffer, newBufferSize, str);
					memcpy(newBuffer + length, str, sizeof(wchar_t) * strLen);
					newBuffer[newLength] = L'\0';
					buffer = newBuffer;
					bufferSize = newBufferSize;
				}
				else
				{
					memcpy(buffer + length, str, sizeof(wchar_t) * strLen);
					buffer[newLength] = L'\0';
					//wcscat_s(buffer, bufferSize, str); // use memcpy, manually deal with zero terminator
				}
				length = newLength;
			}

			int Capacity()
			{
				return bufferSize;
			}

			wchar_t * Buffer()
			{
				return buffer;
			}

			int Length()
			{
				return length;
			}

			String ToString()
			{
				return String(buffer);
			}

			String ProduceString()
			{
				String rs;
				rs.buffer = buffer;
				rs.length = length;
				buffer = 0;
				length = 0;
				return rs;

			}

			String GetSubString(int start, int count)
			{
				String rs;
				rs.buffer = new wchar_t[count+1];
				rs.length = count;
				wcsncpy_s(rs.buffer.Ptr(), count+1, buffer+start, count);
				rs.buffer[count] = 0;
				return rs;
			}

			void Remove(int id, int len)
			{
#if _DEBUG
				if (id >= length || id < 0)
					throw "Remove: Index out of range.";
				if(len < 0)
					throw "Remove: remove length smaller than zero.";
#endif
				int actualDelLength = ((id + len) >= length)? (length - id) : len;
				for (int i = id + actualDelLength; i <= length; i++)
					buffer[i - actualDelLength] = buffer[i];
				length -= actualDelLength;
			}

			void Clear()
			{
				length = 0;
				if (buffer)
					buffer[0] = 0;
			}
		};

		int StringToInt(const String & str);
		double StringToDouble(const String & str);

		
	}
}

#endif

/***********************************************************************
EXCEPTION.H
***********************************************************************/
#ifndef CORE_LIB_EXCEPTION_H
#define CORE_LIB_EXCEPTION_H


namespace CoreLib
{
	namespace Basic
	{
		class Exception : public Object
		{
		public:
			String Message;
			Exception()
			{}
			Exception(const String & message)
				: Message(message)
			{
			}
		};

		class IndexOutofRangeException : public Exception
		{
		public:
			IndexOutofRangeException()
			{}
			IndexOutofRangeException(const String & message)
				: Exception(message)
			{
			}

		};

		class InvalidOperationException : public Exception
		{
		public:
			InvalidOperationException()
			{}
			InvalidOperationException(const String & message)
				: Exception(message)
			{
			}

		};
		
		class ArgumentException : public Exception
		{
		public:
			ArgumentException()
			{}
			ArgumentException(const String & message)
				: Exception(message)
			{
			}

		};

		class KeyNotFoundException : public Exception
		{
		public:
			KeyNotFoundException()
			{}
			KeyNotFoundException(const String & message)
				: Exception(message)
			{
			}
		};
		class KeyExistsException : public Exception
		{
		public:
			KeyExistsException()
			{}
			KeyExistsException(const String & message)
				: Exception(message)
			{
			}
		};

		class NotSupportedException : public Exception
		{
		public:
			NotSupportedException()
			{}
			NotSupportedException(const String & message)
				: Exception(message)
			{
			}
		};

		class NotImplementedException : public Exception
		{
		public:
			NotImplementedException()
			{}
			NotImplementedException(const String & message)
				: Exception(message)
			{
			}
		};

		class InvalidProgramException : public Exception
		{
		public:
			InvalidProgramException()
			{}
			InvalidProgramException(const String & message)
				: Exception(message)
			{
			}
		};
	}
}

#endif

/***********************************************************************
ARRAYVIEW.H
***********************************************************************/
#ifndef CORE_LIB_ARRAY_VIEW_H
#define CORE_LIB_ARRAY_VIEW_H


namespace CoreLib
{
	namespace Basic
	{
		template<typename T>
		class ArrayView
		{
		private:
			T * _buffer;
			int _count;
			int stride;
		public:
			T* begin() const
			{
				return _buffer;
			}
			T* end() const
			{
				return (T*)((char*)_buffer + _count*stride);
			}
		public:
			ArrayView()
			{
				_buffer = 0;
				_count = 0;
			}
			ArrayView(const T & singleObj)
			{
				SetData((T*)&singleObj, 1, sizeof(T));
			}
			ArrayView(T * buffer, int count)
			{
				SetData(buffer, count, sizeof(T));
			}
			ArrayView(void * buffer, int count, int _stride)
			{
				SetData(buffer, count, _stride);
			}
			void SetData(void * buffer, int count, int _stride)
			{
				this->_buffer = (T*)buffer;
				this->_count = count;
				this->stride = _stride;
			}
			inline int GetCapacity() const
			{
				return _count;
			}
			inline int Count() const
			{
				return _count;
			}

			inline T & operator [](int id) const
			{
#if _DEBUG
				if (id >= _count || id < 0)
					throw IndexOutofRangeException(L"Operator[]: Index out of Range.");
#endif
				return *(T*)((char*)_buffer+id*stride);
			}

			inline T* Buffer() const
			{
				return _buffer;
			}

			template<typename T2>
			int IndexOf(const T2 & val) const
			{
				for (int i = 0; i < _count; i++)
				{
					if (*(T*)((char*)_buffer + i*stride) == val)
						return i;
				}
				return -1;
			}

			template<typename T2>
			int LastIndexOf(const T2 & val) const
			{
				for (int i = _count - 1; i >= 0; i--)
				{
					if (*(T*)((char*)_buffer + i*stride) == val)
						return i;
				}
				return -1;
			}

			template<typename Func>
			int FindFirst(const Func & predicate) const
			{
				for (int i = 0; i < _count; i++)
				{
					if (predicate(_buffer[i]))
						return i;
				}
				return -1;
			}

			template<typename Func>
			int FindLast(const Func & predicate) const
			{
				for (int i = _count - 1; i >= 0; i--)
				{
					if (predicate(_buffer[i]))
						return i;
				}
				return -1;
			}
		};

		template<typename T>
		ArrayView<T> MakeArrayView(const T & obj)
		{
			return ArrayView<T>(obj);
		}
		template<typename T>
		ArrayView<T> MakeArrayView(T * buffer, int count)
		{
			return ArrayView<T>(buffer, count);
		}
	}
}
#endif

/***********************************************************************
LIBMATH.H
***********************************************************************/
#ifndef CORE_LIB_MATH_H
#define CORE_LIB_MATH_H

#include <math.h>

namespace CoreLib
{
	namespace Basic
	{
		class Math
		{
		public:
			static const float Pi;
			template<typename T>
			static T Min(const T& v1, const T&v2)
			{
				return v1<v2?v1:v2;
			}
			template<typename T>
			static T Max(const T& v1, const T&v2)
			{
				return v1>v2?v1:v2;
			}
			template<typename T>
			static T Min(const T& v1, const T&v2, const T&v3)
			{
				return Min(v1, Min(v2, v3));
			}
			template<typename T>
			static T Max(const T& v1, const T&v2, const T&v3)
			{
				return Max(v1, Max(v2, v3));
			}
			template<typename T>
			static T Clamp(const T& val, const T& vmin, const T&vmax)
			{
				if (val < vmin) return vmin;
				else if (val > vmax) return vmax;
				else return val;
			}

			static inline int FastFloor(float x)
			{
				int i = (int)x;
				return i - (i > x);
			}

			static inline int FastFloor(double x)
			{
				int i = (int)x;
				return i - (i > x);
			}

			static inline int IsNaN(float x)
			{
#ifdef _M_X64
				return _isnanf(x);
#else
				return isnan(x);
#endif
			}

			static inline int IsInf(float x)
			{
				return isinf(x);
			}

			static inline unsigned int Ones32(register unsigned int x)
			{
				/* 32-bit recursive reduction using SWAR...
					but first step is mapping 2-bit values
					into sum of 2 1-bit values in sneaky way
				*/
				x -= ((x >> 1) & 0x55555555);
				x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
				x = (((x >> 4) + x) & 0x0f0f0f0f);
				x += (x >> 8);
				x += (x >> 16);
				return(x & 0x0000003f);
			}

			static inline unsigned int Log2Floor(register unsigned int x)
			{
				x |= (x >> 1);
				x |= (x >> 2);
				x |= (x >> 4);
				x |= (x >> 8);
				x |= (x >> 16);
				return(Ones32(x >> 1));
			}

			static inline unsigned int Log2Ceil(register unsigned int x)
			{
				int y = (x & (x - 1));
				y |= -y;
				y >>= (32 - 1);
				x |= (x >> 1);
				x |= (x >> 2);
				x |= (x >> 4);
				x |= (x >> 8);
				x |= (x >> 16);
				return(Ones32(x >> 1) - y);
			}
			/*
			static inline int Log2(float x)
			{
				unsigned int ix = (unsigned int&)x;
				unsigned int exp = (ix >> 23) & 0xFF;
				int log2 = (unsigned int)(exp) - 127;

				return log2;
			}
			*/
		};
		inline int FloatAsInt(float val)
		{
			union InterCast
			{
				float fvalue;
				int ivalue;
			} cast;
			cast.fvalue = val;
			return cast.ivalue;
		}
		inline float IntAsFloat(int val)
		{
			union InterCast
			{
				float fvalue;
				int ivalue;
			} cast;
			cast.ivalue = val;
			return cast.fvalue;
		}

		inline unsigned short FloatToHalf(float val)
		{
			int x = *(int*)&val;
			unsigned short bits = (x >> 16) & 0x8000;
			unsigned short m = (x >> 12) & 0x07ff;
			unsigned int e = (x >> 23) & 0xff;
			if (e < 103)
				return bits;
			if (e > 142)
			{
				bits |= 0x7c00u;
				bits |= e == 255 && (x & 0x007fffffu);
				return bits;
			}
			if (e < 113)
			{
				m |= 0x0800u;
				bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
				return bits;
			}
			bits |= ((e - 112) << 10) | (m >> 1);
			bits += m & 1;
			return bits;
		}

		inline float HalfToFloat(unsigned short input)
		{
			union InterCast
			{
				float fvalue;
				int ivalue;
				InterCast() = default;
				InterCast(int ival)
				{
					ivalue = ival;
				}
			};
			static const InterCast magic = InterCast((127 + (127 - 15)) << 23);
			static const InterCast was_infnan = InterCast((127 + 16) << 23);
			InterCast o;
			o.ivalue = (input & 0x7fff) << 13;     // exponent/mantissa bits
			o.fvalue *= magic.fvalue;                 // exponent adjust
			if (o.fvalue >= was_infnan.fvalue)        // make sure Inf/NaN survive
				o.ivalue |= 255 << 23;
			o.ivalue |= (input & 0x8000) << 16;    // sign bit
			return o.fvalue;
		}

		class Random
		{
		private:
			unsigned int seed;
		public:
			Random(int seed)
			{
				this->seed = seed;
			}
			int Next() // random between 0 and RandMax (currently 0x7fff)
			{
				return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
			}
			int Next(int min, int max) // inclusive min, exclusive max
			{
				unsigned int a = ((seed = seed * 214013L + 2531011L) & 0xFFFF0000);
				unsigned int b = ((seed = seed * 214013L + 2531011L) >> 16);
				unsigned int r = a + b;
				return min + r % (max - min);
			}
			float NextFloat()
			{
				return ((Next() << 15) + Next()) / ((float)(1 << 30));
			}
			float NextFloat(float valMin, float valMax)
			{
				return valMin + (valMax - valMin) * NextFloat();
			}
			static int RandMax()
			{
				return 0x7fff;
			}
		};
	}
}

#endif 

/***********************************************************************
ARRAY.H
***********************************************************************/
#ifndef CORE_LIB_ARRAY_H
#define CORE_LIB_ARRAY_H


namespace CoreLib
{
	namespace Basic
	{
		template<typename T, int size>
		class Array
		{
		private:
			T _buffer[size];
			int _count;
		public:
			T* begin() const
			{
				return (T*)_buffer;
			}
			T* end() const
			{
				return (T*)_buffer+_count;
			}
		public:
			Array()
			{
				_count = 0;
			}
			inline int GetCapacity() const
			{
				return size;
			}
			inline int Count() const
			{
				return _count;
			}
			inline void SetSize(int newSize)
			{
#ifdef _DEBUG
				if (newSize > size)
					throw IndexOutofRangeException(L"size too large.");
#endif
				_count = newSize;
			}
			inline void Add(const T & item)
			{
#ifdef _DEBUG
				if (_count == size)
					throw IndexOutofRangeException(L"out of range access to static array.");
#endif
				_buffer[_count++] = item;
			}
			inline void Add(T && item)
			{
#ifdef _DEBUG
				if (_count == size)
					throw IndexOutofRangeException(L"out of range access to static array.");
#endif
				_buffer[_count++] = _Move(item);
			}

			inline T & operator [](int id) const
			{
#if _DEBUG
				if(id >= _count || id < 0)
					throw IndexOutofRangeException(L"Operator[]: Index out of Range.");
#endif
				return ((T*)_buffer)[id];
			}

			inline T* Buffer() const
			{
				return (T*)_buffer;
			}

			inline void Clear()
			{
				_count = 0;
			}

			template<typename T2>
			int IndexOf(const T2 & val) const
			{
				for (int i = 0; i < _count; i++)
				{
					if (_buffer[i] == val)
						return i;
				}
				return -1;
			}

			template<typename T2>
			int LastIndexOf(const T2 & val) const
			{
				for (int i = _count - 1; i >= 0; i--)
				{
					if(_buffer[i] == val)
						return i;
				}
				return -1;
			}

			inline ArrayView<T> GetArrayView() const
			{
				return ArrayView<T>((T*)_buffer, _count);
			}
			inline ArrayView<T> GetArrayView(int start, int count) const
			{
				return ArrayView<T>((T*)_buffer + start, count);
			}
		};
	}
}

#endif

/***********************************************************************
ALLOCATOR.H
***********************************************************************/
#ifndef CORE_LIB_ALLOCATOR_H
#define CORE_LIB_ALLOCATOR_H


namespace CoreLib
{
	namespace Basic
	{
		inline void * AlignedAlloc(size_t size, size_t alignment)
		{
#ifdef _MSC_VER
			return _aligned_malloc(size, alignment);
#else
			void * rs = 0;
			int succ = posix_memalign(&rs, alignment, size);
			if (succ!=0)
				rs = 0;
			return rs;
#endif
		}

		inline void AlignedFree(void * ptr)
		{
#ifdef _MSC_VER
			_aligned_free(ptr);
#else
			free(ptr);
#endif
		}

		class StandardAllocator
		{
		public:
			// not really called
			void * Alloc(size_t size)
			{
				return malloc(size);
			}
			void Free(void * ptr)
			{
				return free(ptr);
			}
		};

		template<int alignment>
		class AlignedAllocator
		{
		public:
			void * Alloc(size_t size)
			{
				return AlignedAlloc(size, alignment);
			}
			void Free(void * ptr)
			{
				return AlignedFree(ptr);
			}
		};
	}
}

#endif

/***********************************************************************
LIST.H
***********************************************************************/
#ifndef FUNDAMENTAL_LIB_LIST_H
#define FUNDAMENTAL_LIB_LIST_H

#include <type_traits>
#include <new>
#include <algorithm>

const int MIN_QSORT_SIZE = 32;

namespace CoreLib
{
	namespace Basic
	{
		template<typename T, int isPOD>
		class Initializer
		{

		};

		template<typename T>
		class Initializer<T, 0>
		{
		public:
			static void Initialize(T * buffer, int size)
			{
				for (int i = 0; i<size; i++)
					new (buffer + i) T();
			}
		};

		template<typename T, typename TAllocator>
		class AllocateMethod
		{
		public:
			static inline T* Alloc(int size)
			{
				TAllocator allocator;
				T * rs = (T*)allocator.Alloc(size*sizeof(T));
				Initializer<T, std::is_pod<T>::value>::Initialize(rs, size);
				return rs;
			}
			static inline void Free(T * ptr, int bufferSize)
			{
				TAllocator allocator;
				if (!std::is_trivially_destructible<T>::value)
				{
					for (int i = 0; i<bufferSize; i++)
						ptr[i].~T();
				}
				allocator.Free(ptr);
			}
		};

		template<typename T>
		class AllocateMethod<T, StandardAllocator>
		{
		public:
			static inline T* Alloc(int size)
			{
				return new T[size];
			}
			static inline void Free(T* ptr, int /*bufferSize*/)
			{
				delete [] ptr;
			}
		};

		template<typename T>
		class Initializer<T, 1>
		{
		public:
			static void Initialize(T * buffer, int size)
			{
				for (int i = 0; i<size; i++)
					new (buffer + i) T;
			}
		};

		template<typename T, typename TAllocator = StandardAllocator>
		class List
		{
		private:

			inline T * Allocate(int size)
			{
				return AllocateMethod<T, TAllocator>::Alloc(size);
				
			}
		private:
			static const int InitialSize = 16;
			TAllocator allocator;
		private:
			T * buffer;
			int _count;
			int bufferSize;
			void FreeBuffer()
			{
				AllocateMethod<T, TAllocator>::Free(buffer, bufferSize);
				buffer = 0;
			}
			void Free()
			{
				if (buffer)
				{
					FreeBuffer();
				}
				buffer = 0;
				_count = bufferSize = 0;
			}
		public:
			T* begin() const
			{
				return buffer;
			}
			T* end() const
			{
				return buffer+_count;
			}
		public:
			List()
				: buffer(0), _count(0), bufferSize(0)
			{
			}
			List(const List<T> & list)
				: buffer(0), _count(0), bufferSize(0)
			{
				this->operator=(list);
			}
			List(List<T> && list)
				: buffer(0), _count(0), bufferSize(0)
			{
				//int t = static_cast<int>(1.0f); reinterpret_cast<double*>(&t), dynamic_cast<> 
				this->operator=(static_cast<List<T>&&>(list));
			}
			~List()
			{
				Free();
			}
			List<T> & operator=(const List<T> & list)
			{
				Free();
				AddRange(list);

				return *this;
			}

			List<T> & operator=(List<T> && list)
			{
				Free();
				_count = list._count;
				bufferSize = list.bufferSize;
				buffer = list.buffer;

				list.buffer = 0;
				list._count = 0;
				list.bufferSize = 0;
				return *this;
			}

			T & First() const
			{
#ifdef _DEBUG
				if (_count == 0)
					throw "Index out of range.";
#endif
				return buffer[0];
			}

			T & Last() const
			{
#ifdef _DEBUG
				if (_count == 0)
					throw "Index out of range.";
#endif
				return buffer[_count-1];
			}

			inline void SwapWith(List<T, TAllocator> & other)
			{
				T* tmpBuffer = this->buffer;
				this->buffer = other.buffer;
				other.buffer = tmpBuffer;
				int tmpBufferSize = this->bufferSize;
				this->bufferSize = other.bufferSize;
				other.bufferSize = tmpBufferSize;
				int tmpCount = this->_count;
				this->_count = other._count;
				other._count = tmpCount;
				TAllocator tmpAlloc = _Move(this->allocator);
				this->allocator = _Move(other.allocator);
				other.allocator = _Move(tmpAlloc);
			}

			inline ArrayView<T> GetArrayView() const
			{
				return ArrayView<T>(buffer, _count);
			}

			inline ArrayView<T> GetArrayView(int start, int count) const
			{
#ifdef _DEBUG
				if (start + count > _count || start < 0 || count < 0)
					throw "Index out of range.";
#endif
				return ArrayView<T>(buffer + start, count);
			}

			void Add(T && obj)
			{
				if (bufferSize < _count + 1)
				{
					int newBufferSize = InitialSize;
					if (bufferSize)
						newBufferSize = (bufferSize << 1);

					Reserve(newBufferSize);
				}
				buffer[_count++] = static_cast<T&&>(obj);
			}

			void Add(const T & obj)
			{
				if (bufferSize < _count + 1)
				{
					int newBufferSize = InitialSize;
					if (bufferSize)
						newBufferSize = (bufferSize << 1);

					Reserve(newBufferSize);
				}
				buffer[_count++] = obj;

			}

			int Count() const
			{
				return _count;
			}

			T * Buffer() const
			{
				return buffer;
			}

			int Capacity() const
			{
				return bufferSize;
			}

			void Insert(int id, const T & val)
			{
				InsertRange(id, &val, 1);
			}

			void InsertRange(int id, const T * vals, int n)
			{
				if (bufferSize < _count + n)
				{
					int newBufferSize = InitialSize;
					while (newBufferSize < _count + n)
						newBufferSize = newBufferSize << 1;

					T * newBuffer = Allocate(newBufferSize);
					if (bufferSize)
					{
						/*if (std::has_trivial_copy_assign<T>::value && std::has_trivial_destructor<T>::value)
						{
							memcpy(newBuffer, buffer, sizeof(T) * id);
							memcpy(newBuffer + id + n, buffer + id, sizeof(T) * (_count - id));
						}
						else*/
						{
							for (int i = 0; i < id; i++)
								newBuffer[i] = buffer[i];
							for (int i = id; i < _count; i++)
								newBuffer[i + n] = T(static_cast<T&&>(buffer[i]));
						}
						FreeBuffer();
					}
					buffer = newBuffer;
					bufferSize = newBufferSize;
				}
				else
				{
					/*if (std::has_trivial_copy_assign<T>::value && std::has_trivial_destructor<T>::value)
						memmove(buffer + id + n, buffer + id, sizeof(T) * (_count - id));
					else*/
					{
						for (int i = _count - 1; i >= id; i--)
							buffer[i + n] = static_cast<T&&>(buffer[i]);
					}
				}
				/*if (std::has_trivial_copy_assign<T>::value && std::has_trivial_destructor<T>::value)
					memcpy(buffer + id, vals, sizeof(T) * n);
				else*/
					for (int i = 0; i < n; i++)
						buffer[id + i] = vals[i];

				_count += n;
			}

			//slower than original edition
			//void Add(const T & val)
			//{
			//	InsertRange(_count, &val, 1);
			//}

			void InsertRange(int id, const List<T> & list)
			{
				InsertRange(id, list.buffer, list._count);
			}

			void AddRange(const T * vals, int n)
			{
				InsertRange(_count, vals, n);
			}

			void AddRange(const List<T> & list)
			{
				InsertRange(_count, list.buffer, list._count);
			}

			void RemoveRange(int id, int deleteCount)
			{
#if _DEBUG
				if (id >= _count || id < 0)
					throw "Remove: Index out of range.";
				if(deleteCount < 0)
					throw "Remove: deleteCount smaller than zero.";
#endif
				int actualDeleteCount = ((id + deleteCount) >= _count)? (_count - id) : deleteCount;
				for (int i = id + actualDeleteCount; i < _count; i++)
					buffer[i - actualDeleteCount] = static_cast<T&&>(buffer[i]);
				_count -= actualDeleteCount;
			}

			void RemoveAt(int id)
			{
				RemoveRange(id, 1);
			}

			void Remove(const T & val)
			{
				int idx = IndexOf(val);
				if (idx != -1)
					RemoveAt(idx);
			}

			void Reverse()
			{
				for (int i = 0; i < (_count >> 1); i++)
				{
					Swap(buffer, i, _count - i - 1);
				}
			}

			void FastRemove(const T & val)
			{
				int idx = IndexOf(val);
				if (idx != -1 && _count-1 != idx)
				{
					buffer[idx] = _Move(buffer[_count-1]);
				}
				_count--;
			}

			void Clear()
			{
				_count = 0;
			}

			void Reserve(int size)
			{
				if(size > bufferSize)
				{
					T * newBuffer = Allocate(size);
					if (bufferSize)
					{
						/*if (std::has_trivial_copy_assign<T>::value && std::has_trivial_destructor<T>::value)
							memcpy(newBuffer, buffer, _count * sizeof(T));
						else*/
						{
							for (int i = 0; i < _count; i++)
								newBuffer[i] = static_cast<T&&>(buffer[i]);
						}
						FreeBuffer();
					}
					buffer = newBuffer;
					bufferSize = size;
				}
			}

			void GrowToSize(int size)
			{
				int newBufferSize = 1<<Math::Log2Ceil(size);
				if (bufferSize < newBufferSize)
				{
					Reserve(newBufferSize);
				}
				this->_count = size;
			}

			void SetSize(int size)
			{
				Reserve(size);
				_count = size;
			}

			void UnsafeShrinkToSize(int size)
			{
				_count = size;
			}

			void Compress()
			{
				if (bufferSize > _count && _count > 0)
				{
					T * newBuffer = Allocate(_count);
					for (int i = 0; i < _count; i++)
						newBuffer[i] = static_cast<T&&>(buffer[i]);
					FreeBuffer();
					buffer = newBuffer;
					bufferSize = _count;
				}
			}

#ifndef FORCE_INLINE
#ifdef _MSC_VER
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif
#endif

			FORCE_INLINE T & operator [](int id) const
			{
#if _DEBUG
				if(id >= _count || id < 0)
					throw IndexOutofRangeException(L"Operator[]: Index out of Range.");
#endif
				return buffer[id];
			}

			template<typename Func>
			int FindFirst(const Func & predicate) const
			{
				for (int i = 0; i < _count; i++)
				{
					if (predicate(buffer[i]))
						return i;
				}
				return -1;
			}

			template<typename Func>
			int FindLast(const Func & predicate) const
			{
				for (int i = _count - 1; i >= 0; i--)
				{
					if (predicate(buffer[i]))
						return i;
				}
				return -1;
			}

			template<typename T2>
			int IndexOf(const T2 & val) const
			{
				for (int i = 0; i < _count; i++)
				{
					if (buffer[i] == val)
						return i;
				}
				return -1;
			}

			template<typename T2>
			int LastIndexOf(const T2 & val) const
			{
				for (int i = _count - 1; i >= 0; i--)
				{
					if(buffer[i] == val)
						return i;
				}
				return -1;
			}

			void Sort()
			{
				Sort([](T& t1, T& t2){return t1<t2;});
			}

			bool Contains(const T & val)
			{
				for (int i = 0; i<_count; i++)
					if (buffer[i] == val)
						return true;
				return false;
			}

			template<typename Comparer>
			void Sort(Comparer compare)
			{
				//InsertionSort(buffer, 0, _count - 1);
				//QuickSort(buffer, 0, _count - 1, compare);
				std::sort(buffer, buffer + _count, compare);
			}

			template <typename IterateFunc>
			void ForEach(IterateFunc f) const
			{
				for (int i = 0; i<_count; i++)
					f(buffer[i]);
			}

			template<typename Comparer>
			void QuickSort(T * vals, int startIndex, int endIndex, Comparer comparer)
			{
				if(startIndex < endIndex)
				{
					if (endIndex - startIndex < MIN_QSORT_SIZE)
						InsertionSort(vals, startIndex, endIndex, comparer);
					else
					{
						int pivotIndex = (startIndex + endIndex) >> 1;
						int pivotNewIndex = Partition(vals, startIndex, endIndex, pivotIndex, comparer);
						QuickSort(vals, startIndex, pivotNewIndex - 1, comparer);
						QuickSort(vals, pivotNewIndex + 1, endIndex, comparer);
					}
				}

			}
			template<typename Comparer>
			int Partition(T * vals, int left, int right, int pivotIndex, Comparer comparer)
			{
				T pivotValue = vals[pivotIndex];
				Swap(vals, right, pivotIndex);
				int storeIndex = left;
				for (int i = left; i < right; i++)
				{
					if (comparer(vals[i], pivotValue))
					{
						Swap(vals, i, storeIndex);
						storeIndex++;
					}
				}
				Swap(vals, storeIndex, right);
				return storeIndex;
			}
			template<typename Comparer>
			void InsertionSort(T * vals, int startIndex, int endIndex, Comparer comparer)
			{
				for (int i = startIndex  + 1; i <= endIndex; i++)
				{
					T insertValue = static_cast<T&&>(vals[i]);
					int insertIndex = i - 1;
					while (insertIndex >= startIndex && comparer(insertValue, vals[insertIndex]))
					{
						vals[insertIndex + 1] = static_cast<T&&>(vals[insertIndex]);
						insertIndex--;
					}
					vals[insertIndex + 1] = static_cast<T&&>(insertValue);
				}
			}

			inline void Swap(T * vals, int index1, int index2)
			{
				if (index1 != index2)
				{
					T tmp = static_cast<T&&>(vals[index1]);
					vals[index1] = static_cast<T&&>(vals[index2]);
					vals[index2] = static_cast<T&&>(tmp);
				}
			}

			template<typename T2, typename Comparer>
			int BinarySearch(const T2 & obj, Comparer comparer)
			{
				int imin = 0, imax = _count - 1;
				while (imax >= imin)
				{
					int imid = (imin + imax) >> 1;
					int compareResult = comparer(buffer[imid], obj);
					if (compareResult == 0)
						return imid;
					else if (compareResult < 0)
						imin = imid + 1;
					else
						imax = imid - 1;
				}
				return -1;
			}

			template<typename T2>
			int BinarySearch(const T2 & obj)
			{
				return BinarySearch(obj, 
					[](T & curObj, const T2 & thatObj)->int
					{
						if (curObj < thatObj)
							return -1;
						else if (curObj == thatObj)
							return 0;
						else
							return 1;
					});
			}
		};

		template<typename T>
		T Min(const List<T> & list)
		{
			T minVal = list.First();
			for (int i = 1; i<list.Count(); i++)
				if (list[i] < minVal)
					minVal = list[i];
			return minVal;
		}

		template<typename T>
		T Max(const List<T> & list)
		{
			T maxVal = list.First();
			for (int i = 1; i<list.Count(); i++)
				if (list[i] > maxVal)
					maxVal = list[i];
			return maxVal;
		}
	}
}

#endif

/***********************************************************************
LINK.H
***********************************************************************/
#ifndef CORE_LIB_LINK_H
#define CORE_LIB_LINK_H


namespace CoreLib
{
	namespace Basic
	{
		template<typename T>
		class LinkedList;

		template<typename T>
		class LinkedNode
		{
			template<typename T1>
			friend class LinkedList;
		private:
			LinkedNode<T> *pPrev, *pNext;
			LinkedList<T> * FLink;
		public:
			T Value;
			LinkedNode (LinkedList<T> * lnk):FLink(lnk)
			{
				pPrev = pNext = 0;
			};
			LinkedNode<T> * GetPrevious()
			{
				return pPrev;
			};
			LinkedNode<T> * GetNext()
			{
				return pNext;
			};
			LinkedNode<T> * InsertAfter(const T & nData)
			{
				LinkedNode<T> * n = new LinkedNode<T>(FLink);
				n->Value = nData;
				n->pPrev = this;
				n->pNext = this->pNext;
				LinkedNode<T> *npp = n->pNext;
				if (npp)
				{
					npp->pPrev = n;
				}
				pNext = n;
				if (!n->pNext)
					FLink->FTail = n;
				FLink->FCount ++;
				return n;
			};
			LinkedNode<T> * InsertBefore(const T & nData)
			{
				LinkedNode<T> * n = new LinkedNode<T>(FLink);
				n->Value = nData;
				n->pPrev = pPrev;
				n->pNext = this;
				pPrev = n;
				LinkedNode<T> *npp = n->pPrev;
				if (npp)
					npp->pNext = n;
				if (!n->pPrev)
					FLink->FHead = n;
				FLink->FCount ++;
				return n;
			};
			void Delete()
			{
				if (pPrev)
					pPrev->pNext = pNext;
				if (pNext)
					pNext->pPrev = pPrev;
				FLink->FCount --;
				if (FLink->FHead == this)
				{
					FLink->FHead = pNext;
				}
				if (FLink->FTail == this)
				{
					FLink->FTail = pPrev;
				}
				delete this;
			}
		};
		template<typename T>
		class LinkedList
		{
			template<typename T1>
			friend class LinkedNode;
		private:
			LinkedNode<T> * FHead, *FTail;
			int FCount;
		public:
			class Iterator
			{
			public:
				LinkedNode<T> * Current, *Next;
				void SetCurrent(LinkedNode<T> * cur)
				{
					Current = cur;
					if (Current)
						Next = Current->GetNext();
					else
						Next = 0;
				}
				Iterator()
				{
					Current = Next = 0;
				}
				Iterator(LinkedNode<T> * cur)
				{
					SetCurrent(cur);
				}
				T & operator *() const
				{
					return Current->Value;
				}
				Iterator& operator ++()
				{
					SetCurrent(Next);
					return *this;
				}
				Iterator operator ++(int)
				{
					Iterator rs = *this;
					SetCurrent(Next);
					return rs;
				}
				bool operator != (const Iterator & iter) const
				{
					return Current != iter.Current;
				}
				bool operator == (const Iterator & iter) const
				{
					return Current == iter.Current;
				}
			};
			Iterator begin() const
			{
				return Iterator(FHead);
			}
			Iterator end() const
			{
				return Iterator(0);
			}
		public:
			LinkedList() : FHead(0), FTail(0), FCount(0)
			{
			}
			~LinkedList()
			{
				Clear();
			}
			LinkedList(const LinkedList<T> & link) : FHead(0), FTail(0), FCount(0)
			{
				this->operator=(link);
			}
			LinkedList(LinkedList<T> && link) : FHead(0), FTail(0), FCount(0)
			{
				this->operator=(_Move(link));
			}
			LinkedList<T> & operator = (LinkedList<T> && link)
			{
				if (FHead != 0)
					Clear();
				FHead = link.FHead;
				FTail = link.FTail;
				FCount = link.FCount;
				link.FHead = 0;
				link.FTail = 0;
				link.FCount = 0;
				for (auto node = FHead; node; node = node->GetNext())
					node->FLink = this;
				return *this;
			}
			LinkedList<T> & operator = (const LinkedList<T> & link)
			{
				if (FHead != 0)
					Clear();
				auto p = link.FHead;
				while (p)
				{
					AddLast(p->Value);
					p = p->GetNext();
				}
				return *this;
			}
			template<typename IteratorFunc>
			void ForEach(const IteratorFunc & f)
			{
				auto p = FHead;
				while (p)
				{
					f(p->Value);
					p = p->GetNext();
				}
			}
			LinkedNode<T> * GetNode(int x)
			{
				LinkedNode<T> *pCur = FHead;
				for (int i=0;i<x;i++)
				{
					if (pCur)
						pCur = pCur->pNext;
					else
						throw "Index out of range";
				}
				return pCur;
			};
			LinkedNode<T> * Find(const T& fData)
			{
				for (LinkedNode<T> * pCur = FHead; pCur; pCur = pCur->pNext)
				{
					if (pCur->Value == fData)
						return pCur;
				}
				return 0;
			};
			LinkedNode<T> * FirstNode()
			{
				return FHead;
			};
			T & First()
			{
				if (!FHead)
					throw IndexOutofRangeException("LinkedList: index out of range.");
				return FHead->Value;
			}
			T & Last()
			{
				if (!FTail)
					throw IndexOutofRangeException("LinkedList: index out of range.");
				return FTail->Value;
			}
			LinkedNode<T> * LastNode()
			{
				return FTail;
			};
			LinkedNode<T> * AddLast(const T & nData)
			{
				LinkedNode<T> * n = new LinkedNode<T>(this);
				n->Value = nData;
				n->pPrev = FTail;
				if (FTail)
					FTail->pNext = n;
				n->pNext = 0;
				FTail = n;
				if (!FHead)
					FHead = n;
				FCount ++;
				return n;
			};
			// Insert a blank node
			LinkedNode<T> * AddLast()
			{
				LinkedNode<T> * n = new LinkedNode<T>(this);
				n->pPrev = FTail;
				if (FTail)
					FTail->pNext = n;
				n->pNext = 0;
				FTail = n;
				if (!FHead)
					FHead = n;
				FCount ++;
				return n;
			};
			LinkedNode<T> * AddFirst(const T& nData)
			{
				LinkedNode<T> *n = new LinkedNode<T>(this);
				n->Value = nData;
				n->pPrev = 0;
				n->pNext = FHead;
				if (FHead)
					FHead->pPrev = n;
				FHead = n;
				if (!FTail)
					FTail = n;
				FCount ++;
				return n;
			};
			void Delete(LinkedNode<T>*n, int Count = 1)
			{
				LinkedNode<T> *n1,*n2 = 0, *tn;
				n1 = n->pPrev;
				tn = n;
				int numDeleted = 0;
				for (int i=0; i<Count; i++)
				{
					n2 = tn->pNext;
					delete tn;
					tn = n2;
					numDeleted++;
					if (tn == 0)
						break;
				}
				if (n1)
					n1->pNext = n2;
				else
					FHead = n2;
				if (n2)
					n2->pPrev = n1;
				else
					FTail = n1;
				FCount -= numDeleted;
			}
			void Clear()
			{
				for (LinkedNode<T> *n = FHead; n; )
				{
					LinkedNode<T> * tmp = n->pNext;
					delete n;
					n = tmp;
				}
				FHead = 0;
				FTail = 0;
				FCount = 0;
			}
			List<T> ToList() const
			{
				List<T> rs;
				rs.Reserve(FCount);
				for (auto & item : *this)
				{
					rs.Add(item);
				}
				return rs;
			}
			int Count()
			{
				return FCount;
			}
		};
	}
}
#endif

/***********************************************************************
INTSET.H
***********************************************************************/
#ifndef BIT_VECTOR_INT_SET_H
#define BIT_VECTOR_INT_SET_H

#include <memory.h>

namespace CoreLib
{
	namespace Basic
	{
		class IntSet
		{
		private:
			List<int> buffer;
		public:
			IntSet()
			{}
			IntSet(const IntSet & other)
			{
				buffer = other.buffer;
			}
			IntSet(IntSet && other)
			{
				*this = (_Move(other));
			}
			IntSet & operator = (IntSet && other)
			{
				buffer = _Move(other.buffer);
				return *this;
			}
			IntSet & operator = (const IntSet & other)
			{
				buffer = other.buffer;
				return *this;
			}
			int GetHashCode()
			{
				int rs = 0;
				for (auto val : buffer)
					rs ^= val;
				return rs;
			}
			IntSet(int maxVal)
			{
				SetMax(maxVal);
			}
			int Size() const
			{
				return buffer.Count()*32;
			}
			void SetMax(int val)
			{
				Resize(val);
				Clear();
			}
			void SetAll()
			{
				for (int i = 0; i<buffer.Count(); i++)
					buffer[i] = 0xFFFFFFFF;
			}
			void Resize(int size)
			{
				int oldBufferSize = buffer.Count();
				buffer.SetSize((size+31)>>5);
				if (buffer.Count() > oldBufferSize)
					memset(buffer.Buffer()+oldBufferSize, 0, (buffer.Count()-oldBufferSize) * sizeof(int));
			}
			void Clear()
			{
				for (int i = 0; i<buffer.Count(); i++)
					buffer[i] = 0;
			}
			void Add(int val)
			{
				int id = val>>5;
				if (id < buffer.Count())
					buffer[id] |= (1<<(val&31));
				else
				{
					int oldSize = buffer.Count();
					buffer.SetSize(id+1);
					memset(buffer.Buffer() + oldSize, 0, (buffer.Count()-oldSize)*sizeof(int));
					buffer[id] |= (1<<(val&31));
				}
			}
			void Remove(int val)
			{
				if ((val>>5) < buffer.Count())
					buffer[(val>>5)] &= ~(1<<(val&31));
			}
			bool Contains(int val) const
			{
				if ((val>>5) >= buffer.Count())
					return false;
				return (buffer[(val>>5)] & (1<<(val&31))) != 0;
			}
			void UnionWith(const IntSet & set)
			{
				for (int i = 0; i<Math::Min(set.buffer.Count(), buffer.Count()); i++)
				{
					buffer[i] |= set.buffer[i];
				}
				if (set.buffer.Count() > buffer.Count())
					buffer.AddRange(set.buffer.Buffer()+buffer.Count(), set.buffer.Count()-buffer.Count());
			}
			bool operator == (const IntSet & set)
			{
				if (buffer.Count() != set.buffer.Count())
					return false;
				for (int i = 0; i<buffer.Count(); i++)
					if (buffer[i] != set.buffer[i])
						return false;
				return true;
			}
			bool operator != (const IntSet & set)
			{
				return !(*this == set);
			}
			void IntersectWith(const IntSet & set)
			{
				if (set.buffer.Count() < buffer.Count())
					memset(buffer.Buffer() + set.buffer.Count(), 0, (buffer.Count()-set.buffer.Count())*sizeof(int));
				for (int i = 0; i<Math::Min(set.buffer.Count(), buffer.Count()); i++)
				{
					buffer[i] &= set.buffer[i];
				}
			}
			static void Union(IntSet & rs, const IntSet & set1, const IntSet & set2)
			{
				rs.buffer.SetSize(Math::Max(set1.buffer.Count(), set2.buffer.Count()));
				rs.Clear();
				for (int i = 0; i<set1.buffer.Count(); i++)
					rs.buffer[i] |= set1.buffer[i];
				for (int i = 0; i<set2.buffer.Count(); i++)
					rs.buffer[i] |= set2.buffer[i];
			}
			static void Intersect(IntSet & rs, const IntSet & set1, const IntSet & set2)
			{
				rs.buffer.SetSize(Math::Min(set1.buffer.Count(), set2.buffer.Count()));
				for (int i = 0; i<rs.buffer.Count(); i++)
					rs.buffer[i] = set1.buffer[i] & set2.buffer[i];
			}
			static void Subtract(IntSet & rs, const IntSet & set1, const IntSet & set2)
			{
				rs.buffer.SetSize(set1.buffer.Count());
				for (int i = 0; i<Math::Min(set1.buffer.Count(), set2.buffer.Count()); i++)
					rs.buffer[i] = set1.buffer[i] & (~set2.buffer[i]);
			}
			static bool HasIntersection(const IntSet & set1, const IntSet & set2)
			{
				for (int i = 0; i<Math::Min(set1.buffer.Count(), set2.buffer.Count()); i++)
				{
					if (set1.buffer[i] & set2.buffer[i])
						return true;
				}
				return false;
			}
		};
	}
}

#endif

/***********************************************************************
DICTIONARY.H
***********************************************************************/
#ifndef CORE_LIB_DICTIONARY_H
#define CORE_LIB_DICTIONARY_H
namespace CoreLib
{
	namespace Basic
	{
		template<int IsInt>
		class Hash
		{
		public:
		};
		template<>
		class Hash<1>
		{
		public:
			template<typename TKey>
			static int GetHashCode(TKey & key)
			{
				return (int)key;
			}
		};
		template<>
		class Hash<0>
		{
		public:
			template<typename TKey>
			static int GetHashCode(TKey & key)
			{
				return key.GetHashCode();
			}
		};
		template<int IsPointer>
		class PointerHash
		{};
		template<>
		class PointerHash<1>
		{
		public:
			template<typename TKey>
			static int GetHashCode(TKey & key)
			{
				return (int)((CoreLib::PtrInt)key)/sizeof(typename std::remove_pointer<TKey>::type);
			}
		};
		template<>
		class PointerHash<0>
		{
		public:
			template<typename TKey>
			static int GetHashCode(TKey & key)
			{
				return Hash<std::is_integral<TKey>::value || std::is_enum<TKey>::value>::GetHashCode(key);
			}
		};

		template<typename TKey>
		int GetHashCode(const TKey & key)
		{
			return PointerHash<std::is_pointer<TKey>::value>::GetHashCode(key);
		}

		template<typename TKey>
		int GetHashCode(TKey & key)
		{
			return PointerHash<std::is_pointer<TKey>::value>::GetHashCode(key);
		}
		
		
		inline int GetHashCode(double key)
		{
			return FloatAsInt((float)key);
		}
		inline int GetHashCode(float key)
		{
			return FloatAsInt(key);
		}

		template<typename TKey, typename TValue>
		class KeyValuePair
		{
		public:
			TKey Key;
			TValue Value;
			KeyValuePair()
			{}
			KeyValuePair(const TKey & key, const TValue & value)
			{
				Key = key;
				Value = value;
			}
			KeyValuePair(TKey && key, TValue && value)
			{
				Key = _Move(key);
				Value = _Move(value);
			}
			KeyValuePair(TKey && key, const TValue & value)
			{
				Key = _Move(key);
				Value = value;
			}
			KeyValuePair(const KeyValuePair<TKey, TValue> & _that)
			{
				Key = _that.Key;
				Value = _that.Value;
			}
			KeyValuePair(KeyValuePair<TKey, TValue> && _that)
			{
				operator=(_Move(_that));
			}
			KeyValuePair & operator=(KeyValuePair<TKey, TValue> && that)
			{
				Key = _Move(that.Key);
				Value = _Move(that.Value);
				return *this;
			}
			KeyValuePair & operator=(const KeyValuePair<TKey, TValue> & that)
			{
				Key = that.Key;
				Value = that.Value;
				return *this;
			}
			int GetHashCode()
			{
				return GetHashCode(Key);
			}
		};

		const float MaxLoadFactor = 0.7f;

		template<typename TKey, typename TValue>
		class Dictionary
		{
			friend class Iterator;
			friend class ItemProxy;
		private:
			inline int GetProbeOffset(int /*probeId*/) const
			{
				// quadratic probing
				return 1;
			}
		private:
			int bucketSizeMinusOne, shiftBits;
			int _count;
			IntSet marks;
			KeyValuePair<TKey, TValue>* hashMap;
			void Free()
			{
				if (hashMap)
					delete [] hashMap;
				hashMap = 0;
			}
			inline bool IsDeleted(int pos) const
			{
				return marks.Contains((pos<<1) + 1);
			}
			inline bool IsEmpty(int pos) const
			{
				return !marks.Contains((pos<<1));
			}
			inline void SetDeleted(int pos, bool val)
			{
				if (val)
					marks.Add((pos<<1)+1);
				else
					marks.Remove((pos<<1)+1);
			}
			inline void SetEmpty(int pos, bool val)
			{
				if (val)
					marks.Remove((pos<<1));
				else
					marks.Add((pos<<1));
			}
			struct FindPositionResult
			{
				int ObjectPosition;
				int InsertionPosition;
				FindPositionResult()
				{
					ObjectPosition = -1;
					InsertionPosition = -1;
				}
				FindPositionResult(int objPos, int insertPos)
				{
					ObjectPosition = objPos;
					InsertionPosition = insertPos;
				}

			};
			inline int GetHashPos(TKey & key) const
			{
				return ((unsigned int)(GetHashCode(key)*2654435761)) >> shiftBits;
			}
			FindPositionResult FindPosition(const TKey & key) const
			{
				int hashPos = GetHashPos((TKey&)key);
				int insertPos = -1;
				int numProbes = 0;
				while (numProbes <= bucketSizeMinusOne)
				{
					if (IsEmpty(hashPos))
					{
						if (insertPos == -1)
							return FindPositionResult(-1, hashPos);
						else
							return FindPositionResult(-1, insertPos);
					}
					else if (IsDeleted(hashPos))
					{
						if (insertPos == -1)
							insertPos = hashPos;
					}
					else if (hashMap[hashPos].Key == key)
					{
						return FindPositionResult(hashPos, -1);
					}
					numProbes++;
					hashPos = (hashPos+GetProbeOffset(numProbes)) & bucketSizeMinusOne;
				}
				if (insertPos != -1)
					return FindPositionResult(-1, insertPos);
				throw InvalidOperationException(L"Hash map is full. This indicates an error in Key::Equal or Key::GetHashCode.");
			}
			TValue & _Insert(KeyValuePair<TKey, TValue> && kvPair, int pos)
			{
				hashMap[pos] = _Move(kvPair);
				SetEmpty(pos, false);
				SetDeleted(pos, false);
				return hashMap[pos].Value;
			}
			void Rehash()
			{
				if (bucketSizeMinusOne == -1 || _count/(float)bucketSizeMinusOne >= MaxLoadFactor)
				{
					int newSize = (bucketSizeMinusOne+1) * 2;
					int newShiftBits = shiftBits - 1;
					if (newSize == 0)
					{
						newSize = 16;
						newShiftBits = 28;
					}
					Dictionary<TKey, TValue> newDict;
					newDict.shiftBits = newShiftBits;
					newDict.bucketSizeMinusOne = newSize - 1;
					newDict.hashMap = new KeyValuePair<TKey, TValue>[newSize];
					newDict.marks.SetMax(newSize*2);
					if (hashMap)
					{
						for (auto & kvPair : *this)
						{
							newDict.Add(_Move(kvPair));
						}
					}
					*this = _Move(newDict);
				}
			}
			
			bool AddIfNotExists(KeyValuePair<TKey, TValue> && kvPair)
			{
				Rehash();
				auto pos = FindPosition(kvPair.Key);
				if (pos.ObjectPosition != -1)
					return false;
				else if (pos.InsertionPosition != -1)
				{
					_count++;
					_Insert(_Move(kvPair), pos.InsertionPosition);
					return true;
				}
				else
					throw InvalidOperationException(L"Inconsistent find result returned. This is a bug in Dictionary implementation.");
			}
			void Add(KeyValuePair<TKey, TValue> && kvPair)
			{
				if (!AddIfNotExists(_Move(kvPair)))
					throw KeyExistsException(L"The key already exists in Dictionary.");
			}
			TValue & Set(KeyValuePair<TKey, TValue> && kvPair)
			{
				Rehash();
				auto pos = FindPosition(kvPair.Key);
				if (pos.ObjectPosition != -1)
					return _Insert(_Move(kvPair), pos.ObjectPosition);
				else if (pos.InsertionPosition != -1)
				{
					_count++;
					return _Insert(_Move(kvPair), pos.InsertionPosition);
				}
				else
					throw InvalidOperationException(L"Inconsistent find result returned. This is a bug in Dictionary implementation.");
			}
		public:
			class Iterator
			{
			private:
				const Dictionary<TKey, TValue> * dict;
				int pos;
			public:
				KeyValuePair<TKey, TValue> & operator *() const
				{
					return dict->hashMap[pos];
				}
				KeyValuePair<TKey, TValue> * operator ->() const
				{
					return dict->hashMap + pos;
				}
				Iterator & operator ++()
				{
					if (pos > dict->bucketSizeMinusOne)
						return *this;
					pos++;
					while (pos <= dict->bucketSizeMinusOne && (dict->IsDeleted(pos) || dict->IsEmpty(pos)))
					{
						pos++;
					}
					return *this;
				}
				Iterator operator ++(int)
				{
					Iterator rs = * this;
					operator++( );
					return rs;
				}
				bool operator != (const Iterator & _that) const
				{
					return pos != _that.pos || dict != _that.dict;
				}
				bool operator == (const Iterator & _that) const
				{
					return pos == _that.pos && dict == _that.dict;
				}
				Iterator(const Dictionary<TKey, TValue> * _dict, int _pos)
				{
					this->dict = _dict;
					this->pos = _pos;
				}
				Iterator()
				{
					this->dict = 0;
					this->pos = 0;
				}
			};

			Iterator begin() const
			{
				int pos = 0;
				while (pos < bucketSizeMinusOne + 1)
				{
					if (IsEmpty(pos) || IsDeleted(pos))
						pos++;
					else
						break;
				}
				return Iterator(this, pos);
			}
			Iterator end() const
			{
				return Iterator(this, bucketSizeMinusOne + 1);
			}
		public:
			void Add(const TKey & key, const TValue & value)
			{
				Add(KeyValuePair<TKey, TValue>(key, value));
			}
			void Add(TKey && key, TValue && value)
			{
				Add(KeyValuePair<TKey, TValue>(_Move(key), _Move(value)));
			}
			bool AddIfNotExists(const TKey & key, const TValue & value)
			{
				return AddIfNotExists(KeyValuePair<TKey, TValue>(key, value));
			}
			bool AddIfNotExists(TKey && key, TValue && value)
			{
				return AddIfNotExists(KeyValuePair<TKey, TValue>(_Move(key), _Move(value)));
			}
			void Remove(const TKey & key)
			{
				if (_count == 0)
					return;
				auto pos = FindPosition(key);
				if (pos.ObjectPosition != -1)
				{
					SetDeleted(pos.ObjectPosition, true);
					_count--;
				}
			}
			void Clear()
			{
				_count = 0;

				marks.Clear();
			}
			bool ContainsKey(const TKey & key) const
			{
				if (bucketSizeMinusOne == -1)
					return false;
				auto pos = FindPosition(key);
				return pos.ObjectPosition != -1;
			}
			bool TryGetValue(const TKey & key, TValue & value) const
			{
				if (bucketSizeMinusOne == -1)
					return false;
				auto pos = FindPosition(key);
				if (pos.ObjectPosition != -1)
				{
					value = hashMap[pos.ObjectPosition].Value;
					return true;
				}
				return false;
			}
			TValue * TryGetValue(const TKey & key) const
			{
				if (bucketSizeMinusOne == -1)
					return nullptr;
				auto pos = FindPosition(key);
				if (pos.ObjectPosition != -1)
				{
					return &hashMap[pos.ObjectPosition].Value;
				}
				return nullptr;
			}
			class ItemProxy
			{
			private:
				const Dictionary<TKey, TValue> * dict;
				TKey key;
			public:
				ItemProxy(const TKey & _key, const Dictionary<TKey, TValue> * _dict)
				{
					this->dict = _dict;
					this->key = _key;
				}
				ItemProxy(TKey && _key, const Dictionary<TKey, TValue> * _dict)
				{
					this->dict = _dict;
					this->key = _Move(_key);
				}
				TValue & GetValue() const
				{
					auto pos = dict->FindPosition(key);
					if (pos.ObjectPosition != -1)
					{
						return dict->hashMap[pos.ObjectPosition].Value;
					}
					else
						throw KeyNotFoundException(L"The key does not exists in dictionary.");
				}
				inline TValue & operator()() const
				{
					return GetValue();
				}
				operator TValue&() const
				{
					return GetValue();
				}
				TValue & operator = (const TValue & val) const
				{
					return ((Dictionary<TKey,TValue>*)dict)->Set(KeyValuePair<TKey, TValue>(_Move(key), val));
				}
				TValue & operator = (TValue && val) const
				{
					return ((Dictionary<TKey,TValue>*)dict)->Set(KeyValuePair<TKey, TValue>(_Move(key), _Move(val)));
				}
			};
			ItemProxy operator [](const TKey & key) const
			{
				return ItemProxy(key, this);
			}
			ItemProxy operator [](TKey && key) const
			{
				return ItemProxy(_Move(key), this);
			}
			int Count() const
			{
				return _count;
			}
		public:
			Dictionary()
			{
				bucketSizeMinusOne = -1;
				shiftBits = 32;
				_count = 0;
				hashMap = 0;
			}
			Dictionary(const Dictionary<TKey, TValue> & other)
				: bucketSizeMinusOne(-1), _count(0), hashMap(0)
			{
				*this = other;
			}
			Dictionary(Dictionary<TKey, TValue> && other)
				: bucketSizeMinusOne(-1), _count(0), hashMap(0)
			{
				*this = (_Move(other));
			}
			Dictionary<TKey, TValue> & operator = (const Dictionary<TKey, TValue> & other)
			{
				Free();
				bucketSizeMinusOne = other.bucketSizeMinusOne;
				_count = other._count;
				shiftBits = other.shiftBits;
				hashMap = new KeyValuePair<TKey, TValue>[other.bucketSizeMinusOne+1];
				marks = other.marks;
				for (int i = 0; i<= bucketSizeMinusOne; i++)
					hashMap[i] = other.hashMap[i];
				return *this;
			}
			Dictionary<TKey, TValue> & operator = (Dictionary<TKey, TValue> && other)
			{
				Free();
				bucketSizeMinusOne = other.bucketSizeMinusOne;
				_count = other._count;
				hashMap = other.hashMap;
				shiftBits = other.shiftBits;
				marks = _Move(other.marks);
				other.hashMap = 0;
				other._count = 0;
				other.bucketSizeMinusOne = -1;
				return *this;
			}
			~Dictionary()
			{
				Free();
			}
		};

		template<typename TKey, typename TValue>
		class EnumerableDictionary
		{
			friend class Iterator;
			friend class ItemProxy;
		private:
			inline int GetProbeOffset(int /*probeIdx*/) const
			{
				// quadratic probing
				return 1;
			}
		private:
			int bucketSizeMinusOne, shiftBits;
			int _count;
			IntSet marks;

			// debug op
			struct Op
			{
				TKey key;
				int opType;
				Op()
				{}
				Op(const TKey & key, int t)
				{
					this->key = key;
					opType = t;
				}
			};
			LinkedList<KeyValuePair<TKey, TValue>> kvPairs;
			LinkedNode<KeyValuePair<TKey, TValue>>** hashMap;
			void Free()
			{
				if (hashMap)
					delete[] hashMap;
				hashMap = 0;
				kvPairs.Clear();
			}
			inline bool IsDeleted(int pos) const
			{
				return marks.Contains((pos << 1) + 1);
			}
			inline bool IsEmpty(int pos) const
			{
				return !marks.Contains((pos << 1));
			}
			inline void SetDeleted(int pos, bool val)
			{
				if (val)
					marks.Add((pos << 1) + 1);
				else
					marks.Remove((pos << 1) + 1);
			}
			inline void SetEmpty(int pos, bool val)
			{
				if (val)
					marks.Remove((pos << 1));
				else
					marks.Add((pos << 1));
			}
			struct FindPositionResult
			{
				int ObjectPosition;
				int InsertionPosition;
				FindPositionResult()
				{
					ObjectPosition = -1;
					InsertionPosition = -1;
				}
				FindPositionResult(int objPos, int insertPos)
				{
					ObjectPosition = objPos;
					InsertionPosition = insertPos;
				}

			};
			inline int GetHashPos(TKey & key) const
			{
				return ((unsigned int)(GetHashCode(key) * 2654435761)) >> shiftBits;
			}
			FindPositionResult FindPosition(const TKey & key) const
			{
				int hashPos = GetHashPos((TKey&)key);
				int insertPos = -1;
				int numProbes = 0;
				while (numProbes <= bucketSizeMinusOne)
				{
					if (IsEmpty(hashPos))
					{
						if (insertPos == -1)
							return FindPositionResult(-1, hashPos);
						else
							return FindPositionResult(-1, insertPos);
					}
					else if (IsDeleted(hashPos))
					{
						if (insertPos == -1)
							insertPos = hashPos;
					}
					else if (hashMap[hashPos]->Value.Key == key)
					{
						return FindPositionResult(hashPos, -1);
					}
					numProbes++;
					hashPos = (hashPos + GetProbeOffset(numProbes)) & bucketSizeMinusOne;
				}
				if (insertPos != -1)
					return FindPositionResult(-1, insertPos);
				throw InvalidOperationException(L"Hash map is full. This indicates an error in Key::Equal or Key::GetHashCode.");
			}
			TValue & _Insert(KeyValuePair<TKey, TValue> && kvPair, int pos)
			{
				auto node = kvPairs.AddLast();
				node->Value = _Move(kvPair);
				hashMap[pos] = node;
				SetEmpty(pos, false);
				SetDeleted(pos, false);
				return node->Value.Value;
			}
			void Rehash()
			{
				if (bucketSizeMinusOne == -1 || _count / (float)bucketSizeMinusOne >= MaxLoadFactor)
				{
					int newSize = (bucketSizeMinusOne + 1) * 2;
					int newShiftBits = shiftBits - 1;
					if (newSize == 0)
					{
						newSize = 16;
						newShiftBits = 28;
					}
					EnumerableDictionary<TKey, TValue> newDict;
					newDict.shiftBits = newShiftBits;
					newDict.bucketSizeMinusOne = newSize - 1;
					newDict.hashMap = new LinkedNode<KeyValuePair<TKey, TValue>>*[newSize];
					newDict.marks.SetMax(newSize * 2);
					if (hashMap)
					{
						for (auto & kvPair : *this)
						{
							newDict.Add(_Move(kvPair));
						}
					}
					*this = _Move(newDict);
				}
			}

			bool AddIfNotExists(KeyValuePair<TKey, TValue> && kvPair)
			{
				Rehash();
				auto pos = FindPosition(kvPair.Key);
				if (pos.ObjectPosition != -1)
					return false;
				else if (pos.InsertionPosition != -1)
				{
					_count++;
					_Insert(_Move(kvPair), pos.InsertionPosition);
					return true;
				}
				else
					throw InvalidOperationException(L"Inconsistent find result returned. This is a bug in Dictionary implementation.");
			}
			void Add(KeyValuePair<TKey, TValue> && kvPair)
			{
				if (!AddIfNotExists(_Move(kvPair)))
					throw KeyExistsException(L"The key already exists in Dictionary.");
			}
			TValue & Set(KeyValuePair<TKey, TValue> && kvPair)
			{
				Rehash();
				auto pos = FindPosition(kvPair.Key);
				if (pos.ObjectPosition != -1)
				{
					hashMap[pos.ObjectPosition]->Delete();
					return _Insert(_Move(kvPair), pos.ObjectPosition);
				}
				else if (pos.InsertionPosition != -1)
				{
					_count++;
					return _Insert(_Move(kvPair), pos.InsertionPosition);
				}
				else
					throw InvalidOperationException(L"Inconsistent find result returned. This is a bug in Dictionary implementation.");
			}
		public:
			typedef typename LinkedList<KeyValuePair<TKey, TValue>>::Iterator Iterator;

			typename LinkedList<KeyValuePair<TKey, TValue>>::Iterator begin() const
			{
				return kvPairs.begin();
			}
			typename LinkedList<KeyValuePair<TKey, TValue>>::Iterator end() const
			{
				return kvPairs.end();
			}
		public:
			void Add(const TKey & key, const TValue & value)
			{
				Add(KeyValuePair<TKey, TValue>(key, value));
			}
			void Add(TKey && key, TValue && value)
			{
				Add(KeyValuePair<TKey, TValue>(_Move(key), _Move(value)));
			}
			bool AddIfNotExists(const TKey & key, const TValue & value)
			{
				return AddIfNotExists(KeyValuePair<TKey, TValue>(key, value));
			}
			bool AddIfNotExists(TKey && key, TValue && value)
			{
				return AddIfNotExists(KeyValuePair<TKey, TValue>(_Move(key), _Move(value)));
			}
			void Remove(const TKey & key)
			{
				if (_count > 0)
				{
					auto pos = FindPosition(key);
					if (pos.ObjectPosition != -1)
					{
						kvPairs.Delete(hashMap[pos.ObjectPosition]);
						hashMap[pos.ObjectPosition] = 0;
						SetDeleted(pos.ObjectPosition, true);
						_count--;
					}
				}
			}
			void Clear()
			{
				_count = 0;
				kvPairs.Clear();
				marks.Clear();
			}
			bool ContainsKey(const TKey & key) const
			{
				if (bucketSizeMinusOne == -1)
					return false;
				auto pos = FindPosition(key);
				return pos.ObjectPosition != -1;
			}
			TValue * TryGetValue(const TKey & key) const
			{
				if (bucketSizeMinusOne == -1)
					return nullptr;
				auto pos = FindPosition(key);
				if (pos.ObjectPosition != -1)
				{
					return &(hashMap[pos.ObjectPosition]->Value.Value);
				}
				return nullptr;
			}
			bool TryGetValue(const TKey & key, TValue & value) const
			{
				if (bucketSizeMinusOne == -1)
					return false;
				auto pos = FindPosition(key);
				if (pos.ObjectPosition != -1)
				{
					value = hashMap[pos.ObjectPosition]->Value.Value;
					return true;
				}
				return false;
			}
			class ItemProxy
			{
			private:
				const EnumerableDictionary<TKey, TValue> * dict;
				TKey key;
			public:
				ItemProxy(const TKey & _key, const EnumerableDictionary<TKey, TValue> * _dict)
				{
					this->dict = _dict;
					this->key = _key;
				}
				ItemProxy(TKey && _key, const EnumerableDictionary<TKey, TValue> * _dict)
				{
					this->dict = _dict;
					this->key = _Move(_key);
				}
				TValue & GetValue() const
				{
					auto pos = dict->FindPosition(key);
					if (pos.ObjectPosition != -1)
					{
						return dict->hashMap[pos.ObjectPosition]->Value.Value;
					}
					else
					{
						throw KeyNotFoundException(L"The key does not exists in dictionary.");
					}
				}
				inline TValue & operator()() const
				{
					return GetValue();
				}
				operator TValue&() const
				{
					return GetValue();
				}
				TValue & operator = (const TValue & val)
				{
					return ((EnumerableDictionary<TKey, TValue>*)dict)->Set(KeyValuePair<TKey, TValue>(_Move(key), val));
				}
				TValue & operator = (TValue && val)
				{
					return ((EnumerableDictionary<TKey, TValue>*)dict)->Set(KeyValuePair<TKey, TValue>(_Move(key), _Move(val)));
				}
			};
			ItemProxy operator [](const TKey & key) const
			{
				return ItemProxy(key, this);
			}
			ItemProxy operator [](TKey && key) const
			{
				return ItemProxy(_Move(key), this);
			}
			int Count() const
			{
				return _count;
			}
		public:
			EnumerableDictionary()
			{
				bucketSizeMinusOne = -1;
				shiftBits = 32;
				_count = 0;
				hashMap = 0;
			}
			EnumerableDictionary(const EnumerableDictionary<TKey, TValue> & other)
				: bucketSizeMinusOne(-1), _count(0), hashMap(0)
			{
				*this = other;
			}
			EnumerableDictionary(EnumerableDictionary<TKey, TValue> && other)
				: bucketSizeMinusOne(-1), _count(0), hashMap(0)
			{
				*this = (_Move(other));
			}
			EnumerableDictionary<TKey, TValue> & operator = (const EnumerableDictionary<TKey, TValue> & other)
			{
				Clear();
				for (auto & item : other)
					Add(item.Key, item.Value);
				return *this;
			}
			EnumerableDictionary<TKey, TValue> & operator = (EnumerableDictionary<TKey, TValue> && other)
			{
				Free();
				bucketSizeMinusOne = other.bucketSizeMinusOne;
				_count = other._count;
				hashMap = other.hashMap;
				shiftBits = other.shiftBits;
				marks = _Move(other.marks);
				other.hashMap = 0;
				other._count = 0;
				other.bucketSizeMinusOne = -1;
				kvPairs = _Move(other.kvPairs);
				return *this;
			}
			~EnumerableDictionary()
			{
				Free();
			}
		};
		
		class _DummyClass
		{};

		template<typename T, typename DictionaryType>
		class HashSetBase
		{
		private:
			DictionaryType dict;
		public:
			HashSetBase()
			{}
			HashSetBase(const HashSetBase & set)
			{
				operator=(set);
			}
			HashSetBase(HashSetBase && set)
			{
				operator=(_Move(set));
			}
			HashSetBase & operator = (const HashSetBase & set)
			{
				dict = set.dict;
				return *this;
			}
			HashSetBase & operator = (HashSetBase && set)
			{
				dict = _Move(set.dict);
				return *this;
			}
		public:
			class Iterator
			{
			private:
				typename DictionaryType::Iterator iter;
			public:
				Iterator() = default;
				T & operator *() const
				{
					return (*iter).Key;
				}
				T * operator ->() const
				{
					return &(*iter).Key;
				}
				Iterator & operator ++()
				{
					++iter;
					return *this;
				}
				Iterator operator ++(int)
				{
					Iterator rs = * this;
					operator++( );
					return rs;
				}
				bool operator != (const Iterator & _that) const
				{
					return iter != _that.iter;
				}
				bool operator == (const Iterator & _that) const
				{
					return iter == _that.iter;
				}
				Iterator(const typename DictionaryType::Iterator & _iter)
				{
					this->iter = _iter;
				}
			};
			Iterator begin() const
			{
				return Iterator(dict.begin());
			}
			Iterator end() const
			{
				return Iterator(dict.end());
			}
		public:
			T & First() const
			{
				return *begin();
			}
			T & Last() const
			{
				return *end();
			}
			int Count() const
			{
				return dict.Count();
			}
			void Clear()
			{
				dict.Clear();
			}
			bool Add(const T& obj)
			{
				return dict.AddIfNotExists(obj, _DummyClass());
			}
			bool Add(T && obj)
			{
				return dict.AddIfNotExists(_Move(obj), _DummyClass());
			}
			void Remove(const T & obj)
			{
				dict.Remove(obj);
			}
			bool Contains(const T & obj) const
			{
				return dict.ContainsKey(obj);
			}
		};
		template <typename T>
		class HashSet : public HashSetBase<T, Dictionary<T, _DummyClass>>
		{};

		template <typename T>
		class EnumerableHashSet : public HashSetBase<T, EnumerableDictionary<T, _DummyClass>>
		{};
	}
}

#endif

/***********************************************************************
FUNC.H
***********************************************************************/
#ifndef CORELIB_FUNC_H
#define CORELIB_FUNC_H


namespace CoreLib
{
	namespace Basic
	{
		template<typename TResult, typename... Arguments>
		class FuncPtr
		{
		public:
			virtual TResult operator()(Arguments...) = 0;
			virtual bool operator == (const FuncPtr<TResult, Arguments...> *)
			{
				return false;
			}
			virtual ~FuncPtr() {}
		};

		template<typename TResult, typename... Arguments>
		class CdeclFuncPtr : public FuncPtr<TResult, Arguments...>
		{
		public:
			typedef TResult (*FuncType)(Arguments...);
		private:
			FuncType funcPtr;
		public:
			CdeclFuncPtr(FuncType func)
				:funcPtr(func)
			{
			}

			virtual TResult operator()(Arguments... params) override
			{
				return funcPtr(params...);
			}

			virtual bool operator == (const FuncPtr<TResult, Arguments...> * ptr) override
			{
				auto cptr = dynamic_cast<const CdeclFuncPtr<TResult, Arguments...>*>(ptr);
				if (cptr)
					return funcPtr == cptr->funcPtr;
				else
					return false;
			}
		};

		template<typename Class, typename TResult, typename... Arguments>
		class MemberFuncPtr : public FuncPtr<TResult, Arguments...>
		{
		public:
			typedef TResult (Class::*FuncType)(Arguments...);
		private:
			FuncType funcPtr;
			Class * object;
		public:
			MemberFuncPtr(Class * obj, FuncType func)
				: funcPtr(func), object(obj)
			{
			}

			virtual TResult operator()(Arguments... params) override
			{
				return (object->*funcPtr)(params...);
			}

			virtual bool operator == (const FuncPtr<TResult, Arguments...> * ptr) override
			{
				auto cptr = dynamic_cast<const MemberFuncPtr<Class, TResult, Arguments...>*>(ptr);
				if (cptr)
					return funcPtr == cptr->funcPtr && object == cptr->object;
				else
					return false;
			}
		};

		template<typename F, typename TResult, typename... Arguments>
		class LambdaFuncPtr : public FuncPtr<TResult, Arguments...>
		{
		private:
			F func;
		public:
			LambdaFuncPtr(const F & _func)
				: func(_func)
			{}
			virtual TResult operator()(Arguments... params) override
			{
				return func(params...);
			}
			virtual bool operator == (const FuncPtr<TResult, Arguments...> * /*ptr*/) override
			{
				return false;
			}
		};

		template<typename TResult, typename... Arguments>
		class Func
		{
		private:
			RefPtr<FuncPtr<TResult, Arguments...>> funcPtr;
		public:
			Func(){}
			Func(typename CdeclFuncPtr<TResult, Arguments...>::FuncType func)
			{
				funcPtr = new CdeclFuncPtr<TResult, Arguments...>(func);
			}
			template<typename Class>
			Func(Class * object, typename MemberFuncPtr<Class, TResult, Arguments...>::FuncType func)
			{
				funcPtr = new MemberFuncPtr<Class, TResult, Arguments...>(object, func);
			}
			template<typename TFuncObj>
			Func(const TFuncObj & func)
			{
				funcPtr = new LambdaFuncPtr<TFuncObj, TResult, Arguments...>(func);
			}
			Func & operator = (typename CdeclFuncPtr<TResult, Arguments...>::FuncType func)
			{
				funcPtr = new CdeclFuncPtr<TResult, Arguments...>(func);
				return *this;
			}
			template<typename Class>
			Func & operator = (const MemberFuncPtr<Class, TResult, Arguments...> & func)
			{
				funcPtr = new MemberFuncPtr<Class, TResult, Arguments...>(func);
				return *this;
			}
			template<typename TFuncObj>
			Func & operator = (const TFuncObj & func)
			{
				funcPtr = new LambdaFuncPtr<TFuncObj, TResult, Arguments...>(func);
				return *this;
			}
			TResult operator()(Arguments... params)
			{
				return (*funcPtr)(params...);
			}
		};

		// template<typename... Arguments>
		// using Procedure = Func<void, Arguments...>;

		template<typename... Arguments>
		class Procedure : public Func<void, Arguments...>
		{
		private:
			RefPtr<FuncPtr<void, Arguments...>> funcPtr;
		public:
			Procedure(){}
			Procedure(const Procedure & proc)
			{
				funcPtr = proc;
			}
			Procedure(typename CdeclFuncPtr<void, Arguments...>::FuncType func)
			{
				funcPtr = new CdeclFuncPtr<void, Arguments...>(func);
			}
			template<typename Class>
			Procedure(Class * object, typename MemberFuncPtr<Class, void, Arguments...>::FuncType func)
			{
				funcPtr = new MemberFuncPtr<Class, void, Arguments...>(object, func);
			}
			template<typename TFuncObj>
			Procedure(const TFuncObj & func)
			{
				funcPtr = new LambdaFuncPtr<TFuncObj, void, Arguments...>(func);
			}
			Procedure & operator = (typename CdeclFuncPtr<void, Arguments...>::FuncType func)
			{
				funcPtr = new CdeclFuncPtr<void, Arguments...>(func);
				return *this;
			}
			template<typename Class>
			Procedure & operator = (const MemberFuncPtr<Class, void, Arguments...> & func)
			{
				funcPtr = new MemberFuncPtr<Class, void, Arguments...>(func);
				return *this;
			}
			template<typename TFuncObj>
			Procedure & operator = (const TFuncObj & func)
			{
				funcPtr = new LambdaFuncPtr<TFuncObj, void, Arguments...>(func);
				return *this;
			}
			Procedure & operator = (const Procedure & proc)
			{
				funcPtr = proc.funcPtr;
			}
			void Clear()
			{
				funcPtr = nullptr;
			}
			void operator()(Arguments... params)
			{
				if (funcPtr)
					(*funcPtr)(params...);
			}
		};
	}
}

#endif

/***********************************************************************
LINQ.H
***********************************************************************/
#ifndef FUNDAMENTAL_LIB_LINQ_H
#define FUNDAMENTAL_LIB_LINQ_H


namespace CoreLib
{
	namespace Basic
	{

		template <typename T>
		T ConstructT();

		template <typename T>
		class RemoveReference
		{
		public:
			typedef T Type;
		};

		template <typename T>
		class RemoveReference<T&>
		{
		public:
			typedef T Type;
		};

		template <typename T>
		class RemoveReference<T&&>
		{
		public:
			typedef T Type;
		};

		template<typename T>
		struct RemovePointer
		{
			typedef T Type;
		};

		template<typename T>
		struct RemovePointer<T*>
		{
			typedef T Type;
		};

		template <typename TQueryable, typename TEnumerator, typename T, typename TFunc>
		class WhereQuery
		{
		private:
			TQueryable items;
			TFunc func;
		public:
			WhereQuery(const TQueryable & queryable, const TFunc & f)
				: items(queryable), func(f)
			{}
			class Enumerator
			{
			private:
				TEnumerator ptr;
				TEnumerator end;
				TFunc *func;
			public:
				Enumerator(const Enumerator &) = default;
				Enumerator(TEnumerator ptr, TEnumerator end, TFunc & f)
					: ptr(ptr), end(end), func(&f)
				{}
				T operator *() const
				{
					return *(ptr);
				}
				Enumerator& operator ++()
				{
					++ptr;
					while (ptr != end)
					{
						if ((*func)(*ptr))
							break;
						else
							++ptr;
					}
					return *this;
				}
				Enumerator operator ++(int)
				{
					Enumerator rs = *this;
					while (rs.ptr != end)
					{
						if ((*func)(*rs.ptr))
							break;
						++rs.ptr;
					}
					return rs;
				}
				bool operator != (const Enumerator & iter) const
				{
					return ptr != iter.ptr;
				}
				bool operator == (const Enumerator & iter) const
				{
					return ptr == iter.ptr;
				}
			};
			Enumerator begin()
			{
				auto ptr = items.begin();
				auto end = items.end();
				while (ptr != end)
				{
					if (func(*ptr))
						break;
					++ptr;
				}
				return Enumerator(ptr, end, func);
			}
			Enumerator end()
			{
				return Enumerator(items.end(), items.end(), func);
			}
		};

		template <typename TQueryable, typename TEnumerator, typename T, typename TFunc>
		class SelectQuery
		{
		private:
			TQueryable items;
			TFunc func;
		public:
			SelectQuery(const TQueryable & queryable, const TFunc & f)
				: items(queryable), func(f)
			{}
			class Enumerator
			{
			private:
				TEnumerator ptr;
				TEnumerator end;
				TFunc *func;
			public:
				Enumerator(const Enumerator &) = default;
				Enumerator(TEnumerator ptr, TEnumerator end, TFunc & f)
					: ptr(ptr), end(end), func(&f)
				{}
				auto operator *() const -> decltype((*func)(*ptr))
				{
					return (*func)(*ptr);
				}
				Enumerator& operator ++()
				{
					++ptr;
					return *this;
				}
				Enumerator operator ++(int)
				{
					Enumerator rs = *this;
					++rs;
					return rs;
				}
				bool operator != (const Enumerator & iter) const
				{
					return !(ptr == iter.ptr);
				}
				bool operator == (const Enumerator & iter) const
				{
					return ptr == iter.ptr;
				}
			};
			Enumerator begin()
			{
				return Enumerator(items.begin(), items.end(), func);
			}
			Enumerator end()
			{
				return Enumerator(items.end(), items.end(), func);
			}
		};

		template <typename TQueryable, typename TEnumerator, typename T, typename TFunc>
		class SelectManyQuery
		{
		private:
			TQueryable items;
			TFunc func;
			SelectManyQuery()
			{}
		public:
			SelectManyQuery(const TQueryable & queryable, const TFunc & f)
				: items(queryable), func(f)
			{}
			template<typename TItems, typename TItemPtr>
			class Enumerator
			{
			private:
				TEnumerator ptr;
				TEnumerator end;
				TFunc &func;
				TItems items;
				TItemPtr subPtr;
			public:
				Enumerator(const Enumerator &) = default;
				Enumerator(TEnumerator ptr, TEnumerator end, TFunc & f)
					: ptr(ptr), end(end), func(f)
				{
					if (ptr != end)
					{
						items = f(*ptr);
						subPtr = items.begin();
					}
				}
				auto operator *() const -> decltype(*subPtr)
				{
					return *subPtr;
				}
				Enumerator& operator ++()
				{
					++subPtr;
					while (subPtr == items.end() && ptr != end)
					{
						++ptr;
						if (ptr != end)
						{
							items = func(*ptr);
							subPtr = items.begin();
						}
						else
							break;
					}
					
					return *this;
				}
				Enumerator operator ++(int)
				{
					Enumerator rs = *this;
					++rs;
					return rs;
				}
				bool operator != (const Enumerator & iter) const
				{
					return !operator==(iter);
				}
				bool operator == (const Enumerator & iter) const
				{
					if (ptr == iter.ptr)
					{
						if (ptr == end)
							return true;
						else
							return subPtr == iter.subPtr;
					}
					else
						return false;
				}
			};
			auto begin()->Enumerator<decltype(func(ConstructT<T>())), decltype(func(ConstructT<T>()).begin())>
			{
				return Enumerator<decltype(func(ConstructT<T>())), decltype(func(ConstructT<T>()).begin())>(items.begin(), items.end(), func);
			}
			auto end()->Enumerator<decltype(func(ConstructT<T>())), decltype(func(ConstructT<T>()).begin())>
			{
				return Enumerator<decltype(func(ConstructT<T>())), decltype(func(ConstructT<T>()).begin())>(items.end(), items.end(), func);
			}
		};

		template <typename T>
		struct EnumeratorType
		{
			typedef decltype(ConstructT<T>().begin()) Type;
		};

		template <typename TFunc, typename TArg>
		class ExtractReturnType
		{
		public:
			static TFunc * f;
			static TArg ConstructArg(){};
			typedef decltype((*f)(ConstructArg())) ReturnType;
		};

		template <typename T>
		class ExtractItemType
		{
		public:
			typedef typename RemovePointer<decltype(ConstructT<T>().begin())>::Type Type;
		};

		template <typename TQueryable, typename TEnumerator, typename T>
		class Queryable
		{
		private:
			TQueryable items;
		public:
			auto begin() -> decltype(items.begin())
			{
				return items.begin();
			}
			auto end() -> decltype(items.end())
			{
				return items.end();
			}
		public:
			Queryable(const TQueryable & items)
				: items(items)
			{}

			template<typename TFunc>
			Queryable<WhereQuery<TQueryable, TEnumerator, T, TFunc>, typename WhereQuery<TQueryable, TEnumerator, T, TFunc>::Enumerator, T> Where(const TFunc & f)
			{
				return Queryable<WhereQuery<TQueryable, TEnumerator, T, TFunc>, typename WhereQuery<TQueryable, TEnumerator, T, TFunc>::Enumerator, T>(WhereQuery<TQueryable, TEnumerator, T, TFunc>(items, f));
			}

			template<typename TFunc>
			Queryable<SelectQuery<TQueryable, TEnumerator, T, TFunc>, typename SelectQuery<TQueryable, TEnumerator, T, TFunc>::Enumerator, typename RemoveReference<typename ExtractReturnType<TFunc, T>::ReturnType>::Type> Select(const TFunc & f)
			{
				return Queryable<SelectQuery<TQueryable, TEnumerator, T, TFunc>, typename SelectQuery<TQueryable, TEnumerator, T, TFunc>::Enumerator, typename RemoveReference<typename ExtractReturnType<TFunc, T>::ReturnType>::Type>(SelectQuery<TQueryable, TEnumerator, T, TFunc>(items, f));
			}

			template<typename TFunc>
			auto SelectMany(const TFunc & f) ->Queryable<SelectManyQuery<TQueryable, TEnumerator, T, TFunc>, typename EnumeratorType<SelectManyQuery<TQueryable, TEnumerator, T, TFunc>>::Type, typename ExtractItemType<decltype(f(ConstructT<T>()))>::Type>
			{
				return Queryable<SelectManyQuery<TQueryable, TEnumerator, T, TFunc>, typename EnumeratorType<SelectManyQuery<TQueryable, TEnumerator, T, TFunc>>::Type, typename ExtractItemType<decltype(f(ConstructT<T>()))>::Type>(SelectManyQuery<TQueryable, TEnumerator, T, TFunc>(items, f));
			}

			template<typename TAggregateResult, typename TFunc>
			auto Aggregate(const TAggregateResult & initial, const TFunc & f) -> decltype(f(initial, *items.begin()))
			{
				TAggregateResult rs = initial;
				for (auto && x : items)
					rs = f(rs, x);
				return rs;
			}

			template<typename TFunc>
			T & First(const TFunc & condition)
			{
				for (auto && x : items)
					if (condition(x))
						return x;
			}

			template <typename TFunc>
			T Max(const TFunc & selector)
			{
				return Aggregate(*items.begin(), [&](const T & v0, const T & v1)
				{
					return selector(v0) > selector(v1) ? v0 : v1; 
				});
			}

			template <typename TFunc>
			T Min(const TFunc & selector)
			{
				return Aggregate(*items.begin(), [&](const T & v0, const T & v1)
				{
					return selector(v0) < selector(v1) ? v0 : v1;
				});
			}

			template <typename TFunc>
			auto Sum(const TFunc & selector) -> decltype(selector(ConstructT<T>()))
			{
				decltype(selector(ConstructT<T>())) rs(0);
				for (auto && x : items)
					rs = rs + selector(x);
				return rs;
			}

			T Max()
			{
				return Aggregate(*items.begin(), [](const T & v0, const T & v1) {return v0 > v1 ? v0 : v1; });
			}

			T Min()
			{
				return Aggregate(*items.begin(), [](const T & v0, const T & v1) {return v0 < v1 ? v0 : v1; });
			}

			T Sum()
			{
				T rs = T(0);
				for (auto && x : items)
					rs = rs + x;
				return rs;
			}

			T Avg()
			{
				T rs = T(0);
				int count = 0;
				for (auto && x : items)
				{
					rs = rs + x;
					count++;
				}
				return rs / count;
			}

			int Count()
			{
				int rs = 0;
				for (auto && x : items)
					rs++;
				return rs;
			}

			List<T> ToList()
			{
				List<T> rs;
				for (auto && val : items)
					rs.Add(val);
				return rs;
			}
		};


		template<typename T, typename TAllocator>
		inline Queryable<ArrayView<T>, T*, T> AsQueryable(const List<T, TAllocator> & list)
		{
			return Queryable<ArrayView<T>, T*, T>(list.GetArrayView());
		}

		template<typename T>
		inline Queryable<ArrayView<T>, T*, T> AsQueryable(const ArrayView<T> & list)
		{
			return Queryable<ArrayView<T>, T*, T>(list);
		}

		
		template<typename T>
		struct LinkedListView
		{
			typename LinkedList<T>::Iterator start, last;
			typename LinkedList<T>::Iterator begin()
			{
				return start;
			}
			typename LinkedList<T>::Iterator end()
			{
				return last;
			}
		};

		template<typename T>
		inline Queryable<LinkedListView<T>, LinkedNode<T>, T> AsQueryable(const LinkedList<T> & list)
		{
			LinkedListView<T> view;
			view.start = list.begin();
			view.last = list.end();
			return Queryable<LinkedListView<T>, LinkedNode<T>, T>(view);
		}

		template<typename TKey, typename TValue>
		struct EnumerableDictView
		{
			typename EnumerableDictionary<TKey, TValue>::Iterator start, last;
			typename EnumerableDictionary<TKey, TValue>::Iterator begin()
			{
				return start;
			}
			typename EnumerableDictionary<TKey, TValue>::Iterator end()
			{
				return last;
			}
		};

		template<typename TKey, typename TValue>
		inline Queryable<EnumerableDictView<TKey, TValue>, typename EnumerableDictionary<TKey, TValue>::Iterator, KeyValuePair<TKey, TValue>> AsQueryable(const EnumerableDictionary<TKey, TValue> & dict)
		{
			EnumerableDictView<TKey, TValue> view;
			view.start = dict.begin();
			view.last = dict.end();
			return Queryable<EnumerableDictView<TKey, TValue>, typename EnumerableDictionary<TKey, TValue>::Iterator, KeyValuePair<TKey, TValue>>(view);
		}

		template<typename TKey>
		struct EnumerableHashSetView
		{
			typename HashSetBase<TKey, EnumerableDictionary<TKey, _DummyClass>>::Iterator start, last;
			typename EnumerableHashSet<TKey>::Iterator begin()
			{
				return start;
			}
			typename EnumerableHashSet<TKey>::Iterator end()
			{
				return last;
			}
		};

		template<typename TKey>
		inline Queryable<EnumerableHashSetView<TKey>, typename HashSetBase<TKey, EnumerableDictionary<TKey, _DummyClass>>::Iterator, TKey> AsQueryable(const EnumerableHashSet<TKey> & dict)
		{
			EnumerableHashSetView<TKey> view;
			view.start = dict.begin();
			view.last = dict.end();
			return Queryable<EnumerableHashSetView<TKey>, typename HashSetBase<TKey, EnumerableDictionary<TKey, _DummyClass>>::Iterator, TKey>(view);
		}
	}
}

#endif

/***********************************************************************
BASIC.H
***********************************************************************/
#ifndef CORE_LIB_BASIC_H
#define CORE_LIB_BASIC_H


namespace CoreLib
{
	using namespace Basic;
}

#endif

/***********************************************************************
STREAM.H
***********************************************************************/
#ifndef CORE_LIB_STREAM_H
#define CORE_LIB_STREAM_H


namespace CoreLib
{
	namespace IO
	{
		using CoreLib::Basic::Exception;
		using CoreLib::Basic::String;
		using CoreLib::Basic::RefPtr;

		class IOException : public Exception
		{
		public:
			IOException()
			{}
			IOException(const String & message)
				: CoreLib::Basic::Exception(message)
			{
			}
		};

		class EndOfStreamException : public IOException
		{
		public:
			EndOfStreamException()
			{}
			EndOfStreamException(const String & message)
				: IOException(message)
			{
			}
		};

		enum class SeekOrigin
		{
			Start, End, Current
		};

		class Stream : public CoreLib::Basic::Object
		{
		public:
			virtual Int64 GetPosition()=0;
			virtual void Seek(SeekOrigin origin, Int64 offset)=0;
			virtual Int64 Read(void * buffer, Int64 length) = 0;
			virtual Int64 Write(const void * buffer, Int64 length) = 0;
			virtual bool IsEnd() = 0;
			virtual bool CanRead() = 0;
			virtual bool CanWrite() = 0;
			virtual void Close() = 0;
		};

		class BinaryReader
		{
		private:
			RefPtr<Stream> stream;
		public:
			BinaryReader(RefPtr<Stream> stream)
			{
				this->stream = stream;
			}
			Stream * GetStream()
			{
				return stream.Ptr();
			}
			void ReleaseStream()
			{
				stream.Release();
			}
			template<typename T>
			void Read(T * buffer, int count)
			{
				stream->Read(buffer, sizeof(T)*(Int64)count);
			}
			int ReadInt32()
			{
				int rs;
				stream->Read(&rs, sizeof(int));
				return rs;
			}
			short ReadInt16()
			{
				short rs;
				stream->Read(&rs, sizeof(short));
				return rs;
			}
			Int64 ReadInt64()
			{
				Int64 rs;
				stream->Read(&rs, sizeof(Int64));
				return rs;
			}
			float ReadFloat()
			{
				float rs;
				stream->Read(&rs, sizeof(float));
				return rs;
			}
			double ReadDouble()
			{
				double rs;
				stream->Read(&rs, sizeof(double));
				return rs;
			}
			char ReadChar()
			{
				char rs;
				stream->Read(&rs, sizeof(char));
				return rs;
			}
			String ReadString()
			{
				int len = ReadInt32();
				wchar_t * buffer = new wchar_t[len+1];
				try
				{
					stream->Read(buffer, sizeof(wchar_t)*len);
				}
				catch(IOException & e)
				{
					delete [] buffer;
					throw e;
				}
				buffer[len] = 0;
				return String::FromBuffer(buffer, len);
			}
		};

		class BinaryWriter
		{
		private:
			RefPtr<Stream> stream;
		public:
			BinaryWriter(RefPtr<Stream> stream)
			{
				this->stream = stream;
			}
			Stream * GetStream()
			{
				return stream.Ptr();
			}
			template<typename T>
			void Write(const T& val)
			{
				stream->Write(&val, sizeof(T));
			}
			template<typename T>
			void Write(T * buffer, int count)
			{
				stream->Write(buffer, sizeof(T)*(Int64)count);
			}
			void Write(const String & str)
			{
				Write(str.Length());
				Write(str.Buffer(), str.Length());
			}
			void ReleaseStream()
			{
				stream.Release();
			}
			void Close()
			{
				stream->Close();
			}
		};

		enum class FileMode
		{
			Create, Open, CreateNew, Append
		};

		enum class FileAccess
		{
			Read = 1, Write = 2, ReadWrite = 3
		};

		enum class FileShare
		{
			None, ReadOnly, WriteOnly, ReadWrite
		};

		class FileStream : public Stream
		{
		private:
			FILE * handle;
			FileAccess fileAccess;
			bool endReached = false;
			void Init(const CoreLib::Basic::String & fileName, FileMode fileMode, FileAccess access, FileShare share);
		public:
			FileStream(const CoreLib::Basic::String & fileName, FileMode fileMode = FileMode::Open);
			FileStream(const CoreLib::Basic::String & fileName, FileMode fileMode, FileAccess access, FileShare share);
			~FileStream();
		public:
			virtual Int64 GetPosition();
			virtual void Seek(SeekOrigin origin, Int64 offset);
			virtual Int64 Read(void * buffer, Int64 length);
			virtual Int64 Write(const void * buffer, Int64 length);
			virtual bool CanRead();
			virtual bool CanWrite();
			virtual void Close();
			virtual bool IsEnd();
		};
	}
}

#endif

/***********************************************************************
TEXTIO.H
***********************************************************************/
#ifndef CORE_LIB_TEXT_IO_H
#define CORE_LIB_TEXT_IO_H


namespace CoreLib
{
	namespace IO
	{
		using CoreLib::Basic::List;
		using CoreLib::Basic::_EndLine;

		class TextReader : public CoreLib::Basic::Object
		{
		private:
			wchar_t lookAhead = 0;
			bool hasLookAhead = false;
		protected:
			virtual wchar_t ReadChar() = 0;
		public:
			~TextReader()
			{
				Close();
			}
			virtual void Close(){}
			virtual String ReadLine()=0;
			virtual String ReadToEnd()=0;
			virtual bool IsEnd() = 0;
			int Read(wchar_t * buffer, int count);
			wchar_t Read()
			{
				if (!hasLookAhead)
					return ReadChar();
				else
				{
					hasLookAhead = false;
					return lookAhead;
				}
			}
			wchar_t Peak()
			{
				if (hasLookAhead)
					return lookAhead;
				lookAhead = Read();
				hasLookAhead = true;
				return lookAhead;
			}
		};

		class TextWriter : public CoreLib::Basic::Object
		{
		public:
			~TextWriter()
			{
				Close();
			}
			virtual void Write(const String & str)=0;
			virtual void Write(const wchar_t * str)=0;
			virtual void Write(const char * str)=0;
			virtual void Close(){}
			template<typename T>
			TextWriter & operator << (const T& val)
			{
				Write(val.ToString());
				return *this;
			}
			TextWriter & operator << (wchar_t value)
			{
				Write(String(value));
				return *this;
			}
			TextWriter & operator << (int value)
			{
				Write(String(value));
				return *this;
			}
			TextWriter & operator << (float value)
			{
				Write(String(value));
				return *this;
			}
			TextWriter & operator << (double value)
			{
				Write(String(value));
				return *this;
			}
			TextWriter & operator << (const char* value)
			{
				Write(value);
				return *this;
			}
			TextWriter & operator << (const wchar_t * const val)
			{
				Write(val);
				return *this;
			}
			TextWriter & operator << (wchar_t * const val)
			{
				Write(val);
				return *this;
			}
			TextWriter & operator << (const String & val)
			{
				Write(val);
				return *this;
			}
			TextWriter & operator << (const _EndLine &)
			{
#ifdef _WIN32
				Write(L"\r\n");
#else
				Write(L"\n");
#endif
				return *this;
			}
		};

		class Encoding
		{
		public:
			static Encoding * UTF8, * Ansi, * UTF16, *UTF16Reversed;
			virtual void GetBytes(List<char> & buffer, const String & str)=0;
			virtual ~Encoding()
			{}
		};

		class StreamWriter : public TextWriter
		{
		private:
			List<char> encodingBuffer;
			RefPtr<Stream> stream;
			Encoding * encoding;
		public:
			StreamWriter(const String & path, Encoding * encoding = Encoding::UTF8);
			StreamWriter(RefPtr<Stream> stream, Encoding * encoding = Encoding::UTF8);
			virtual void Write(const String & str);
			virtual void Write(const wchar_t * str);
			virtual void Write(const char * str);
			virtual void Close()
			{
				stream->Close();
			}
		};

		class StreamReader : public TextReader
		{
		private:
			wchar_t lowSurrogate = 0;
			bool hasLowSurrogate = false;
			RefPtr<Stream> stream;
			List<char> buffer;
			Encoding * encoding;
			int ptr;
			char ReadBufferChar();
			void ReadBuffer();
			template<typename GetFunc>
			wchar_t GetChar(GetFunc get)
			{
				wchar_t decoded = 0;
				if (encoding == Encoding::UTF8)
				{
					if (hasLowSurrogate)
					{
						hasLowSurrogate = false;
						return lowSurrogate;
					}
					int codePoint = 0;
					int leading = get(0);
					int mask = 0b10000000;
					int count = 0;
					while (leading & mask)
					{
						count++;
						mask >>= 1;
					}
					codePoint = (leading & (mask - 1));
					for (int i = 1; i <= count - 1; i++)
					{
						codePoint <<= 6;
						codePoint += (get(i) & 0b111111);
					}
#ifdef _WIN32
					if (codePoint <= 0xD7FF || (codePoint >= 0xE000 && codePoint <= 0xFFFF))
						return (wchar_t)codePoint;
					else
					{
						int sub = codePoint - 0x10000;
						int high = (sub >> 10) + 0xD800;
						int low = (sub & 0x3FF) + 0xDC00;
						hasLowSurrogate = true;
						lowSurrogate = (wchar_t)low;
						return (wchar_t)high;
					}
#else
					return (wchar_t)codePoint; // linux platforms use UTF32
#endif
				}
				else if (encoding == Encoding::UTF16)
				{
					decoded = get(0) + (get(1) << 8);
#ifndef _WIN32
					if (decoded >= 0xD800 && decoded <= 0xDBFF) // high surrogate detected
					{
						unsigned short lowSurrogate = get(2) + (get(3) << 8);
						decoded = ((decoded - 0xD800) << 10) + (lowSurrogate - 0xDC00);
					}
#endif
					return decoded;
				}
				else if (encoding == Encoding::UTF16Reversed)
				{
					decoded = (get(0) << 8) + get(1);
#ifndef _WIN32
					if (decoded >= 0xD800 && decoded <= 0xDBFF) // high surrogate detected
					{
						unsigned short lowSurrogate = (get(2) << 8) + get(3);
						decoded = ((decoded - 0xD800) << 10) + (lowSurrogate - 0xDC00);
					}
#endif
					return decoded;
				}
				else
				{
					return get(0);
				}
			}
			Encoding * DetermineEncoding();
		protected:
			virtual wchar_t ReadChar()
			{
				return GetChar([&](int) {return ReadBufferChar(); });
			}
		public:
			StreamReader(const String & path);
			StreamReader(RefPtr<Stream> stream, Encoding * encoding = nullptr);
			virtual String ReadLine();
			virtual String ReadToEnd();
			virtual bool IsEnd()
			{
				return ptr == buffer.Count() && stream->IsEnd();
			}
			virtual void Close()
			{
				stream->Close();
			}
		};
	}
}

#endif

/***********************************************************************
REGEX\REGEXTREE.H
***********************************************************************/
#ifndef GX_REGEX_PARSER_H
#define GX_REGEX_PARSER_H


namespace CoreLib
{
	namespace Text
	{
		using namespace CoreLib::Basic;

		class RegexCharSetNode;
		class RegexRepeatNode;
		class RegexConnectionNode;
		class RegexSelectionNode;

		class RegexNodeVisitor : public Object
		{
		public:
			virtual void VisitCharSetNode(RegexCharSetNode * node);
			virtual void VisitRepeatNode(RegexRepeatNode * node);
			virtual void VisitConnectionNode(RegexConnectionNode * node);
			virtual void VisitSelectionNode(RegexSelectionNode * node);
		};

		class RegexNode : public Object
		{
		public:
			virtual String Reinterpret() = 0;
			virtual void Accept(RegexNodeVisitor * visitor) = 0;
		};

		class RegexCharSet : public Object
		{
		private:
			List<RegexCharSet *> OriSet;
			void CopyCtor(const RegexCharSet & set);
		public:
			bool Neg;
			struct RegexCharRange
			{
				wchar_t Begin,End;
				bool operator == (const RegexCharRange & r);
			};
			List<RegexCharRange> Ranges;
			List<unsigned short> Elements; 
		
		public:
			RegexCharSet()
			{
				Neg = false;
			}
			RegexCharSet(const RegexCharSet & set);
			String Reinterpret();
			void Normalize();
			void Sort();
			void AddRange(RegexCharRange r);
			void SubtractRange(RegexCharRange r);
			bool Contains(RegexCharRange r);
			bool operator ==(const RegexCharSet & set);
			RegexCharSet & operator = (const RegexCharSet & set);
			static void InsertElement(List<RefPtr<RegexCharSet>> &L, RefPtr<RegexCharSet> & elem);
			static void RangeMinus(RegexCharRange r1, RegexCharRange r2, RegexCharSet & rs);
			static void CharSetMinus(RegexCharSet & s1, RegexCharSet & s2);
			static void RangeIntersection(RegexCharRange r1, RegexCharRange r2, RegexCharSet &rs);
			static void CalcCharElementFromPair(RegexCharSet * c1, RegexCharSet * c2, RegexCharSet & AmB, RegexCharSet & BmA, RegexCharSet & AnB);
			static void CalcCharElements(List<RegexCharSet *> & sets, List<RegexCharRange> & elements);
		};

		class RegexCharSetNode : public RegexNode
		{
		public:
			RefPtr<RegexCharSet> CharSet;
		public:
			String Reinterpret();
			virtual void Accept(RegexNodeVisitor * visitor);
			RegexCharSetNode();
		};

		class RegexRepeatNode : public RegexNode
		{
		public:
			enum _RepeatType
			{
				rtOptional, rtArbitary, rtMoreThanOnce, rtSpecified
			};
			_RepeatType RepeatType = rtOptional;
			int MinRepeat = -1, MaxRepeat = -1;
			RefPtr<RegexNode> Child;
		public:
			String Reinterpret();
			virtual void Accept(RegexNodeVisitor * visitor);
		};

		class RegexConnectionNode : public RegexNode
		{
		public:
			RefPtr<RegexNode> LeftChild, RightChild;
		public:
			String Reinterpret();
			virtual void Accept(RegexNodeVisitor * visitor);
		};

		class RegexSelectionNode : public RegexNode
		{
		public:
			RefPtr<RegexNode> LeftChild, RightChild;
		public:
			String Reinterpret();
			virtual void Accept(RegexNodeVisitor * visitor);
		};

		class RegexParser : public Object
		{
		private:
			String src;
			int ptr;
			RegexNode * ParseSelectionNode();
			RegexNode * ParseConnectionNode();
			RegexNode * ParseRepeatNode();
			RegexCharSet * ParseCharSet();
			wchar_t ReadNextCharInCharSet();
			int ParseInteger();
			bool IsOperator();
		public:
			struct SyntaxError
			{
				int Position;
				String Text;
			};
			List<SyntaxError> Errors;
			RefPtr<RegexNode> Parse(const String & regex); 
		};
	}
}

#endif

/***********************************************************************
REGEX\REGEXNFA.H
***********************************************************************/
#ifndef REGEX_NFA_H
#define REGEX_NFA_H


namespace CoreLib
{
	namespace Text
	{
		using namespace CoreLib::Basic;

		class NFA_Node;

		class NFA_Translation : public Object
		{
		public:
			RefPtr<RegexCharSet> CharSet;
			NFA_Node * NodeSrc, * NodeDest;
			NFA_Translation();
			NFA_Translation(NFA_Node * src, NFA_Node * dest, RefPtr<RegexCharSet> charSet);
			NFA_Translation(NFA_Node * src, NFA_Node * dest);
		};

		class NFA_Node : public Object
		{
		private:
			static int HandleCount;
		public:
			int ID = -1;
			bool Flag = 0;
			bool IsFinal = false;
			int TerminalIdentifier;
			List<NFA_Translation *> Translations;
			List<NFA_Translation *> PrevTranslations;
			void RemoveTranslation(NFA_Translation * trans);
			void RemovePrevTranslation(NFA_Translation * trans);
			NFA_Node();
		};

		class NFA_Graph : public RegexNodeVisitor
		{
			friend class DFA_Graph;
		private:
			NFA_Node * start = nullptr, * end = nullptr;
			struct NFA_StatePair
			{
				NFA_Node * start = nullptr, * end = nullptr;
			};
			List<NFA_StatePair> stateStack;
			NFA_StatePair PopState();
			void PushState(NFA_StatePair s);
		private:
			List<RefPtr<NFA_Node>> nodes;
			List<RefPtr<NFA_Translation>> translations;
			void ClearNodes();
			void ClearNodeFlags();
			void GetValidStates(List<NFA_Node *> & states);
			void GetEpsilonClosure(NFA_Node * node, List<NFA_Node *> & states);
			void EliminateEpsilon();
		public:
			NFA_Node * CreateNode();
			NFA_Translation * CreateTranslation();
			String Interpret();
			void GenerateFromRegexTree(RegexNode * tree, bool elimEpsilon = true);
			void PostGenerationProcess();
			void CombineNFA(NFA_Graph * graph);
			NFA_Node * GetStartNode();
			void SetStartNode(NFA_Node * node);
			void SetTerminalIdentifier(int id);
			virtual void VisitCharSetNode(RegexCharSetNode * node);
			virtual void VisitRepeatNode(RegexRepeatNode * node);
			virtual void VisitConnectionNode(RegexConnectionNode * node);
			virtual void VisitSelectionNode(RegexSelectionNode * node);
		};
	}
}


#endif

/***********************************************************************
REGEX\REGEXDFA.H
***********************************************************************/
#ifndef REGEX_DFA_H
#define REGEX_DFA_H


namespace CoreLib
{
	namespace Text
	{
		using namespace CoreLib::Basic;

		typedef List<Word> RegexCharTable;
	
		class CharTableGenerator : public Object
		{
		private:
			List<String> sets;
			RegexCharTable * table;
			int AddSet(String set);
		public:
			List<RegexCharSet::RegexCharRange> elements;
			CharTableGenerator(RegexCharTable * _table);
			int Generate(List<RegexCharSet *> & charSets);
		};

		class DFA_Table_Tag
		{
		public:
			bool IsFinal = false;
			List<int> TerminalIdentifiers; // sorted
			DFA_Table_Tag();
		};

		class DFA_Table : public Object
		{
		public:
			int StateCount;
			int AlphabetSize;
			int ** DFA;
			List<RefPtr<DFA_Table_Tag>> Tags;
			int StartState;
			RefPtr<RegexCharTable> CharTable;
			DFA_Table();
			~DFA_Table();
		};

		class DFA_Node : public Object
		{
		public:
			int ID = -1;
			bool IsFinal = false;
			List<int> TerminalIdentifiers; // sorted
			List<NFA_Node*> Nodes;  // sorted
			List<DFA_Node *> Translations;
			DFA_Node(int elements);
			bool operator == (const DFA_Node & node);
		};

		class DFA_Graph : public Object
		{
		private:
			List<RegexCharSet::RegexCharRange> CharElements;
			RefPtr<RegexCharTable> table;
			DFA_Node * startNode;
			List<RefPtr<DFA_Node>> nodes;
			void CombineCharElements(NFA_Node * node, List<Word> & elem);
		public:
			void Generate(NFA_Graph * nfa);
			String Interpret();
			void ToDfaTable(DFA_Table * dfa);
		};
	}
}
#endif

/***********************************************************************
REGEX\REGEX.H
***********************************************************************/
#ifndef GX_REGEX_H
#define GX_REGEX_H


namespace CoreLib
{
	namespace Text
	{
		class IllegalRegexException : public Exception
		{
		};

		class RegexMatcher : public Object
		{
		private:
			DFA_Table * dfa;
		public:
			RegexMatcher(DFA_Table * table);
			int Match(const String & str, int startPos = 0);
		};

		class PureRegex : public Object
		{
		private:
			RefPtr<DFA_Table> dfaTable;
		public:
			struct RegexMatchResult
			{
				int Start;
				int Length;
			};
			PureRegex(const String & regex);
			bool IsMatch(const String & str); // Match Whole Word
			RegexMatchResult Search(const String & str, int startPos = 0);
			DFA_Table * GetDFA();
		};
	}
}

#endif

/***********************************************************************
REGEX\METALEXER.H
***********************************************************************/
#ifndef GX_META_LEXER_H
#define GX_META_LEXER_H


namespace CoreLib
{
	namespace Text
	{
		class LexToken
		{
		public:
			String Str;
			int TypeID;
			int Position;
		};

		class LazyLexToken
		{
		public:
			int TypeID;
			int Position;
			int Length;
		};

		typedef LinkedList<LexToken> LexStream;
	
		class LexerError
		{
		public:
			String Text;
			int Position;
		};

		struct LexProfileToken
		{
			String str;
			enum LexProfileTokenType
			{
				Identifier,
				Equal,
				Regex
			} type;
		};

		typedef LinkedList<LexProfileToken> LexProfileTokenStream;
		typedef LinkedNode<LexProfileToken> LexProfileTokenNode;

		class LexicalParseException : public Exception
		{
		public:
			int Position;
			LexicalParseException(String str, int pos) : Exception(str), Position(pos)
			{}
		};

		class LazyLexStream
		{
		private:
			RefPtr<DFA_Table> dfa; 
			List<bool> *ignoreSet;
		public:
			String InputText;
			LazyLexStream() = default;
			LazyLexStream(String text, const RefPtr<DFA_Table> & dfa, List<bool> *ignoreSet)
				: dfa(dfa), ignoreSet(ignoreSet), InputText(text)
			{}
			inline DFA_Table * GetDFA()
			{
				return dfa.Ptr();
			}
			inline List<bool> & GetIgnoreSet()
			{
				return *ignoreSet;
			}
			class Iterator
			{
			public:
				int ptr, state, lastTokenPtr;
				LazyLexStream * stream;
				LazyLexToken currentToken;
				bool operator != (const Iterator & iter) const
				{
					return lastTokenPtr != iter.lastTokenPtr;
				}
				bool operator == (const Iterator & iter) const
				{
					return lastTokenPtr == iter.lastTokenPtr;
				}
				LazyLexToken * operator ->()
				{
					return &currentToken;
				}
				LazyLexToken operator *()
				{
					return currentToken;
				}
				Iterator & operator ++();
				Iterator operator ++(int)
				{
					Iterator rs = *this;
					this->operator++();
					return rs;
				}
			};
			Iterator begin()
			{
				Iterator iter;
				iter.ptr = 0;
				iter.lastTokenPtr = 0;
				iter.state = dfa->StartState;
				iter.stream = this;
				++iter;
				return iter;
			}
			Iterator end()
			{
				Iterator iter;
				iter.ptr = InputText.Length();
				iter.lastTokenPtr = -1;
				iter.state = dfa->StartState;
				iter.stream = this;
				return iter;
			}
		};

		class MetaLexer : public Object
		{
		private:
			RefPtr<DFA_Table> dfa;
			List<String> Regex;
			List<String> TokenNames;
			List<bool> Ignore;
			String ReadProfileToken(LexProfileTokenNode*n, LexProfileToken::LexProfileTokenType type);
			bool ParseLexProfile(const String &lex);
			void ConstructDFA();
		public:
			int TokensParsed;
			List<LexerError> Errors;
			MetaLexer(String LexProfile);
			MetaLexer();
			DFA_Table * GetDFA();
			String GetTokenName(int id);
			int GetRuleCount();
			void SetLexProfile(String lex);
			bool Parse(String str, LexStream & stream);
			LazyLexStream Parse(String str)
			{
				return LazyLexStream(str, dfa, &Ignore);
			}
		};
	}
}

#endif

/***********************************************************************
EVENTS.H
***********************************************************************/
#ifndef GX_EVENTS_H
#define GX_EVENTS_H

namespace CoreLib
{
	namespace Basic
	{
	/***************************************************************************

	Events.h

	Usage:

		class A
		{
		public:
			void EventHandler(int a)
			{
				cout<<endl<<"function of object handler invoked. a*a = ";
				cout<<a*a<<endl;
			}
		};

		class B
		{
		public:
			typedef gxEvent1<int> gxOnEvent;
		public:
			gxOnEvent OnEvent;
			void DoSomething()
			{
				OnEvent.Invoke(4);
			}
		};

		void FuncHandler()
		{
			cout<<"Function invoked."<<endl;
		}

		void main()
		{
			A a;
			B b;
			b.OnEvent.Bind(&a,&A::EventHandler);	
			b.OnEvent.Bind(FuncHandler);			
			b.DoSomething();
			b.OnEvent.Unbind(FuncHandler);			
			b.OnEvent.Unbind(&a,&A::EventHandler);
			b.DoSomething();                       
		}

	***************************************************************************/
		template <typename... Arguments>
		class Event
		{
		private:
			List<RefPtr<FuncPtr<void, Arguments... >>> Handlers;
			void Bind(FuncPtr<void, Arguments...> * fobj)
			{
				Handlers.Add(fobj);
			}
			void Unbind(FuncPtr<void, Arguments...> * fobj)
			{
				int id = -1;
				for (int i = 0; i < Handlers.Count(); i++)
				{
					if ((*Handlers[i]) == fobj)
					{
						id = i;
						break;
					}
				}
				if (id != -1)
				{
					Handlers[id] = 0;
					Handlers.Delete(id);				
				}
			}
		public:
			Event()
			{
			}
			Event(const Event & e)
			{
				operator=(e);
			}
			Event & operator = (const Event & e)
			{
				for (int i = 0; i < e.Handlers.Count(); i++)
					Handlers.Add(e.Handlers[i]->Clone());
				return *this;
			}
			template <typename Class>
			Event(Class * Owner, typename MemberFuncPtr<Class, void, Arguments...>::FuncType handler)
			{
				Bind(Owner, handler);
			}
			Event(typename CdeclFuncPtr<void, Arguments...>::FuncType f)
			{
				Bind(f);
			}
			template <typename TFunctor>
			Event(const TFunctor & func)
			{
				Bind(func);
			}
			template <typename Class>
			void Bind(Class * Owner, typename MemberFuncPtr<Class, void, Arguments...>::FuncType handler)
			{
				Handlers.Add(new MemberFuncPtr<Class, void, Arguments...>(Owner, handler));
			}
			template <typename Class>
			void Unbind(Class * Owner, typename MemberFuncPtr<Class, void, Arguments...>::FuncType handler)
			{
				MemberFuncPtr<Class, void, Arguments...> h(Owner, handler);
				Unbind(&h);
			}
			void Bind(typename CdeclFuncPtr<void, Arguments...>::FuncType f)
			{
				Bind(new CdeclFuncPtr<void, Arguments...>(f));
			}
			void Unbind(typename CdeclFuncPtr<void, Arguments...>::FuncType f)
			{
				CdeclFuncPtr<void, Arguments...> h(f);
				Unbind(&h);
			}
			template <typename TFunctor>
			void Bind(const TFunctor & func)
			{
				Handlers.Add(new LambdaFuncPtr<TFunctor, void, Arguments...>(func));
			}
			template <typename TFunctor>
			void Unbind(const TFunctor & func)
			{
				LambdaFuncPtr<TFunctor, void, Arguments...> h(func);
				Unbind(&h);
			}
			Event & operator += (typename CdeclFuncPtr<void, Arguments...>::FuncType f)
			{
				Bind(f);
				return *this;
			}
			Event & operator -= (typename CdeclFuncPtr<void, Arguments...>::FuncType f)
			{
				Unbind(f);
				return *this;
			}
			template <typename TFunctor>
			Event & operator += (const TFunctor & f)
			{
				Bind(f);
				return *this;
			}
			template <typename TFunctor>
			Event & operator -= (const TFunctor & f)
			{
				Unbind(f);
				return *this;
			}
			void Invoke(Arguments... params) const
			{
				for (int i = 0; i < Handlers.Count(); i++)
					Handlers[i]->operator()(params...);
			}
			void operator ()(Arguments... params) const
			{
				Invoke(params...);
			}
		};
	}
}

#endif

/***********************************************************************
VECTORMATH.H
***********************************************************************/
#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H
#include <random>
#include <cmath>
#include <xmmintrin.h>
#ifdef _M_X64
#define NO_SIMD_ASM
#endif
#if !defined(_MSC_VER) || defined(__clang__)
#define NO_SIMD_ASM
#endif
#ifndef NO_VECTOR_CONSTRUCTORS
#define NO_VECTOR_CONSTRUCTORS
#endif
namespace VectorMath
{
	using namespace CoreLib::Basic;
	const float PI = 3.1415926535f;
	const float Epsilon = 1e-4f;
	const int DefaultFloatUlps = 1024;
	inline float Clamp(float val, float vmin, float vmax)
	{
		return val>vmax?vmax:val<vmin?vmin:val;
	}
	inline bool FloatEquals(float A, float B, int maxUlps = DefaultFloatUlps)
	{
		int aInt = *(int*)&A;
		// Make aInt lexicographically ordered as a twos-complement int
		if (aInt < 0)
			aInt = 0x80000000 - aInt;
		// Make bInt lexicographically ordered as a twos-complement int
		int bInt = *(int*)&B;
		if (bInt < 0)
			bInt = 0x80000000 - bInt;
		int intDiff = abs(aInt - bInt);
		if (intDiff <= maxUlps)
			return true;
		return false;
	}
	inline bool FloatLarger(float A, float B, int maxUlps = DefaultFloatUlps)
	{
		return A>B && !FloatEquals(A,B,maxUlps);
	}
	inline bool FloatSmaller(float A, float B, int maxUlps = DefaultFloatUlps)
	{
		return A<B && !FloatEquals(A,B,maxUlps);
	}
	inline bool FloatSmallerOrEquals(float A, float B, int maxUlps = DefaultFloatUlps)
	{
		return A<B || FloatEquals(A, B, maxUlps);
	}
	inline bool FloatLargerOrEquals(float A, float B, int maxUlps = DefaultFloatUlps)
	{
		return A>B || FloatEquals(A, B, maxUlps);
	}

	template<typename T>
	inline T Max(T v1, T v2)
	{
		if (v1>v2) return v1; else return v2;
	}
	template<typename T>
	inline T Min(T v1, T v2)
	{
		if (v1<v2) return v1; else return v2;
	}

	class Vec4;
	class Vec2
	{
	public:
		float x, y;
#ifndef NO_VECTOR_CONSTRUCTORS
		Vec2() = default;
		Vec2(const Vec2 & v) = default;
		Vec2(float vx, float vy)
		{
			x = vx; y = vy;
		}
#endif
		static Vec2 Create(float f)
		{
			Vec2 rs;
			rs.x = rs.y = f;
			return rs;
		}
		static Vec2 Create(float vx, float vy)
		{
			Vec2 rs;
			rs.x = vx;	rs.y = vy;
			return rs;
		}
		inline void SetZero()
		{
			x = y = 0.0f;
		}
		static inline float Dot(const Vec2 & v0, const Vec2 & v1)
		{
			return v0.x * v1.x + v0.y * v1.y;
		}
		inline float & operator [] (int i)
		{
			return ((float*)this)[i];
		}
		inline Vec2 operator * (float s) const
		{
			Vec2 rs;
			rs.x = x * s;
			rs.y = y * s;
			return rs;
		}
		inline Vec2 operator * (const Vec2 &vin) const
		{
			Vec2 rs;
			rs.x = x * vin.x;
			rs.y = y * vin.y;
			return rs;
		}
		inline Vec2 operator + (const Vec2 &vin) const
		{
			Vec2 rs;
			rs.x = x + vin.x;
			rs.y = y + vin.y;
			return rs;
		}
		inline Vec2 operator - (const Vec2 &vin) const
		{
			Vec2 rs;
			rs.x = x - vin.x;
			rs.y = y - vin.y;
			return rs;
		}
		inline Vec2 & operator += (const Vec2 & vin)
		{
			x += vin.x;
			y += vin.y;
			return *this;
		}
		inline Vec2 & operator -= (const Vec2 & vin)
		{
			x -= vin.x;
			y -= vin.y;
			return *this;
		}
		Vec2 & operator = (float v)
		{
			x = y = v;
			return *this;
		}
		inline Vec2 & operator *= (float s)
		{
			x *= s;
			y *= s;
			return *this;
		}
		inline Vec2 & operator *= (const Vec2 & vin)
		{
			x *= vin.x;
			y *= vin.y;
			return *this;
		}
		inline Vec2 Normalize()
		{
			float len = sqrt(x*x + y*y);
			float invLen = 1.0f / len;
			Vec2 rs;
			rs.x = x * invLen;
			rs.y = y * invLen;
			return rs;
		}
		inline float Length()
		{
			return sqrt(x*x + y*y);
		}
	};

	struct Vec3_Struct
	{
		float x,y,z;
	};

	class Vec3 : public Vec3_Struct
	{
	public:
#ifndef NO_VECTOR_CONSTRUCTORS
		inline Vec3() = default;
		inline Vec3(float f)
		{
			x = y = z = f;
		}
		inline Vec3(float vx, float vy, float vz)
		{
			x = vx;	y = vy;	z = vz;
		}
		inline Vec3(const Vec3 & v) = default;
#endif
		static inline Vec3 Create(float f)
		{
			Vec3 rs;
			rs.x = rs.y = rs.z = f;
			return rs;
		}
		static inline Vec3 Create(float vx, float vy, float vz)
		{
			Vec3 rs;
			rs.x = vx;	rs.y = vy;	rs.z = vz;
			return rs;
		}
		static inline Vec3 FromHomogeneous(const Vec4 & v);
		inline void SetZero()
		{
			x = y = z = 0.0f;
		}
		inline float& operator [] (int i) const
		{
			return ((float*)this)[i];
		}
		inline Vec3 operator + (const Vec3 &vin) const
		{
			Vec3 rs;
			rs.x = x + vin.x;
			rs.y = y + vin.y;
			rs.z = z + vin.z;
			return rs;
		}
		inline Vec3 operator - (const Vec3 &vin) const
		{
			Vec3 rs;
			rs.x = x - vin.x;
			rs.y = y - vin.y;
			rs.z = z - vin.z;
			return rs;
		}
		inline Vec3 operator - () const
		{
			Vec3 rs;
			rs.x = -x;
			rs.y = -y;
			rs.z = -z;
			return rs;
		}
		inline Vec3 operator * (float scale) const
		{
			Vec3 rs;
			rs.x = x * scale;
			rs.y = y * scale;
			rs.z = z * scale;
			return rs;
		}
		inline Vec3 & operator += (const Vec3 & vin)
		{
			x += vin.x; y += vin.y; z += vin.z;
			return *this;
		}
		inline Vec3 & operator -= (const Vec3 & vin)
		{
			x -= vin.x; y -= vin.y; z -= vin.z; 
			return *this;
		}
		inline Vec3 & operator *= (const Vec3 & vin)
		{
			x *= vin.x; y *= vin.y; z *= vin.z;
			return *this;
		}
		inline Vec3 & operator *= (float s)
		{
			x *= s; y *= s; z *= s;
			return *this;
		}
		inline Vec3 & operator /= (const Vec3 & vin)
		{
			x /= vin.x; y /= vin.y; z /= vin.z;
			return *this;
		}
		inline Vec3 & operator /= (float s)
		{
			float inv = 1.0f/s;
			return (*this)*=inv;
		}
		inline bool operator == (const Vec3 & vin)
		{
			return x == vin.x && y == vin.y && z == vin.z;
		}
		inline bool operator != (const Vec3 & vin)
		{
			return x != vin.x || y != vin.y || z != vin.z;
		}
		inline int GetHashCode()
		{
			return FloatAsInt(x) ^ FloatAsInt(y) ^ FloatAsInt(z);
		}
		inline static float Dot(const Vec3 & v1, const Vec3 & v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}
		inline static void Cross(Vec3 & rs_d, const Vec3 & v1, const Vec3 & v2)
		{
			rs_d.x = v1.y*v2.z - v1.z * v2.y;
			rs_d.y = v1.z*v2.x - v1.x * v2.z;
			rs_d.z = v1.x*v2.y - v1.y * v2.x;
		}
		inline static Vec3 Cross(const Vec3 & v1, const Vec3 & v2)
		{
			Vec3 rs_d;
			rs_d.x = v1.y*v2.z - v1.z * v2.y;
			rs_d.y = v1.z*v2.x - v1.x * v2.z;
			rs_d.z = v1.x*v2.y - v1.y * v2.x;
			return rs_d;
		}
		inline static void Scale(Vec3 & rs, const Vec3 & v1, float s)
		{
			rs.x = v1.x*s;	rs.y = v1.y*s;	rs.z = v1.z*s;
		}
		inline static void Add(Vec3 & rs, const Vec3 & v1, const Vec3 & v2)
		{
			rs.x = v1.x + v2.x;
			rs.y = v1.y + v2.y;
			rs.z = v1.z + v2.z;
		}
		inline static void Subtract(Vec3 & rs, const Vec3 & v1, const Vec3 & v2)
		{
			rs.x = v1.x - v2.x;
			rs.y = v1.y - v2.y;
			rs.z = v1.z - v2.z;
		}
		inline static void Multiply(Vec3 & rs, const Vec3 & v1, const Vec3 & v2)
		{
			rs.x = v1.x * v2.x;
			rs.y = v1.y * v2.y;
			rs.z = v1.z * v2.z;
		}
		inline float LengthFPU() const
		{
			return sqrt(x*x + y*y + z*z);
		}
		inline float Length2() const
		{
			return x*x+y*y+z*z;
		}
		static inline void NormalizeFPU(Vec3 & rs, const Vec3 & vin)
		{
			float invLen = 1.0f/vin.LengthFPU();
			Scale(rs, vin, invLen);
		}
		inline float Length() const;
		static inline void Normalize(Vec3 & rs, const Vec3 & vin);
		inline Vec3 Normalize() const
		{
			Vec3 rs;
			Normalize(rs, *this);
			return rs;
		}
	};

	struct Vec4_Struct
	{
		float x,y,z,w;
	};

	class Vec4 : public Vec4_Struct
	{
	public:
#ifndef NO_VECTOR_CONSTRUCTORS
		inline Vec4() = default;
		inline Vec4(const Vec4_Struct & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}
		inline Vec4(float f)
		{
			x = y = z = w = f;
		}
		inline Vec4(float vx, float vy, float vz, float vw)
		{
			x = vx;	y = vy;	z = vz;	w = vw;
		}
		inline Vec4(const Vec3 & v)
		{
			x = v.x; y = v.y; z = v.z; w = 0.0f;
		}
		inline Vec4(const Vec3 & v, float vw)
		{
			x = v.x; y = v.y; z = v.z; w = vw;
		}
		inline Vec4(const Vec4 & v) = default;
#endif
		static inline Vec4 Create(float f)
		{
			Vec4 rs;
			rs.x = rs.y = rs.z = rs.w = f;
			return rs;
		}
		static inline Vec4 Create(float vx, float vy, float vz, float vw)
		{
			Vec4 rs;
			rs.x = vx;	rs.y = vy;	rs.z = vz; rs.w = vw;
			return rs;
		}
		static inline Vec4 Create(const Vec3 & v, float vw)
		{
			Vec4 rs;
			rs.x = v.x; rs.y = v.y; rs.z = v.z; rs.w = vw;
			return rs;
		}
		inline void SetZero()
		{
			x = y = z = w = 0.0f;
		}
		inline void xyz(Vec3 & v) const
		{
			v.x = x;
			v.y = y;
			v.z = z;
		}
		inline Vec3 xyz() const
		{
			Vec3 rs;
			rs.x = x;
			rs.y = y;
			rs.z = z;
			return rs;
		}
		inline float& operator [] (int i)
		{
			return ((float*)this)[i];
		}
		inline Vec4 operator + (const Vec4 &vin)
		{
			Vec4 rs;
			rs.x = x + vin.x;
			rs.y = y + vin.y;
			rs.z = z + vin.z;
			rs.w = w + vin.w;
			return rs;
		}
		inline Vec4 operator - (const Vec4 &vin)
		{
			Vec4 rs;
			rs.x = x - vin.x;
			rs.y = y - vin.y;
			rs.z = z - vin.z;
			rs.w = w - vin.w;
			return rs;
		}
		inline Vec4 operator - ()
		{
			Vec4 rs;
			rs.x = -x;
			rs.y = -y;
			rs.z = -z;
			rs.w = -w;
			return rs;
		}
		inline Vec4 operator * (float scale) const
		{
			Vec4 rs;
			rs.x = x * scale;
			rs.y = y * scale;
			rs.z = z * scale;
			rs.w = w * scale;
			return rs;
		}
		inline Vec4 & operator += (const Vec4 & vin)
		{
			x += vin.x; y += vin.y; z += vin.z; w += vin.w;
			return *this;
		}
		inline Vec4 & operator -= (const Vec4 & vin)
		{
			x -= vin.x; y -= vin.y; z -= vin.z; w -= vin.w;
			return *this;
		}
		inline Vec4 & operator *= (const Vec4 & vin)
		{
			x *= vin.x; y *= vin.y; z *= vin.z; w *= vin.w;
			return *this;
		}
		inline Vec4 & operator *= (float s)
		{
			x *= s; y *= s; z *= s; w *= s;
			return *this;
		}
		inline Vec4 & operator /= (const Vec4 & vin)
		{
			x /= vin.x; y /= vin.y; z /= vin.z; w /= vin.w;
			return *this;
		}
		inline Vec4 & operator /= (float s)
		{
			float inv = 1.0f/s;
			return (*this)*=inv;
		}
		inline bool operator == (const Vec4 & vin)
		{
			return vin.x == x && vin.y == y && vin.z == z && vin.w == w;
		}
		inline bool operator != (const Vec4 & vin)
		{
			return vin.x != x || vin.y != y || vin.z != z || vin.w != w;
		}
		inline int GetHashCode()
		{
			return FloatAsInt(x) ^ FloatAsInt(y) ^ FloatAsInt(z) ^ FloatAsInt(w);
		}
		static inline void Add(Vec4 & rs, const Vec4 & v1, const Vec4 & v2);
		static inline void Subtract(Vec4 & rs, const Vec4 & v1, const Vec4 & v2);
		static inline void Multiply(Vec4 & rs, const Vec4 & v1, const Vec4 & v2);
		static inline void MultiplyScale(Vec4 & rs, const Vec4 & v1, const Vec4 & v2);
		static inline void Scale(Vec4 & rs, const Vec4 & v1, float s);
		static inline float Dot(const Vec4 & v1, const Vec4 & v2);
		static inline void Cross(Vec4 & rs_d, const Vec4 & v1, const Vec4 & v2);
		inline float LengthFPU() const;
		inline float Length() const;
		static inline void NormalizeFPU(Vec4& vout, const Vec4& vin);
		static inline void Normalize(Vec4 &vout, const Vec4 &vin);
		inline Vec4 Normalize()
		{
			Vec4 rs;
			Normalize(rs, *this);
			return rs;
		}
	};

	class Vec4_M128
	{
	public:
		__m128 vec;
		Vec4_M128()
		{}
		Vec4_M128(__m128 v)
		{
			vec = v;
		}
		Vec4_M128(float a, float b, float c, float d)
		{
			vec = _mm_set_ps(a, b, c, d);
		}
		Vec4_M128(const Vec4 & v)
		{
			vec = _mm_load_ps((const float*)&v);
		}
		inline void Zero()
		{
			vec = _mm_setzero_ps();
		}
		inline void ToVec4(Vec4 & v) const
		{
			_mm_store_ps((float*)&v, vec);
		}
	};

	class Matrix3
	{
	public:
		union
		{
			float values[9];
			float m[3][3];
			struct
			{
				float _11, _12, _13,
				_21, _22, _23,
				_31, _32, _33;
			} mi;
		};
		inline Vec3 Transform(const Vec3& vIn) const
		{
			Vec3 rs;
			rs.x = m[0][0] * vIn.x + m[1][0] * vIn.y + m[2][0] * vIn.z;
			rs.y = m[0][1] * vIn.x + m[1][1] * vIn.y + m[2][1] * vIn.z;
			rs.z = m[0][2] * vIn.x + m[1][2] * vIn.y + m[2][2] * vIn.z;
			return rs;
		}
		static inline void Multiply(Matrix3 & rs, Matrix3 & m1, Matrix3 & m2)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					float dot = 0.0f;
					for (int k = 0; k < 3; k++)
						dot += m1.m[k][j] * m2.m[i][k];
					rs.m[i][j] = dot;
				}
		}
	};

	class Matrix4
	{
	public:
		union
		{
			float values[16];
			float m[4][4];
			struct
			{
				float _11,_12,_13,_14,
				  _21,_22,_23,_24,
				  _31,_32,_33,_34,
				  _41,_42,_43,_44;
			} mi;
			struct
			{
				float _11,_12,_13,_14,
				  _21,_22,_23,_24,
				  _31,_32,_33,_34,
				  _41,_42,_43,_44;
			} mr;
		};
		Matrix4()
		{}
		Matrix4(float v)
		{
			for (int i = 0; i<16; i++)
				values[i] = v;
		}
		Matrix4(const Vec4 & c1, const Vec4 & c2, const Vec4 & c3, const Vec4 &c4)
		{
			memcpy(m[0], &c1, sizeof(Vec4));
			memcpy(m[1], &c2, sizeof(Vec4));
			memcpy(m[2], &c3, sizeof(Vec4));
			memcpy(m[3], &c4, sizeof(Vec4));
		}
		inline Matrix4 & operator += (const Matrix4 & v)
		{
			for (int i = 0; i < 16; i++)
				values[i] += v.values[i];
			return *this;
		}
		inline Matrix3 GetMatrix3()
		{
			Matrix3 rs;
			for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				rs.m[i][j] = m[i][j];
			return rs;
		}
		inline Matrix4 & operator *= (const float & val)
		{
			for (int i = 0; i < 16; i++)
				values[i] *= val;
			return *this;
		}
		inline Matrix4 & operator *= (const Matrix4 & matrix)
		{
			Multiply(*this, *this, matrix);
			return *this;
		}
		inline Matrix4 operator * (const Matrix4 & matrix)
		{
			Matrix4 rs;
			Multiply(rs, *this, matrix);
			return rs;
		}
		inline Matrix4 & LeftMultiply(const Matrix4 & matrix)
		{
			Multiply(*this, matrix, *this);
			return *this;
		}
		inline void Transpose()
		{
			float tmp;
			for (int i = 1; i<4; i++)
				for (int j = 0; j < i; j++)
				{
					tmp = m[i][j];
					m[i][j] = m[j][i];
					m[j][i] = tmp;
				}
		}
		static inline void CreateIdentityMatrix(Matrix4 & mOut);
		static inline void CreateRandomMatrix(Matrix4 & mOut);
		static inline void CreateOrthoMatrix(Matrix4 & mOut, float left, float right, float top, float bottom, float zNear, float zFar);
		static inline void CreatePerspectiveMatrixFromViewAngle(Matrix4 &mOut, float fovY, float aspect, float zNear, float zFar);
		static inline void CreatePerspectiveMatrixFromViewAngleTiled(Matrix4 &mOut, float fovY, float aspect, float zNear, float zFar, float x0, float y0, float x1, float y1);
		static inline void CreatePerspectiveMatrix(Matrix4 &mOut, float left, float right, float bottom, float top, float zNear, float zFar);
		static void LookAt(Matrix4 & rs, const Vec3 & pos, const Vec3 & center, const Vec3 & up);
		static inline void RotationX(Matrix4 & rs, float angle);
		static inline void RotationY(Matrix4 & rs, float angle);
		static inline void RotationZ(Matrix4 & rs, float angle);
		static void Rotation(Matrix4 & rs, const Vec3 & axis, float angle);
		static void Rotation(Matrix4 & rs, float yaw, float pitch, float roll);
		static inline void Scale(Matrix4 & rs, float sx, float sy, float sz);
		static inline void Translation(Matrix4 & rs, float tx, float ty, float tz);
		inline void Transform(Vec3 & rs_d, const Vec3& vIn) const;
		inline void Transform(Vec4 & rs_d, const Vec4& vIn) const;
		inline void TransformNormal(Vec3 & rs, const Vec3& vIn) const;
		inline void TransposeTransformNormal(Vec3 & rs, const Vec3 & vIn) const;
		inline void TransposeTransform(Vec3 & rs, const Vec3 & vIn) const;
		inline void TransposeTransform(Vec4 & rs_d, const Vec4& vIn) const;
		inline void TransformHomogeneous(Vec3 & rs, const Vec3 & vIn) const;
		inline void TransformHomogeneous2D(Vec2 & rs, const Vec3 & vIn) const;
		static inline void MultiplyFPU(Matrix4 &mOut, const Matrix4& M1, const Matrix4& M2);
		static inline void Multiply(Matrix4 &mOut, const Matrix4& M1, const Matrix4& M2);
		float Inverse3D(Matrix4 & mOut_d) const;
		float InverseFPU(Matrix4 &mOut_d) const;
		void GetNormalMatrix(Matrix4 & mOut) const;
		inline float Inverse(Matrix4 &mOut_d) const;
	};

	//__declspec(align(16))
	class Matrix4_M128
	{
	private:
		static const __m128 VecOne;
	public:
		__m128 C1,C2,C3,C4;
		Matrix4_M128()
		{}
		Matrix4_M128(const Matrix4 & m)
		{
			C1 = _mm_loadu_ps(m.values);
			C2 = _mm_loadu_ps(m.values+4);
			C3 = _mm_loadu_ps(m.values+8);
			C4 = _mm_loadu_ps(m.values+12);
		}
		inline void ToMatrix4(Matrix4 & mOut) const;
		inline void Transform(Vec4_M128 & rs, const Vec4& vIn) const;
		inline void Transform(Vec4 & rs, const Vec4& vIn) const;
		inline void Transform(Vec4_M128 & rs, const Vec3& vIn) const;
		inline void Transform(Vec3 & rs, const Vec3& vIn) const;
		inline void Transform(Vec4_M128 & rs, const Vec4_M128& vIn) const;
		inline void TransformNormal(Vec4_M128 & rs, const Vec4& vIn) const;
		inline void TransformNormal(Vec4 & rs, const Vec4& vIn) const;
		inline void TransformNormal(Vec4_M128 & rs, const Vec3& vIn) const;
		inline void TransformNormal(Vec3 & rs, const Vec3& vIn) const;
		inline void Multiply(Matrix4_M128 & rs, const Matrix4 & mB) const;
		inline void Multiply(Matrix4_M128 & rs, const Matrix4_M128 & mB) const;
		float Inverse(Matrix4_M128 &mOut) const;
	};

	//***********************************************************************
	/**************************** Implementation ****************************/
	//***********************************************************************
	//inline int FloatAsInt(float val)
	//{
	//	union InterCast
	//	{
	//		float fvalue;
	//		int ivalue;
	//	} cast;
	//	cast.fvalue = val;
	//	return cast.ivalue;
	//}
	//inline float IntAsFloat(int val)
	//{
	//	union InterCast
	//	{
	//		float fvalue;
	//		int ivalue;
	//	} cast;
	//	cast.ivalue = val;
	//	return cast.fvalue;
	//}
	inline Vec3 Vec3::FromHomogeneous(const Vec4 & v)
	{
		float invW = 1.0f/v.w;
		return v.xyz()*invW;
	}
	// Vec3
	inline float Vec3::Length() const
	{
		return Vec4::Create(*this, 0.0f).Length();
	}
	inline void Vec3::Normalize(Vec3 & rs, const Vec3 & vin)
	{
		Vec3::NormalizeFPU(rs, vin);
	}
	// Vec4
	inline void Vec4::Add(Vec4 & rs, const Vec4 & v1, const Vec4 & v2)
	{
		rs.x = v1.x + v2.x;
		rs.y = v1.y + v2.y;
		rs.z = v1.z + v2.z;
		rs.w = v1.w + v2.w;
	}
	inline void Vec4::Subtract(Vec4 & rs, const Vec4 & v1, const Vec4 & v2)
	{
		rs.x = v1.x - v2.x;
		rs.y = v1.y - v2.y;
		rs.z = v1.z - v2.z;
		rs.w = v1.w - v2.w;
	}
	inline void Vec4::Multiply(Vec4 & rs, const Vec4 & v1, const Vec4 & v2)
	{
		rs.x = v1.x * v2.x;
		rs.y = v1.y * v2.y;
		rs.z = v1.z * v2.z;
		rs.w = v1.w * v2.w;
	}
	inline void Vec4::MultiplyScale(Vec4 & rs, const Vec4 & v1, const Vec4 & v2)
	{
		rs.x = v1.x * v2.x;
		rs.y = v1.y * v2.y;
		rs.z = v1.z * v2.z;
		rs.w = v1.w * v2.w;
	}
	inline void Vec4::Scale(Vec4 & rs, const Vec4 & v1, float s)
	{
		rs.x = v1.x * s;
		rs.y = v1.y * s;
		rs.z = v1.z * s;
		rs.w = v1.w * s;
	}
	inline float Vec4::Dot(const Vec4 & v1, const Vec4 & v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	}
	inline void Vec4::Cross(Vec4 & rs_d, const Vec4 & v1, const Vec4 & v2)
	{
		rs_d.x = v1.y*v2.z - v1.z * v2.y;
		rs_d.y = v1.z*v2.x - v1.x * v2.z;
		rs_d.z = v1.x*v2.y - v1.y * v2.x;
		rs_d.w = 0.0f;
	}
	inline float Vec4::LengthFPU() const
	{
		return sqrt(Dot(*this, *this));
	}
	inline float Vec4::Length() const
	{
#ifdef NO_SIMD_ASM
		return LengthFPU();
#else
		float f;
		_asm
		{
			lea	ecx, f;
			mov	eax, this;

			movups	xmm0, [eax];
			mulps	xmm0, xmm0;
			movaps	xmm1, xmm0;
			shufps	xmm1, xmm1, 4Eh;
			addps	xmm0, xmm1;
			movaps	xmm1, xmm0;
			shufps	xmm1, xmm1, 11h;
			addss	xmm0, xmm1;

			sqrtss	xmm0, xmm0;
			movss	dword ptr [ecx], xmm0;
		}
		return f;
#endif
	}
	inline void Vec4::NormalizeFPU(Vec4& vout, const Vec4& vin)
	{
		float len = 1.0f/vin.Length();
		Scale(vout, vin, len);
	}
	inline void Vec4::Normalize(Vec4 &vout, const Vec4 &vin)
	{
#ifdef NO_SIMD_ASM
		NormalizeFPU(vout, vin);
#else
		__m128 xmm0 = _mm_loadu_ps((float*)&vin);
		__m128 xmm2 = xmm0;
		xmm0 = _mm_mul_ps(xmm0, xmm0);
		__m128 xmm1 = xmm0;
		xmm1 = _mm_shuffle_ps(xmm1, xmm1, 0x4E);
		xmm0 = _mm_add_ps(xmm0, xmm1);
		xmm1 = xmm0;
		xmm1 = _mm_shuffle_ps(xmm1, xmm1, 0x11);
		xmm0 = _mm_add_ps(xmm0, xmm1);
		xmm0 = _mm_rsqrt_ps(xmm0);
		xmm2 = _mm_mul_ps(xmm2, xmm0);
		_mm_storeu_ps((float*)&vout, xmm2);
#endif
	}

	// Matrix4
	inline void Matrix4::CreateIdentityMatrix(Matrix4 & mOut)
	{
		memset(&mOut, 0, sizeof(Matrix4));
		mOut.m[0][0] = mOut.m[1][1] = mOut.m[2][2] = mOut.m[3][3] = 1.0f;
	}

	inline void Matrix4::CreateOrthoMatrix(Matrix4 & mOut, float left, float right, float top, float bottom, float zNear, float zFar)
	{
		memset(&mOut, 0, sizeof(Matrix4));
		mOut.m[0][0] = 2.0f / (right - left);
		mOut.m[1][1] = 2.0f / (top - bottom);
		mOut.m[2][2] = -2.0f / (zFar - zNear);
		mOut.m[3][0] = -(right + left) / (right - left);
		mOut.m[3][1] = -(top + bottom) / (top - bottom);
		mOut.m[3][2] = -(zFar + zNear) / (zFar - zNear);
		mOut.m[3][3] = 1.0f;
	}

	inline void Matrix4::CreatePerspectiveMatrix(Matrix4 &mOut, float left, float right, float bottom, float top, float znear, float zfar)
	{
		memset(&mOut, 0, sizeof(Matrix4));
		mOut.m[0][0] = (znear*2.0f)/(right-left);
		mOut.m[1][1] = (2.0f*znear)/(top-bottom);
		mOut.m[2][0] = (right+left)/(right-left);
		mOut.m[2][1] = (top+bottom)/(top-bottom);
		mOut.m[2][2] = (zfar+znear)/(znear-zfar);
		mOut.m[2][3] = -1.0f;
		mOut.m[3][2] = 2.0f*zfar*znear/(znear-zfar);
	}

	inline void Matrix4::CreatePerspectiveMatrixFromViewAngle(Matrix4 &mOut, float fovY, float aspect, float zNear, float zFar)
	{
		float xmin, xmax, ymin, ymax;
		ymax = zNear * tan(fovY * CoreLib::Basic::Math::Pi / 360.0f);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		Matrix4::CreatePerspectiveMatrix(mOut, xmin, xmax, ymin, ymax, zNear, zFar);
	}

	inline void Matrix4::CreatePerspectiveMatrixFromViewAngleTiled(Matrix4 &mOut, float fovY, float aspect, float zNear, float zFar, float x0, float y0, float x1, float y1)
	{
		float xmin, xmax, ymin, ymax;
		ymax = zNear * tan(fovY * CoreLib::Basic::Math::Pi / 360.0f);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		x0 *= (xmax - xmin);  x0 += xmin;
		y0 *= (ymax - ymin); y0 += ymin;
		x1 *= (xmax - xmin);  x1 += xmin;
		y1 *= (ymax - ymin); y1 += ymin; 
		Matrix4::CreatePerspectiveMatrix(mOut, x0, x1, y0, y1, zNear, zFar);
	}

	inline void Matrix4::CreateRandomMatrix(Matrix4 & mOut)
	{
		for (int i = 0; i<16; i++)
		{
			mOut.values[i] = rand()/(float)RAND_MAX;
		}
	}
	inline void Matrix4::RotationX(Matrix4 & rs, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);

		Matrix4::CreateIdentityMatrix(rs);
		rs.m[1][1] = c;
		rs.m[2][1] = s;
		rs.m[1][2] = -s;
		rs.m[2][2] = c;
	}
	inline void Matrix4::RotationY(Matrix4 & rs, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);

		Matrix4::CreateIdentityMatrix(rs);
		rs.m[0][0] = c;
		rs.m[2][0] = s;
		rs.m[0][2] = -s;
		rs.m[2][2] = c;
	}
	inline void Matrix4::RotationZ(Matrix4 & rs, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);

		Matrix4::CreateIdentityMatrix(rs);
		rs.m[0][0] = c;
		rs.m[1][0] = s;
		rs.m[0][1] = -s;
		rs.m[1][1] = c;
	}

	inline void Matrix4::Scale(Matrix4 & rs, float sx, float sy, float sz)
	{
		Matrix4::CreateIdentityMatrix(rs);
		rs.m[0][0] = sx;
		rs.m[1][1] = sy;
		rs.m[2][2] = sz;
	}
	inline void Matrix4::Translation(Matrix4 & rs, float tx, float ty, float tz)
	{
		Matrix4::CreateIdentityMatrix(rs);
		rs.values[12] = tx;
		rs.values[13] = ty;
		rs.values[14] = tz;
	}
	inline void Matrix4::TransposeTransformNormal(Vec3 & rs, const Vec3 & vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[0][1]*vIn.y + m[0][2]*vIn.z;
		rs.y = m[1][0]*vIn.x + m[1][1]*vIn.y + m[1][2]*vIn.z;
		rs.z = m[2][0]*vIn.x + m[2][1]*vIn.y + m[2][2]*vIn.z;
	}
	inline void Matrix4::TransposeTransform(Vec3 & rs, const Vec3 & vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[0][1]*vIn.y + m[0][2]*vIn.z + m[0][3];
		rs.y = m[1][0]*vIn.x + m[1][1]*vIn.y + m[1][2]*vIn.z + m[1][3];
		rs.z = m[2][0]*vIn.x + m[2][1]*vIn.y + m[2][2]*vIn.z + m[2][3];
	}
	inline void Matrix4::TransposeTransform(Vec4 & rs, const Vec4 & vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[0][1]*vIn.y + m[0][2]*vIn.z + m[0][3]*vIn.w;
		rs.y = m[1][0]*vIn.x + m[1][1]*vIn.y + m[1][2]*vIn.z + m[1][3]*vIn.w;
		rs.z = m[2][0]*vIn.x + m[2][1]*vIn.y + m[2][2]*vIn.z + m[2][3]*vIn.w;
		rs.w = m[3][0]*vIn.x + m[3][1]*vIn.y + m[3][2]*vIn.z + m[3][3]*vIn.w;
	}
	inline void Matrix4::Transform(Vec3 & rs, const Vec3& vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[1][0]*vIn.y + m[2][0]*vIn.z + m[3][0];
		rs.y = m[0][1]*vIn.x + m[1][1]*vIn.y + m[2][1]*vIn.z + m[3][1];
		rs.z = m[0][2]*vIn.x + m[1][2]*vIn.y + m[2][2]*vIn.z + m[3][2];
	}
	inline void Matrix4::TransformHomogeneous(Vec3 & rs, const Vec3 & vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[1][0]*vIn.y + m[2][0]*vIn.z + m[3][0];
		rs.y = m[0][1]*vIn.x + m[1][1]*vIn.y + m[2][1]*vIn.z + m[3][1];
		rs.z = m[0][2]*vIn.x + m[1][2]*vIn.y + m[2][2]*vIn.z + m[3][2];
		float w = 1.0f/(m[0][3]*vIn.x + m[1][3]*vIn.y + m[2][3]*vIn.z + m[3][3]);
		rs.x *= w;
		rs.y *= w;
		rs.z *= w;
	}
	inline void Matrix4::TransformHomogeneous2D(Vec2 & rs, const Vec3 & vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[1][0]*vIn.y + m[2][0]*vIn.z + m[3][0];
		rs.y = m[0][1]*vIn.x + m[1][1]*vIn.y + m[2][1]*vIn.z + m[3][1];
		float w = 1.0f/(m[0][3]*vIn.x + m[1][3]*vIn.y + m[2][3]*vIn.z + m[3][3]);
		rs.x *= w;
		rs.y *= w;
	}
	inline void Matrix4::TransformNormal(Vec3 & rs, const Vec3& vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[1][0]*vIn.y + m[2][0]*vIn.z;
		rs.y = m[0][1]*vIn.x + m[1][1]*vIn.y + m[2][1]*vIn.z;
		rs.z = m[0][2]*vIn.x + m[1][2]*vIn.y + m[2][2]*vIn.z;
	}
	inline void Matrix4::Transform(Vec4 & rs, const Vec4& vIn) const
	{
		rs.x = m[0][0]*vIn.x + m[1][0]*vIn.y + m[2][0]*vIn.z + m[3][0]*vIn.w;
		rs.y = m[0][1]*vIn.x + m[1][1]*vIn.y + m[2][1]*vIn.z + m[3][1]*vIn.w;
		rs.z = m[0][2]*vIn.x + m[1][2]*vIn.y + m[2][2]*vIn.z + m[3][2]*vIn.w;
		rs.w = m[0][3]*vIn.x + m[1][3]*vIn.y + m[2][3]*vIn.z + m[3][3]*vIn.w;
	}
	inline void Matrix4::MultiplyFPU(Matrix4 &mOut, const Matrix4& M1, const Matrix4& M2)
	{
		Matrix4 TempMat;
		for (int i=0;i<4;i++) //col
		{
			for (int j=0;j<4;j++) // row
			{
				TempMat.m[i][j] = M1.m[0][j]*M2.m[i][0] + M1.m[1][j]*M2.m[i][1] + M1.m[2][j]*M2.m[i][2] + M1.m[3][j]*M2.m[i][3];
			}
		}
		memcpy(&mOut,&TempMat,sizeof(Matrix4));
	}

	inline void Matrix4::Multiply(Matrix4 &mOut, const Matrix4 &M1, const Matrix4 &M2)
	{
		Matrix4 rs;
		Matrix4_M128 TempMat;
		Matrix4_M128 mA(M1);
		mA.Multiply(TempMat, M2);
		TempMat.ToMatrix4(rs);
		mOut = rs;
	}
	inline float Matrix4::Inverse(Matrix4 &mOut_d) const
	{
		Matrix4 mat;
		Matrix4_M128 m_m(*this);
		Matrix4_M128 tmr;
		float rs = m_m.Inverse(tmr);
		tmr.ToMatrix4(mat);
		mOut_d = mat;
		return rs;
	}

	// Matrix4_M128

	inline void Matrix4_M128::ToMatrix4(Matrix4 & mOut) const
	{
		_mm_storeu_ps(mOut.values, C1);
		_mm_storeu_ps(mOut.values+4, C2);
		_mm_storeu_ps(mOut.values+8, C3);
		_mm_storeu_ps(mOut.values+12, C4);
	}
	inline void Matrix4_M128::Transform(Vec4_M128 & rs, const Vec4& vIn) const
	{
		__m128 r;
		r = _mm_mul_ps(C1, _mm_set_ps1(vIn.x));
		r = _mm_add_ps(r, _mm_mul_ps(C2, _mm_set_ps1(vIn.y)));
		r = _mm_add_ps(r, _mm_mul_ps(C3, _mm_set_ps1(vIn.z)));
		r = _mm_add_ps(r, _mm_mul_ps(C4, _mm_set_ps1(vIn.w)));
		rs.vec = r;
	}
	inline void Matrix4_M128::Transform(Vec4 & rs, const Vec4& vIn) const
	{
		Vec4_M128 r;
		Transform(r, vIn);
		_mm_store_ps((float*)&rs, r.vec);
	}
	inline void Matrix4_M128::Transform(Vec4_M128 & rs, const Vec3& vIn) const
	{
		__m128 r;
		r = _mm_mul_ps(C1, _mm_set_ps1(vIn.x));
		r = _mm_add_ps(r, _mm_mul_ps(C2, _mm_set_ps1(vIn.y)));
		r = _mm_add_ps(r, _mm_mul_ps(C3, _mm_set_ps1(vIn.z)));
		rs.vec = r;
	}
	inline void Matrix4_M128::Transform(Vec3 & rs, const Vec3& vIn) const
	{
		Vec4_M128 r;
		Transform(r, vIn);
		Vec4 tmp;
		_mm_store_ps((float*)&tmp, r.vec);
		rs.x = tmp.x;
		rs.y = tmp.y;
		rs.z = tmp.z;
	}
	inline void Matrix4_M128::Transform(Vec4_M128 & rs, const Vec4_M128& vIn) const
	{
		__m128 r;
		__m128 x,y,z,w;
		x = _mm_shuffle_ps(vIn.vec, vIn.vec, _MM_SHUFFLE(0, 0, 0, 0));
		r = _mm_mul_ps(C1, x);
		y = _mm_shuffle_ps(vIn.vec, vIn.vec, _MM_SHUFFLE(1, 1, 1, 1));
		r = _mm_add_ps(r, _mm_mul_ps(C2, y));
		z = _mm_shuffle_ps(vIn.vec, vIn.vec, _MM_SHUFFLE(2, 2, 2, 2));
		r = _mm_add_ps(r, _mm_mul_ps(C3, z));
		w = _mm_shuffle_ps(vIn.vec, vIn.vec, _MM_SHUFFLE(3, 3, 3, 3));
		r = _mm_add_ps(r, _mm_mul_ps(C4, w));
		rs.vec = r;
	}
	inline void Matrix4_M128::TransformNormal(Vec4_M128 & rs, const Vec4& vIn) const
	{
		__m128 r;
		r = _mm_mul_ps(C1, _mm_set_ps1(vIn.x));
		r = _mm_add_ps(r, _mm_mul_ps(C2, _mm_set_ps1(vIn.y)));
		r = _mm_add_ps(r, _mm_mul_ps(C3, _mm_set_ps1(vIn.z)));
		rs.vec = r;
	}
	inline void Matrix4_M128::TransformNormal(Vec4 & rs, const Vec4& vIn) const
	{
		Vec4_M128 r;
		TransformNormal(r, vIn);
		_mm_store_ps((float*)&rs, r.vec);
		rs.w = 0.0f;
	}
	inline void Matrix4_M128::TransformNormal(Vec4_M128 & rs, const Vec3& vIn) const
	{
		__m128 r;
		r = _mm_mul_ps(C1, _mm_set_ps1(vIn.x));
		r = _mm_add_ps(r, _mm_mul_ps(C2, _mm_set_ps1(vIn.y)));
		r = _mm_add_ps(r, _mm_mul_ps(C3, _mm_set_ps1(vIn.z)));
		rs.vec = r;
	}
	inline void Matrix4_M128::TransformNormal(Vec3 & rs, const Vec3& vIn) const
	{
		Vec4_M128 r;
		TransformNormal(r, vIn);
		Vec4 tmp;
		_mm_store_ps((float*)&tmp, r.vec);
		rs = tmp.xyz();
	}
	inline void Matrix4_M128::Multiply(Matrix4_M128 & rs, const Matrix4 & mB) const
	{
		__m128 T0, T1, T2, T3, R0, R1, R2, R3;
		T0 = _mm_set_ps1(mB.values[0]);
		T1 = _mm_set_ps1(mB.values[1]);
		T2 = _mm_set_ps1(mB.values[2]);
		T3 = _mm_set_ps1(mB.values[3]);
		R0 = _mm_mul_ps(C1, T0);
		T0 = _mm_set_ps1(mB.values[4]);
		R1 = _mm_mul_ps(C2, T1);
		R1 = _mm_add_ps(R1, R0);
		R2 = _mm_mul_ps(C3, T2);
		T1 = _mm_set_ps1(mB.values[5]);
		R3 = _mm_mul_ps(C4, T3);
		R2 = _mm_add_ps(R2, R1);
		T2 = _mm_set_ps1(mB.values[6]);
		rs.C1 = _mm_add_ps(R3, R2);
		R0 = _mm_mul_ps(C1, T0);
		T3 = _mm_set_ps1(mB.values[7]);
		R1 = _mm_mul_ps(C2, T1);
		T0 = _mm_set_ps1(mB.values[8]);
		R2 = _mm_mul_ps(C3, T2);
		R1 = _mm_add_ps(R1, R0);
		T1 = _mm_set_ps1(mB.values[9]);
		R3 = _mm_mul_ps(C4, T3);
		R2 = _mm_add_ps(R2, R1);
		rs.C2 = _mm_add_ps(R3, R2);
		T2 = _mm_set_ps1(mB.values[10]);
		R0 = _mm_mul_ps(C1, T0);
		T3 = _mm_set_ps1(mB.values[11]);
		R1 = _mm_mul_ps(C2, T1);
		T0 = _mm_set_ps1(mB.values[12]);
		R2 = _mm_mul_ps(C3, T2);
		R1 = _mm_add_ps(R1, R0);
		T2 = _mm_set_ps1(mB.values[14]);
		R3 = _mm_mul_ps(C4, T3);
		R2 = _mm_add_ps(R2, R1);
		T1 = _mm_set_ps1(mB.values[13]);
		rs.C3 = _mm_add_ps(R3, R2);
		R0 = _mm_mul_ps(C1, T0);
		R1 = _mm_mul_ps(C2, T1);
		T3 = _mm_set_ps1(mB.values[15]);
		R2 = _mm_mul_ps(C3, T2);
		R1 = _mm_add_ps(R1, R0);
		R3 = _mm_mul_ps(C4, T3);
		R2 = _mm_add_ps(R2, R1);
		rs.C4 = _mm_add_ps(R3, R2);
	}
	inline void Matrix4_M128::Multiply(Matrix4_M128 & rs, const Matrix4_M128 & mB) const
	{
		__m128 T0, T1, T2, T3, R0, R1, R2, R3;
		T0 = _mm_shuffle_ps(mB.C1, mB.C1, _MM_SHUFFLE(0,0,0,0));
		T1 = _mm_shuffle_ps(mB.C1, mB.C1, _MM_SHUFFLE(1,1,1,1));
		T2 = _mm_shuffle_ps(mB.C1, mB.C1, _MM_SHUFFLE(2,2,2,2));
		T3 = _mm_shuffle_ps(mB.C1, mB.C1, _MM_SHUFFLE(3,3,3,3));
		R0 = _mm_mul_ps(C1, T0);
		R1 = _mm_mul_ps(C2, T1);
		R2 = _mm_mul_ps(C3, T2);
		R3 = _mm_mul_ps(C4, T3);
		R1 = _mm_add_ps(R1, R0);
		R2 = _mm_add_ps(R2, R1);
		rs.C1 = _mm_add_ps(R3, R2);

		T0 = _mm_shuffle_ps(mB.C2, mB.C2, _MM_SHUFFLE(0,0,0,0));
		T1 = _mm_shuffle_ps(mB.C2, mB.C2, _MM_SHUFFLE(1,1,1,1));
		T2 = _mm_shuffle_ps(mB.C2, mB.C2, _MM_SHUFFLE(2,2,2,2));
		T3 = _mm_shuffle_ps(mB.C2, mB.C2, _MM_SHUFFLE(3,3,3,3));
		R0 = _mm_mul_ps(C1, T0);
		R1 = _mm_mul_ps(C2, T1);
		R2 = _mm_mul_ps(C3, T2);
		R3 = _mm_mul_ps(C4, T3);
		R1 = _mm_add_ps(R1, R0);
		R2 = _mm_add_ps(R2, R1);
		rs.C2 = _mm_add_ps(R3, R2);

		T0 = _mm_shuffle_ps(mB.C3, mB.C3, _MM_SHUFFLE(0,0,0,0));
		T1 = _mm_shuffle_ps(mB.C3, mB.C3, _MM_SHUFFLE(1,1,1,1));
		T2 = _mm_shuffle_ps(mB.C3, mB.C3, _MM_SHUFFLE(2,2,2,2));
		T3 = _mm_shuffle_ps(mB.C3, mB.C3, _MM_SHUFFLE(3,3,3,3));
		R0 = _mm_mul_ps(C1, T0);
		R1 = _mm_mul_ps(C2, T1);
		R2 = _mm_mul_ps(C3, T2);
		R3 = _mm_mul_ps(C4, T3);
		R1 = _mm_add_ps(R1, R0);
		R2 = _mm_add_ps(R2, R1);
		rs.C3 = _mm_add_ps(R3, R2);

		T0 = _mm_shuffle_ps(mB.C4, mB.C4, _MM_SHUFFLE(0,0,0,0));
		T1 = _mm_shuffle_ps(mB.C4, mB.C4, _MM_SHUFFLE(1,1,1,1));
		T2 = _mm_shuffle_ps(mB.C4, mB.C4, _MM_SHUFFLE(2,2,2,2));
		T3 = _mm_shuffle_ps(mB.C4, mB.C4, _MM_SHUFFLE(3,3,3,3));
		R0 = _mm_mul_ps(C1, T0);
		R1 = _mm_mul_ps(C2, T1);
		R2 = _mm_mul_ps(C3, T2);
		R3 = _mm_mul_ps(C4, T3);
		R1 = _mm_add_ps(R1, R0);
		R2 = _mm_add_ps(R2, R1);
		rs.C4 = _mm_add_ps(R3, R2);
	}

	inline void CartesianToSphere(const Vec3 & dir, float & u, float & v)
	{
		const float inv2Pi = 0.5f/PI;
		v = acos(dir.y);
		u = atan2(dir.z, dir.x);
		if (u<0.0f)
			u += PI * 2.0f;
		u *= inv2Pi;
		v *= 1.0f/PI;
	}

	inline void SphereToCartesian(Vec3 & dir, float u, float v)
	{
		dir.y = cos(v);
		float s = sin(v);
		dir.x = cos(u) * s;
		dir.z = sin(u) * s;
	}

	inline void GetOrthoVec(Vec3 & vout, const Vec3 & vin)
	{
		Vec3 absV = Vec3::Create(abs(vin.x), abs(vin.y), abs(vin.z));
		if (absV.x <= absV.y && absV.x <= absV.z)
			Vec3::Cross(vout, vin, Vec3::Create(1.0f, 0.0f, 0.0f));
		else if (absV.y <= absV.x && absV.y <= absV.z)
			Vec3::Cross(vout, vin, Vec3::Create(0.0f, 1.0f, 0.0f));
		else
			Vec3::Cross(vout, vin, Vec3::Create(0.0f, 0.0f, 1.0f));
	}

	template<typename T>
	inline T CatmullInterpolate(const T & p0, const T & p1, const T & p2, const T & p3, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;
		return (p1 * 2.0f + (-p0 + p2) * t +
			(p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t2 +
			(-p0 + p1 * 3.0f - p2 * 3.0f + p3) * t3) * 0.5f;
	}
#ifdef _MSC_VER
#ifndef __clang__
#ifndef M128_OPERATOR_OVERLOADS
#define M128_OPERATOR_OVERLOADS
	inline __m128 & operator += (__m128 & v0, const __m128 &v1)
	{
		v0 = _mm_add_ps(v0, v1);
		return v0;
	}
	inline __m128 & operator -= (__m128 & v0, const __m128 &v1)
	{
		v0 = _mm_sub_ps(v0, v1);
		return v0;
	}
	inline __m128 & operator *= (__m128 & v0, const __m128 &v1)
	{
		v0 = _mm_mul_ps(v0, v1);
		return v0;
	}
	inline __m128 & operator /= (__m128 & v0, const __m128 &v1)
	{
		v0 = _mm_div_ps(v0, v1);
		return v0;
	}
	inline __m128 operator + (const __m128 & v0, const __m128 & v1)
	{
		return _mm_add_ps(v0, v1);
	}
	inline __m128 operator - (const __m128 & v0, const __m128 & v1)
	{
		return _mm_sub_ps(v0, v1);
	}
	inline __m128 operator * (const __m128 & v0, const __m128 & v1)
	{
		return _mm_mul_ps(v0, v1);
	}
	inline __m128 operator / (const __m128 & v0, const __m128 & v1)
	{
		return _mm_div_ps(v0, v1);
	}
	inline __m128 operator - (const __m128 & v0)
	{
		static const __m128 SIGNMASK = 
               _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
		return _mm_xor_ps(v0, SIGNMASK);
	}

	inline __m128i & operator += (__m128i & v0, const __m128i &v1)
	{
		v0 = _mm_add_epi32(v0, v1);
		return v0;
	}
	inline __m128i & operator -= (__m128i & v0, const __m128i &v1)
	{
		v0 = _mm_sub_epi32(v0, v1);
		return v0;
	}
	inline __m128i & operator *= (__m128i & v0, const __m128i &v1)
	{
		v0 = _mm_mul_epi32(v0, v1);
		return v0;
	}
	inline __m128i operator + (const __m128i & v0, const __m128i & v1)
	{
		return _mm_add_epi32(v0, v1);
	}
	inline __m128i operator - (const __m128i & v0, const __m128i & v1)
	{
		return _mm_sub_epi32(v0, v1);
	}
	inline __m128i operator * (const __m128i & v0, const __m128i & v1)
	{
		return _mm_mullo_epi32(v0, v1);
	}
	inline __m128i operator - (const __m128i & v0)
	{
		return _mm_xor_si128(v0, _mm_set1_epi32(0xFFFFFFFF));
	}
#endif
#endif
	_declspec(align(16))
	class SSEVec3
	{
	public:
		__m128 x,y,z;
		SSEVec3()
		{};
		SSEVec3(__m128 x, __m128 y, __m128 z)
			:x(x), y(y), z(z)
		{
		}
		SSEVec3(const Vec3 &v)
		{
			this->x = _mm_set_ps1(v.x);
			this->y = _mm_set_ps1(v.y);
			this->z = _mm_set_ps1(v.z);
		}
		SSEVec3(float x, float y, float z)
		{
			this->x = _mm_set_ps1(x);
			this->y = _mm_set_ps1(y);
			this->z = _mm_set_ps1(z);
		}
		inline __m128 Length()
		{
			return _mm_sqrt_ps(x*x + y*y + z*z);
		}
		inline void Normalize(__m128 one)
		{
			auto s = one / Length();
			x *= s;
			y *= s;
			z *= s;
		}
		inline SSEVec3 operator + (const SSEVec3 &vin)
		{
			SSEVec3 rs;
			rs.x = x + vin.x;
			rs.y = y + vin.y;
			rs.z = z + vin.z;
			return rs;
		}
		inline SSEVec3 operator - (const SSEVec3 &vin)
		{
			SSEVec3 rs;
			rs.x = x - vin.x;
			rs.y = y - vin.y;
			rs.z = z - vin.z;
			return rs;
		}
		inline SSEVec3 operator - ()
		{
			SSEVec3 rs;
			rs.x = -x;
			rs.y = -y;
			rs.z = -z;
			return rs;
		}
		inline SSEVec3 operator * (__m128 scale)
		{
			SSEVec3 rs;
			rs.x = x * scale;
			rs.y = y * scale;
			rs.z = z * scale;
			return rs;
		}
		inline SSEVec3 & operator += (const SSEVec3 & vin)
		{
			x += vin.x; y += vin.y; z += vin.z;
			return *this;
		}
		inline SSEVec3 & operator -= (const SSEVec3 & vin)
		{
			x -= vin.x; y -= vin.y; z -= vin.z; 
			return *this;
		}
		inline SSEVec3 & operator *= (const SSEVec3 & vin)
		{
			x *= vin.x; y *= vin.y; z *= vin.z;
			return *this;
		}
		inline SSEVec3 & operator *= (__m128 s)
		{
			x *= s; y *= s; z *= s;
			return *this;
		}
		inline SSEVec3 & operator /= (const SSEVec3 & vin)
		{
			x /= vin.x; y /= vin.y; z /= vin.z;
			return *this;
		}
		inline SSEVec3 & operator /= (float s)
		{
			float inv = 1.0f/s;
			return (*this)*=_mm_set_ps1(inv);
		}

		inline static __m128 Dot(const SSEVec3 & v1, const SSEVec3 & v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}
		inline static void Cross(SSEVec3 & rs_d, const SSEVec3 & v1, const SSEVec3 & v2)
		{
			rs_d.x = v1.y*v2.z - v1.z * v2.y;
			rs_d.y = v1.z*v2.x - v1.x * v2.z;
			rs_d.z = v1.x*v2.y - v1.y * v2.x;
		}
	};

	_declspec(align(16))
	class SSEVec4
	{
	public:
		__m128 x, y, z, w;
		SSEVec4()
		{};
		SSEVec4(const __m128 & x, const __m128 & y, const __m128 & z, const __m128 & w)
			:x(x), y(y), z(z), w(w)
		{
		}
		SSEVec4(const Vec4 &v)
		{
			this->x = _mm_set_ps1(v.x);
			this->y = _mm_set_ps1(v.y);
			this->z = _mm_set_ps1(v.z);
			this->w = _mm_set_ps1(v.w);
		}
		SSEVec4(float x, float y, float z, float w)
		{
			this->x = _mm_set_ps1(x);
			this->y = _mm_set_ps1(y);
			this->z = _mm_set_ps1(z);
			this->w = _mm_set_ps1(w);
		}
		inline __m128 Length()
		{
			return _mm_sqrt_ps(x*x + y*y + z*z + w*w);
		}
		inline void Normalize(__m128 one)
		{
			auto s = one / Length();
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}
		inline SSEVec4 operator + (const SSEVec4 &vin)
		{
			SSEVec4 rs;
			rs.x = x + vin.x;
			rs.y = y + vin.y;
			rs.z = z + vin.z;
			rs.w = w + vin.w;
			return rs;
		}
		inline SSEVec4 operator - (const SSEVec4 &vin)
		{
			SSEVec4 rs;
			rs.x = x - vin.x;
			rs.y = y - vin.y;
			rs.z = z - vin.z;
			rs.w = w - vin.w;
			return rs;
		}
		inline SSEVec4 operator - ()
		{
			SSEVec4 rs;
			rs.x = -x;
			rs.y = -y;
			rs.z = -z;
			rs.w = -w;
			return rs;
		}
		inline SSEVec4 operator * (__m128 scale)
		{
			SSEVec4 rs;
			rs.x = x * scale;
			rs.y = y * scale;
			rs.z = z * scale;
			rs.w = w * scale;
			return rs;
		}
		inline SSEVec4 & operator += (const SSEVec4 & vin)
		{
			x += vin.x; y += vin.y; z += vin.z; w += vin.w;
			return *this;
		}
		inline SSEVec4 & operator -= (const SSEVec4 & vin)
		{
			x -= vin.x; y -= vin.y; z -= vin.z; w -= vin.w;
			return *this;
		}
		inline SSEVec4 & operator *= (const SSEVec4 & vin)
		{
			x *= vin.x; y *= vin.y; z *= vin.z; w *= vin.w;
			return *this;
		}
		inline SSEVec4 & operator *= (__m128 s)
		{
			x *= s; y *= s; z *= s; w *= s;
			return *this;
		}
		inline SSEVec4 & operator /= (const SSEVec4 & vin)
		{
			x /= vin.x; y /= vin.y; z /= vin.z; w /= vin.w;
			return *this;
		}
		inline SSEVec4 & operator /= (float s)
		{
			float inv = 1.0f / s;
			return (*this) *= _mm_set_ps1(inv);
		}

		inline static __m128 Dot(const SSEVec4 & v1, const SSEVec4 & v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
		}
	};

	_declspec(align(16))
	class SSEMatrix4
	{
	public:
		__m128 values[16];
		SSEMatrix4()
		{}
		SSEMatrix4(const Matrix4 & mat)
		{
			for (int i = 0; i<16; i++)
				values[i] = _mm_set_ps1(mat.values[i]);
		}
		inline SSEVec3 Transform(SSEVec3 & v)
		{
			SSEVec3 rs;
			rs.x = values[0]*v.x + values[4]*v.y + values[8]*v.z + values[12];
			rs.y = values[1]*v.x + values[5]*v.y + values[9]*v.z + values[13];
			rs.z = values[2]*v.x + values[6]*v.y + values[10]*v.z + values[14];
			auto w = values[3]*v.x + values[7]*v.y + values[11]*v.z + values[15];
			w = _mm_set_ps1(1.0f) / w;
			rs.x *= w;
			rs.y *= w;
			rs.z *= w;
			return rs;
		}
		inline SSEVec3 TransformNonPerspective(SSEVec3 & v)
		{
			SSEVec3 rs;
			rs.x = values[0]*v.x + values[4]*v.y + values[8]*v.z + values[12];
			rs.y = values[1]*v.x + values[5]*v.y + values[9]*v.z + values[13];
			rs.z = values[2]*v.x + values[6]*v.y + values[10]*v.z + values[14];
			return rs;
		}
	};
#endif

	class Vec2i
	{
	public:
		int x, y;
		static Vec2i Create(int px, int py)
		{
			Vec2i rs;
			rs.x = px;
			rs.y = py;
			return rs;
		}
	};

	class Vec3i
	{
	public:
		int x, y, z;
		static Vec3i Create(int px, int py, int pz)
		{
			Vec3i rs;
			rs.x = px;
			rs.y = py;
			rs.z = pz;
			return rs;
		}
	};

	class Vec4i
	{
	public:
		int x, y, z, w;
		static Vec4i Create(int px, int py, int pz, int pw)
		{
			Vec4i rs;
			rs.x = px;
			rs.y = py;
			rs.z = pz;
			rs.w = pw;
			return rs;
		}
	};
}

#endif

/***********************************************************************
LIBIO.H
***********************************************************************/
#ifndef CORE_LIB_IO_H
#define CORE_LIB_IO_H


namespace CoreLib
{
	namespace IO
	{
		class File
		{
		public:
			static bool Exists(const CoreLib::Basic::String & fileName);
			static CoreLib::Basic::String ReadAllText(const CoreLib::Basic::String & fileName);
			static void WriteAllText(const CoreLib::Basic::String & fileName, const CoreLib::Basic::String & text);
		};

		class Path
		{
		public:
#ifdef _WIN32
			static const wchar_t PathDelimiter = L'\\';
#else
			static const wchar_t PathDelimiter = L'/';
#endif
			static String TruncateExt(const String & path);
			static String ReplaceExt(const String & path, const wchar_t * newExt);
			static String GetFileName(const String & path);
			static String GetFileNameWithoutEXT(const String & path);
			static String GetFileExt(const String & path);
			static String GetDirectoryName(const String & path);
			static String Combine(const String & path1, const String & path2);
			static String Combine(const String & path1, const String & path2, const String & path3);
#ifdef CreateDirectory
#undef CreateDirectory
#endif
			static bool CreateDirectory(const String & path);
		};

		class CommandLineWriter : public Object
		{
		public:
			virtual void Write(const String & text) = 0;
		};

		void SetCommandLineWriter(CommandLineWriter * writer);

		extern CommandLineWriter * currentCommandWriter;
		template<typename ...Args>
		void uiprintf(const wchar_t * format, Args... args)
		{
			if (currentCommandWriter)
			{
				wchar_t buffer[1024];
				swprintf_s(buffer, format, args...);
				currentCommandWriter->Write(buffer);
			}
		}
	}
}

#endif

/***********************************************************************
PERFORMANCECOUNTER.H
***********************************************************************/
#ifndef CORELIB_PERFORMANCE_COUNTER_H
#define CORELIB_PERFORMANCE_COUNTER_H

#include <chrono>

namespace CoreLib
{
	namespace Diagnostics
	{
		typedef std::chrono::high_resolution_clock::time_point TimePoint;
		typedef std::chrono::high_resolution_clock::duration Duration;
		class PerformanceCounter
		{
		public:
			static TimePoint Start();
			static Duration End(TimePoint counter);
			static float EndSeconds(TimePoint counter);
			static double ToSeconds(Duration duration);
		};
	}
}

#endif

/***********************************************************************
MEMORYPOOL.H
***********************************************************************/
#ifndef CORE_LIB_MEMORY_POOL_H
#define CORE_LIB_MEMORY_POOL_H


namespace CoreLib
{
	namespace Basic
	{
		struct MemoryBlockFields
		{
			unsigned int Occupied : 1;
			unsigned int Order : 31;
		};
		struct FreeListNode
		{
			FreeListNode * PrevPtr = nullptr, *NextPtr = nullptr;
		};
		class MemoryPool
		{
		private:
			static const int MaxLevels = 32;
			int blockSize = 0, log2BlockSize = 0;
			int numLevels = 0;
			int bytesAllocated = 0;
			int bytesWasted = 0;
			unsigned char * buffer = nullptr;
			FreeListNode * freeList[MaxLevels];
			IntSet used;
			int AllocBlock(int level);
			void FreeBlock(unsigned char * ptr, int level);
		public:
			MemoryPool(unsigned char * buffer, int log2BlockSize, int numBlocks);
			MemoryPool() = default;
			void Init(unsigned char * buffer, int log2BlockSize, int numBlocks);
			unsigned char * Alloc(int size);
			void Free(unsigned char * ptr, int size);
		};
	}
}

#endif

/***********************************************************************
MD5.H
***********************************************************************/
/*
* This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
* MD5 Message-Digest Algorithm (RFC 1321).
*
* Homepage:
* http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
*
* Author:
* Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
*
* This software was written by Alexander Peslyak in 2001.  No copyright is
* claimed, and the software is hereby placed in the public domain.
* In case this attempt to disclaim copyright and place the software in the
* public domain is deemed null and void, then the software is
* Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
* general public under the following terms:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*
* There's ABSOLUTELY NO WARRANTY, express or implied.
*
* See md5.c for more information.
*/

#ifdef HAVE_OPENSSL
#include <openssl/md5.h>
#elif !defined(_MD5_H)
#define _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD5_u32plus;

typedef struct {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned char buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;

extern void MD5_Init(MD5_CTX *ctx);
extern void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
extern void MD5_Final(unsigned char *result, MD5_CTX *ctx);

#endif

/***********************************************************************
PARSER.H
***********************************************************************/
#ifndef CORELIB_TEXT_PARSER_H
#define CORELIB_TEXT_PARSER_H


namespace CoreLib
{
	namespace Text
	{
		class TextFormatException : public Exception
		{
		public:
			TextFormatException(String message)
				: Exception(message)
			{}
		};

		const int TokenType_Identifier = 3;
		const int TokenType_Int = 4;
		const int TokenType_Float = 5;
		const int TokenType_StringLiteral = 6;
		const int TokenType_CharLiteral = 7;

		class Parser
		{
		private:
			static RefPtr<MetaLexer> metaLexer;
			LazyLexStream stream;
			bool legal;
			String text;
			List<LazyLexToken> tokens;
			int tokenPtr;
			LexToken MakeToken(LazyLexToken ltk)
			{
				LexToken tk;
				tk.Position = ltk.Position;
				tk.TypeID = ltk.TypeID;
				tk.Str = text.SubString(ltk.Position, ltk.Length);
				return tk;
			}
		public:
			static MetaLexer * GetTextLexer();
			static void DisposeTextLexer();
			static Basic::List<Basic::String> SplitString(Basic::String str, wchar_t ch);
			Parser(Basic::String text);
			int ReadInt()
			{
				auto token = ReadToken();
				bool neg = false;
				if (token.Str == L'-')
				{
					neg = true;
					token = ReadToken();
				}
				if (token.TypeID == TokenType_Int)
				{
					if (neg)
						return -StringToInt(token.Str);
					else
						return StringToInt(token.Str);
				}
				throw TextFormatException(L"Text parsing error: int expected.");
			}
			double ReadDouble()
			{
				auto token = ReadToken();
				bool neg = false;
				if (token.Str == L'-')
				{
					neg = true;
					token = ReadToken();
				}
				if (token.TypeID == TokenType_Float || token.TypeID == TokenType_Int)
				{
					if (neg)
						return -StringToDouble(token.Str);
					else
						return StringToDouble(token.Str);
				}
				throw TextFormatException(L"Text parsing error: floating point value expected.");
			}
			String ReadWord()
			{
				auto token = ReadToken();
				if (token.TypeID == TokenType_Identifier)
				{
					return token.Str;
				}
				throw TextFormatException(L"Text parsing error: identifier expected.");
			}
			String Read(const wchar_t * expectedStr)
			{
				auto token = ReadToken();
				if (token.Str == expectedStr)
				{
					return token.Str;
				}
				throw TextFormatException(L"Text parsing error: \'" + String(expectedStr) + L"\' expected.");
			}
			String Read(String expectedStr)
			{
				auto token = ReadToken();
				if (token.Str == expectedStr)
				{
					return token.Str;
				}
				throw TextFormatException(L"Text parsing error: \'" + expectedStr + L"\' expected.");
			}
			static String EscapeStringLiteral(String str)
			{
				StringBuilder sb;
				sb << L"\"";
				for (int i = 0; i < str.Length(); i++)
				{
					switch (str[i])
					{
					case L' ':
						sb << L"\\s";
						break;
					case L'\n':
						sb << L"\\n";
						break;
					case L'\r':
						sb << L"\\r";
						break;
					case L'\t':
						sb << L"\\t";
						break;
					case L'\v':
						sb << L"\\v";
						break;
					case L'\'':
						sb << L"\\\'";
						break;
					case L'\"':
						sb << L"\\\"";
						break;
					case L'\\':
						sb << L"\\\\";
						break;
					default:
						sb << str[i];
						break;
					}
				}
				sb << L"\"";
				return sb.ProduceString();
			}
			String UnescapeStringLiteral(String str)
			{
				StringBuilder sb;
				for (int i = 0; i < str.Length(); i++)
				{
					if (str[i] == L'\\' && i < str.Length() - 1)
					{
						switch (str[i + 1])
						{
						case L's':
							sb << L" ";
							break;
						case L't':
							sb << L'\t';
							break;
						case L'n':
							sb << L'\n';
							break;
						case L'r':
							sb << L'\r';
							break;
						case L'v':
							sb << L'\v';
							break;
						case L'\'':
							sb << L'\'';
							break;
						case L'\"':
							sb << L"\"";
							break;
						case L'\\':
							sb << L"\\";
							break;
						default:
							i = i - 1;
							sb << str[i];
						}
						i++;
					}
					else
						sb << str[i];
				}
				return sb.ProduceString();
			}
			String ReadStringLiteral()
			{
				auto token = ReadToken();
				if (token.TypeID == TokenType_StringLiteral)
				{
					return UnescapeStringLiteral(token.Str.SubString(1, token.Str.Length()-2));
				}
				throw TextFormatException(L"Text parsing error: string literal expected.");
			}
			void Back(int count)
			{
				tokenPtr -= count;
			}
			LexToken ReadToken()
			{
				if (tokenPtr < tokens.Count())
				{
					LexToken rs = MakeToken(tokens[tokenPtr]);
					tokenPtr++;
					return rs;
				}
				throw TextFormatException(L"Unexpected ending.");
			}
			LexToken NextToken()
			{
				if (tokenPtr < tokens.Count())
					return MakeToken(tokens[tokenPtr]);
				else
				{
					LexToken rs;
					rs.TypeID = -1;
					rs.Position = -1;
					return rs;
				}
			}
			bool LookAhead(String token)
			{
				if (tokenPtr < tokens.Count())
				{
					auto next = NextToken();
					return next.Str == token;
				}
				else
				{
					return false;
				}
			}
			bool IsEnd()
			{
				return tokenPtr == tokens.Count();
			}
		public:
			bool IsLegalText()
			{
				return legal;
			}
		};

		List<String> Split(String str, wchar_t c);
	}
}

#endif

/***********************************************************************
THREADING.H
***********************************************************************/
#ifndef CORE_LIB_THREADING_H
#define CORE_LIB_THREADING_H
#include <atomic>
#include <thread>
#include <mutex>

#ifndef _WIN32
#define __stdcall
#endif

namespace CoreLib
{
	namespace Threading
	{
		class SpinLock
		{
		private:
			std::atomic_flag lck;
		public:
			SpinLock()
			{
				lck.clear();
			}
			inline bool TryLock()
			{
				return !lck.test_and_set(std::memory_order_acquire);
			}
			inline void Lock()
			{
				while (lck.test_and_set(std::memory_order_acquire))
				{
				}
			}
			inline void Unlock()
			{
				lck.clear(std::memory_order_release);
			}
			SpinLock & operator = (const SpinLock & /*other*/)
			{
				lck.clear();
				return *this;
			}
		};

		class ParallelSystemInfo
		{
		public:
			static int GetProcessorCount();
		};

		typedef CoreLib::Basic::Event<> ThreadProc;
		typedef CoreLib::Basic::Event<CoreLib::Basic::Object *> ThreadParameterizedProc;
		class Thread;

		class ThreadParam
		{
		public:
			Thread * thread;
			CoreLib::Basic::Object * threadParam;
		};

		enum class ThreadPriority
		{
			Normal,
			AboveNormal,
			Highest,
			Critical,
			BelowNormal,
			Lowest,
			Idle
		};
		unsigned int __stdcall ThreadProcedure(const ThreadParam& param);
		class Thread : public CoreLib::Basic::Object
		{
			friend unsigned int __stdcall ThreadProcedure(const ThreadParam& param);
		private:
			 ThreadParam internalParam;
		public:
			
		private:
			std::thread threadHandle;
			CoreLib::Basic::RefPtr<ThreadProc> threadProc;
			CoreLib::Basic::RefPtr<ThreadParameterizedProc> paramedThreadProc;
		public:
			Thread()
			{
				internalParam.threadParam = nullptr;
				internalParam.thread = this;
			}
			Thread(ThreadProc * p)
				: Thread()
			{
				Start(p);
			}
			Thread(ThreadParameterizedProc * p, CoreLib::Basic::Object * param)
				: Thread()
			{
				Start(p, param);
			}
			void Start(ThreadProc * p)
			{
				threadProc = p;
				threadHandle = std::thread(ThreadProcedure, internalParam);
			}
			void Start(ThreadParameterizedProc * p, CoreLib::Basic::Object * param)
			{
				paramedThreadProc = p;
				internalParam.thread = this;
				internalParam.threadParam = param;
				threadHandle = std::thread(ThreadProcedure, internalParam);
			}
			void Join()
			{
				if (threadHandle.joinable())
					threadHandle.join();
			}
			void Detach()
			{
				if (threadHandle.joinable())
					threadHandle.detach();
			}
			std::thread::id GetHandle()
			{
				return threadHandle.get_id();
			}
		};

		class Mutex : public CoreLib::Basic::Object
		{
		private:
			std::mutex handle;
		public:
			void Lock()
			{
				handle.lock();
			}
			bool TryLock()
			{
				return handle.try_lock();
			}
			void Unlock()
			{
				return handle.unlock();
			}
		};
	}
}

#endif
