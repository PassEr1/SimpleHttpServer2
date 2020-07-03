#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Utils.h"
#include "FileReader.h"

// CR: remove unused code!
static DWORD DEFAULT_READ_SIZE_BYTES = 1024 * 1024;

class PathReader
{
	// CR: why are these public?
	// CR: use the bytebuffer::bufferptr
public:
	using MemberFunctionPathHandler = FileReader::BufferPtr (PathReader::*)(void) const;
	using HandlersMapping = std::map<FileReader::PathAttribute, MemberFunctionPathHandler>;

public:
	PathReader(const std::wstring& abs_path); // CR: explicit
	~PathReader();

public:
	FileReader::BufferPtr read_now()const; // CR: spacing

public:
	PathReader(const PathReader& other)=delete;
	PathReader& operator=(const PathReader& other)=delete;
	PathReader(PathReader&& other) = delete;
	PathReader& operator=(PathReader&& other) = delete;

private:
	HandlersMapping get_path_handlers() const;
	FileReader::BufferPtr defult_handler() const; // CR: typo
	FileReader::BufferPtr file_handle()const; // CR: spacing, typo (handler)
	FileReader::BufferPtr directory_handle()const; // CR: spacing, typo (handler)

private:
	static FileReader::BufferPtr wstring_to_pbuffer(std::wstring wstr); // CR: const reference
	// CR: remove unused function!! in general, you have lots of unused code. Dont forget to remove!
	static FileReader::BufferPtr buffer_char_to_wchar(const FileReader::BufferPtr& p_buffer);

private:
	const std::wstring _abs_path;
};



