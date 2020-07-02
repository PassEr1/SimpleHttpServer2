#include "PathReader.h"
#include <sstream>
#include <string>
#include "DirectoryIterator.h"
#include "Utils.h"



PathReader::PathReader(const std::wstring& abs_path)
:_abs_path(abs_path)		
{
}

PathReader::~PathReader()
{

}

PathReader::HandlersMapping PathReader::get_path_handlers()const
{
	HandlersMapping mapping_handlers;
	mapping_handlers.insert({ FileReader::PathAttribute::File , &PathReader::file_handle});
	mapping_handlers.insert({ FileReader::PathAttribute::Directory, &PathReader::directory_handle});
	mapping_handlers.insert({ FileReader::PathAttribute::None, &PathReader::defult_handler});
	return mapping_handlers;
}

FileReader::BufferPtr PathReader::defult_handler() const
{
	static const FileReader::Buffer _default_empty_data = FileReader::Buffer();
	return std::make_shared<FileReader::Buffer>(_default_empty_data);
}

FileReader::BufferPtr PathReader::file_handle() const
{
	static const LPOVERLAPPED DONT_USE_OVERLLAPED = NULL;
	FileReader file_reader(_abs_path, FILE_SHARE_READ, OPEN_EXISTING);
	return file_reader.read(DEFAULT_READ_SIZE_BYTES);
}


FileReader::BufferPtr PathReader::wstring_to_pbuffer(std::wstring wstr)
{
	unsigned int size_of_buffer_in_bytes = wstr.size() * sizeof(WCHAR);
	FileReader::Buffer buffer(size_of_buffer_in_bytes);
	CopyMemory(buffer.data(), wstr.data(), size_of_buffer_in_bytes);

	return std::make_shared<FileReader::Buffer>(buffer);
}

FileReader::BufferPtr PathReader::directory_handle() const
{
	static unsigned int SIZE_OF_NEW_LINE_CHARACTER = 2;
	static const std::wstring NEW_LINE_CHARACTER(L"\n");
	unsigned int total_bytes_read = 0;
	unsigned long append_offset = 0;
	unsigned long size_going_to_be_written = 0;
	FileReader::Buffer buffer;
	DirectoryIterator iterator(_abs_path);
	std::wstring builded_result;

	while (iterator.has_next())
	{
		std::wstring file_name = iterator.get_next();
		builded_result += (file_name + NEW_LINE_CHARACTER);
	}

	return wstring_to_pbuffer(builded_result);
}

//std::wstring read_as_wstring(FileReader::BufferPtr buffer)//const
//{
//	std::wstring buffer_as_string(buffer->begin(), buffer->end());
//}

FileReader::BufferPtr PathReader::read_now() const
{
	FileReader::PathAttribute path_attribute = FileReader::get_path_attribute(_abs_path);
	HandlersMapping handlers_mapping = get_path_handlers();
	MemberFunctionPathHandler selected_handler = handlers_mapping[path_attribute];
	FileReader::BufferPtr presult_buffer = (this->*selected_handler)();
	return presult_buffer;
}

