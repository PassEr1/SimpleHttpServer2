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

unsigned long HttpRequest::calculate_num_of_chunks(const BytesBufferPtr data_buffer)
{
    return ceil((double)data_buffer->size() / HttpRequest::_defult_size_for_message); //round up
}

// CR: I think that if you moved the responsibility of making chunks and stuff into HttpResponse,
// you wont need the HttpMessageChunks struct and the code will be encapsulated better
CHUNKS_DATA HttpRequest::get_chunks(const BytesBufferPtr data_buffer)
{
    THROW_IF_NOT(data_buffer->size() != 0);

    CHUNKS_DATA::HttpDataChunkArray chunks;
    unsigned long number_of_chunks = calculate_num_of_chunks(data_buffer);
    unsigned long size_of_last_chunk = data_buffer->size() % HttpRequest::_defult_size_for_message;
    char* offset_next_chunk = data_buffer->data();

    for (size_t i = 0; i < number_of_chunks; i++, offset_next_chunk += HttpRequest::_defult_size_for_message)
    {
        HTTP_DATA_CHUNK new_data_chunk;
        new_data_chunk.DataChunkType = HttpDataChunkFromMemory;
        new_data_chunk.FromMemory.pBuffer = (PVOID)offset_next_chunk;
        new_data_chunk.FromMemory.BufferLength = HttpRequest::_defult_size_for_message;
        chunks.push_back(new_data_chunk);

    }
    //set last chunk's size
    chunks[number_of_chunks - 1].FromMemory.BufferLength = size_of_last_chunk;

    return CHUNKS_DATA{ 
        std::make_shared<CHUNKS_DATA::HttpDataChunkArray>(chunks),
        number_of_chunks };
}

HttpResponseBuilder HttpRequest::build_response(CHUNKS_DATA chunks_data)
{
    HttpResponseBuilder response_builder(200, "ok");
    response_builder
        .config_header(HttpHeaderContentType, "text/html")
        .config_header(HttpHeaderAcceptEncoding, "UTF-8")
        .config_header(HttpHeaderContentEncoding, "UTF-8");
    response_builder.set_chunks(chunks_data);
    
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
    CHUNKS_DATA chunks_data = get_chunks(data_to_send);
    HttpResponseBuilder reponse_builder = build_response(chunks_data);
    
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


