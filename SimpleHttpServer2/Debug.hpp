#pragma once
#include <string>
#include <Windows.h>
#include <exception>
#include "Utils.h"

#ifdef _DEBUG
#define DEBUG_W(message_str)\
	std::wstring message_forammatted = foramate_string(std::wstring(L"[%s] %s:%d \"%s\""), __FILEW__, __FUNCTIONW__, __LINE__, (message_str));\
	OutputDebugStringW(message_forammatted.c_str());
#else
#  define DEBUG(not_gonna_do_nothing)
#endif


#define STRING(s) std::wstring(#s);



