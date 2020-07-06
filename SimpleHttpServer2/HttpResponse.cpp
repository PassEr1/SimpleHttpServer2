#include "HttpResponse.h"
#include "HttpExternalCode.h"
#include "BytesBuffer.h"
#include "Utils.h"

const unsigned int HttpResponseBuilder::_defult_size_for_message = 2048;

HttpResponseBuilder::HttpResponseBuilder(unsigned long status_code, const char message_code[])
{
	INITIALIZE_HTTP_RESPONSE(&_response, status_code, message_code);
}

HttpResponseBuilder& HttpResponseBuilder::config_header(HTTP_HEADER_ID header_id, const char raw_value[])
{
	ADD_KNOWN_HEADER(_response, header_id, raw_value);
	return *this;
}


unsigned long HttpResponseBuilder::calculate_num_of_chunks(const BytesBufferPtr data_buffer)
{
    return ceil((double)data_buffer->size() / HttpResponseBuilder::_defult_size_for_message); //round up
}

void HttpResponseBuilder::set_chunks(const BytesBufferPtr data_buffer)
{
    THROW_IF_NOT(data_buffer->size() != 0);

    unsigned long number_of_chunks = calculate_num_of_chunks(data_buffer);
    unsigned long size_of_last_chunk = data_buffer->size() % HttpResponseBuilder::_defult_size_for_message;
    char* offset_next_chunk = data_buffer->data();

    for (size_t i = 0; i < number_of_chunks; i++, offset_next_chunk += HttpResponseBuilder::_defult_size_for_message)
    {
        HTTP_DATA_CHUNK new_data_chunk;
        new_data_chunk.DataChunkType = HttpDataChunkFromMemory;
        new_data_chunk.FromMemory.pBuffer = (PVOID)offset_next_chunk;
        new_data_chunk.FromMemory.BufferLength = HttpResponseBuilder::_defult_size_for_message;
        _chunks_array.push_back(new_data_chunk);

    }
    //set last chunk's size
    _chunks_array[number_of_chunks - 1].FromMemory.BufferLength = size_of_last_chunk;
    _response.EntityChunkCount = number_of_chunks;
    _response.pEntityChunks = _chunks_array.data();
    
}


HTTP_RESPONSE* HttpResponseBuilder::get()
{
	return &_response;
}
