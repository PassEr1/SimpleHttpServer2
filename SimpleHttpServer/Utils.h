#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <vector>
#include <string>
#include <exception>

class Win32Exception;

#define STRING(s) (#s);
	
#define THROW_IF_NOT(condition)\
{\
	if(!(condition))\
	{\
		std::string cstr = STRING( #condition)\
		throw std::exception(cstr.c_str());\
	}\
}\


#define WIN32_THROW_IF_NOT(condition)\
{\
	if(!(condition))\
	{\
		std::string cstr = STRING( #condition)\
		throw Win32Exception(cstr.c_str());\
	}\
}\

class Win32Exception
	:public std::exception
{
public:
	Win32Exception(std::string what_happened)
		:std::exception(("Win32Exception- " + what_happened).c_str())
	{
	}

};

template<typename... Args>
std::wstring foramate_string(const std::wstring& format, const Args&... args)
{
	static const unsigned int _DEBUG_MESSAGE_DEFAULT_SIZE = 1000;
	std::wstring formatted(_DEBUG_MESSAGE_DEFAULT_SIZE, '\0');
	wsprintf((LPWSTR)formatted.data(), format.data(), args...);
	return formatted;

}


LPVOID fnAllocate(SIZE_T cbBytes);

void fnFree(LPVOID);

PWSTR convertCSTR(PCSTR sString, DWORD cbBytesInString);



void add_string_to_vector(const std::string& str, std::vector<char>& buffer, unsigned int offset);

void add_string_to_vector(const std::wstring& str, std::vector<char>& buffer, unsigned int offset);

