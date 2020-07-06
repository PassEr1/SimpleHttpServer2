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
#include "PathReader.h"
#include "BytesBuffer.h"
#include "HttpMessageChunks.h"
#include "HttpResponse.h"

/*
this class essentially holds context of the request,
in order to send response to the right destination.
*/
class HttpRequest
{
public:
	HttpRequest(BytesBufferPtr pbuffer, HANDLE h_requests_queue_handle);
	HttpRequest(HttpRequest&& other) = default;
	~HttpRequest();

public:
	HttpRequest(const HttpRequest& other)=delete;
	HttpRequest& operator=(HttpRequest&& other)=delete;
	HttpRequest& operator=(const HttpRequest& other)=delete;


public:
	void response(BytesBufferPtr data_to_send);
	HTTP_VERB get_http_request_type()const ;
	std::wstring get_url_of_request()const;


private:
	static HttpResponseBuilder build_response(BytesBufferPtr data_puffer_ptr);

private:
	static const unsigned int _defult_size_for_message;

private:
	BytesBufferPtr _pmessage_buffer;
	HANDLE _h_request_queue_handle_context;
};
