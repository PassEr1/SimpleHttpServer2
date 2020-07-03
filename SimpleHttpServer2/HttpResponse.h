#pragma once
#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <http.h>
#include <stdio.h>
#include <string>
#include "HttpMessageChunks.h"

class HttpResponseBuilder
{
public:
	explicit HttpResponseBuilder(unsigned long status_code, const char message_code[]);
	HttpResponseBuilder(const HttpResponseBuilder& other) = default;
	HttpResponseBuilder(HttpResponseBuilder&& other) = default;

public:
	HttpResponseBuilder& operator=(HttpResponseBuilder&& other) = delete;
	HttpResponseBuilder& operator=(const HttpResponseBuilder& other) = delete;

public:
	HttpResponseBuilder& config_header(HTTP_HEADER_ID, const char raw_value[]);
	void set_chunks(const CHUNKS_DATA& chunks_data);
	// CR: I dont like the fact that you are returning a pointer to an internal member of the class. Occasionaly its ok to just copy it!
	HTTP_RESPONSE* get();

private:
	HTTP_RESPONSE _response;
};
