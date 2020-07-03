#pragma once
#include <vector>
#include <string>
#include <memory>
#include "SmartHandleHolder.h"



class FileReader 
	:public SmartHandleHolder
{
public:
	enum class PathAttribute { Directory, File, None };
	// CR: duplicate, you have ByteBuffer.h
	using Buffer = std::vector<char>;
	using BufferPtr = std::shared_ptr<Buffer>;


public:
	// CR: const reference
	FileReader(const std::wstring file_path, DWORD share_mode, DWORD creation_disposition); 
	~FileReader();

public:
	BufferPtr read(size_t size)const;
	static PathAttribute get_path_attribute(const std::wstring& path);

public:
	FileReader(const FileReader& other) = delete;
	FileReader(FileReader&& other) = delete;
	FileReader& operator=(FileReader&& other) = delete;
	FileReader& operator=(FileReader& other) = delete;

private:
	// CR: typo, handle
	static HANDLE get_file_handler(const std::wstring& file_path, DWORD share_mode, DWORD creation_disposition);

};