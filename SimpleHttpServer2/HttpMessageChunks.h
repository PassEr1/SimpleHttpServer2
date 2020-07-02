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
#include <vector>
#include <memory>

typedef struct CHUNKS_DATA
{
public:
    using HttpDataChunkArray = std::vector<HTTP_DATA_CHUNK>;
    using HttpDataChunkArrayPtr = std::shared_ptr<HttpDataChunkArray>;
    
public:
    HttpDataChunkArrayPtr chunks_array_ptr;
    unsigned int number_of_chunks;

}CHUNKS_DATA;