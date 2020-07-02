#include "HttpResponse.h"
#include "HttpExternalCode.h"

HttpResponseBuilder::HttpResponseBuilder(unsigned long status_code, const char message_code[])
{
	INITIALIZE_HTTP_RESPONSE(&_response, status_code, message_code);
}

HttpResponseBuilder& HttpResponseBuilder::config_header(HTTP_HEADER_ID header_id, const char raw_value[])
{
	ADD_KNOWN_HEADER(_response, header_id, raw_value);
	return *this;
}

void HttpResponseBuilder::set_chunks(const CHUNKS_DATA& chunks_data)
{
	_response.EntityChunkCount = chunks_data.number_of_chunks;
	_response.pEntityChunks = chunks_data.chunks_array_ptr->data();
}

HTTP_RESPONSE* HttpResponseBuilder::get()
{
	return &_response;
}
