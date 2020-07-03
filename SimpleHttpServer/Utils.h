#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <vector>
#include <string>
#include <exception>

// CR: This is extra credit, but you can work on this if you have time
// In most projects there is a Debug.hpp that contains useful utilities such as:
// 1. A debug logging function, that prints in debug and does nothing in release, using IFDEFS
// 		- This function uses macros to print the function, line number
// 		- This function usually uses some sort of std::wstring StringUtils::format(const std::wstring& format, ...) function that needs to be implemented
// 		- The logging is done using OutputDebugStringW and read from dbgview / visual studio (in you case cout is also ok)
// 2. THROW / CHECK macros, that also print so you know what line number / function threw the exception. Also they print the condition that failed
// 3. Exception heirarchy that has at least a general Exception, and a Win32Exception that you throw when a winapi function fails and also saves the GetLastError()
#define THROW_IF_NOT(condition)\
{\
	if(!(condition))\
	{\
		throw std::exception();\
	}\
}\

// CR: remove unused code!!!
size_t fnGetWStringLength(PCWSTR szString, size_t maxSize);

size_t fnGetWStringSize(PCWSTR szString, size_t maxSize);

LPVOID fnAllocate(SIZE_T cbBytes);

void fnFree(LPVOID);

PWSTR convertCSTR(PCSTR sString, DWORD cbBytesInString);



void add_string_to_vector(const std::string& str, std::vector<char>& buffer, unsigned int offset);

void add_string_to_vector(const std::wstring& str, std::vector<char>& buffer, unsigned int offset);

