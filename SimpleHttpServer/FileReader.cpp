#include "FileReader.h"
#include "Utils.h"

HANDLE FileReader::get_file_handler(const std::wstring& file_path, DWORD share_mode, DWORD creation_disposition)
{
	// CR: no magic variables! static const for the security attributes and template file
	HANDLE hfile = CreateFileW(
		file_path.c_str(),
		GENERIC_READ,
		share_mode,
		NULL, 
		creation_disposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	WIN32_THROW_IF_NOT(hfile != INVALID_HANDLE_VALUE);

	return hfile;
}

FileReader::FileReader(const std::wstring file_path, DWORD share_mode, DWORD creation_disposition)
:SmartHandleHolder(get_file_handler(file_path, share_mode, creation_disposition))
{
}

FileReader::~FileReader()
{
}

FileReader::BufferPtr FileReader::read(size_t size) const
{
	static const LPOVERLAPPED DONT_USE_OVERLLAPED = NULL;
	unsigned long total_bytes_read = 0;
	DWORD bytes_read = 0;
	bool status = TRUE;
	Buffer buffer(size);
	
	status = ReadFile(
		this->get_handle(), //for expressivity sake
		buffer.data(),
		size,
		&bytes_read,
		DONT_USE_OVERLLAPED);

	THROW_IF_NOT(status);
	buffer.resize(bytes_read);
	return std::make_shared<FileReader::Buffer>(buffer);
}

FileReader::PathAttribute FileReader::get_path_attribute(const std::wstring& path)
{
	DWORD attribute_identifer = GetFileAttributesW(path.c_str());
	// CR: Unneeded, is yoda notation
	if (INVALID_FILE_ATTRIBUTES == attribute_identifer)
	{
		return PathAttribute::None; 
	}

	else if (FILE_ATTRIBUTE_DIRECTORY & attribute_identifer)
	{
		return PathAttribute::Directory;
	}
	else
	{
		return PathAttribute::File;
	}
}
