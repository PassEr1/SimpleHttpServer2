#include "HttpRequest.h"


const unsigned int HttpRequest::_defult_size_for_message = 2048 ;

HttpRequest::HttpRequest(BytesBufferPtr buffer, HANDLE h_request_queue_handle)
    :_pmessage_buffer(buffer),
    _h_request_queue_handle_context(h_request_queue_handle)
{

}

HttpRequest::~HttpRequest()
{
}


HttpResponseBuilder HttpRequest::build_response(BytesBufferPtr data_puffer_ptr)
{
    HttpResponseBuilder response_builder(200, "ok");
    response_builder
        .config_header(HttpHeaderContentType, "text/html")
        .config_header(HttpHeaderAcceptEncoding, "UTF-8")
        .config_header(HttpHeaderContentEncoding, "UTF-8");
    response_builder.set_chunks(data_puffer_ptr);
    
    return response_builder;
}

void HttpRequest::response(BytesBufferPtr data_to_send)
{
    static const ULONG RESERVED2 = 0;
    static const LPVOID RESERVED1 = 0;
    static const PHTTP_LOG_DATA DONT_LOG = NULL;
    static const PHTTP_CACHE_POLICY NO_CACH_POLICY = NULL;
    static const LPOVERLAPPED NO_OVERLLAPED = NULL;

    PHTTP_REQUEST pReferenceRequest = reinterpret_cast<PHTTP_REQUEST>(_pmessage_buffer->data());
    DWORD bytesSent;
    HttpResponseBuilder reponse_builder = build_response(data_to_send);
    
    DWORD result = HttpSendHttpResponse(
        _h_request_queue_handle_context,
        pReferenceRequest->RequestId,
        HTTP_SEND_RESPONSE_FLAG_MORE_DATA,
        reponse_builder.get(),
        NO_CACH_POLICY,
        &bytesSent,
        RESERVED1,
        RESERVED2,
        NO_OVERLLAPED,
        DONT_LOG
    );


    THROW_IF_NOT(NO_ERROR == result);
    
}

HTTP_VERB HttpRequest::get_http_request_type()const 
{
	PHTTP_REQUEST p_request = reinterpret_cast<PHTTP_REQUEST>(_pmessage_buffer->data());
	return p_request->Verb;
}

std::wstring HttpRequest::get_url_of_request() const
{
	PHTTP_REQUEST p_request = reinterpret_cast<PHTTP_REQUEST>(_pmessage_buffer->data());
	return std::wstring(p_request->CookedUrl.pAbsPath);
}


