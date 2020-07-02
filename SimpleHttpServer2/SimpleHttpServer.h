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
#include "PathReader.h"
#include <windows.h>
#include <http.h>
#include <stdio.h>
#include <exception>
#include "HttpRequest.h"


class SimpleHttpServer {
public:
	using fpLogger = void (*)(const std::wstring & szMessage);

public:
	SimpleHttpServer(const std::wstring& domain_name, const std::wstring& port_number, const std::wstring& server_root_path, fpLogger plogger_func);
	~SimpleHttpServer();

public:
	void start();

public:
	SimpleHttpServer(const SimpleHttpServer& other) = delete;
	SimpleHttpServer(SimpleHttpServer&& other)=delete;
	SimpleHttpServer& operator=(const SimpleHttpServer& other) = delete;
	SimpleHttpServer& operator=(SimpleHttpServer&& other) = delete;

private:
	static void _register_url(const std::wstring url_to_listen, HANDLE h_requests_queue_handle);
	static std::wstring reformat_domain_name(const std::wstring& domain_name, const std::wstring& port_number);
	static void wrap_buffer_with_html(BytesBufferPtr data_to_render);
	static HANDLE get_quque_handle();
	

private:
	void log_initializtion_finished(const std::wstring& full_domain_name);
	HttpRequest recv_new_request();
	BytesBufferPtr handle_request(const HttpRequest& reference_request);


private:
	SmartHandleHolder _h_requests_queue_handle;
	const fpLogger _plogger_function;
	const std::wstring _server_root_path;

private:
	static const unsigned int _defult_size_for_message; // we need this becuase since we allocate buffer to receive messages
};