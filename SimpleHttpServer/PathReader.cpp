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

FileReader::BufferPtr PathReader::buffer_char_to_wchar(const FileReader::BufferPtr& p_buffer)
{
	unsigned long size_of_orignal_data = p_buffer->size() * sizeof(WCHAR);
	FileReader::Buffer new_buffer(size_of_orignal_data);

	int status = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		p_buffer->data(),
		p_buffer->size(),
		reinterpret_cast<LPWSTR>(new_buffer.data()),
		p_buffer->size() * sizeof(WCHAR)
	);
	
	THROW_IF_NOT(status != 0);

	return std::make_shared<FileReader::Buffer>(new_buffer);
}

FileReader::BufferPtr PathReader::file_handle() const
{
	// CR: this static const is unused...
	static const LPOVERLAPPED DONT_USE_OVERLLAPED = NULL;
	FileReader file_reader(_abs_path, FILE_SHARE_READ, OPEN_EXISTING);
	// CR: why not read the whole file size? for this i would add a get_size() function 
	// to file reader, and also a read() function that receives no arguments and reads 
	// the whole file
	FileReader::BufferPtr p_buffer = file_reader.read(DEFAULT_READ_SIZE_BYTES);
	return p_buffer; //buffer_char_to_wchar(p_buffer);
}


FileReader::BufferPtr PathReader::wstring_to_pbuffer(std::wstring wstr)
{
	unsigned int size_of_buffer_in_bytes = wstr.size() * sizeof(WCHAR);
	FileReader::Buffer buffer(size_of_buffer_in_bytes);
	// CR: prefer memcpy rather than CopyMemory. Not sure why.. maybe portability? even though previously i told you not to be portable :)
	CopyMemory(buffer.data(), wstr.data(), size_of_buffer_in_bytes);

	return std::make_shared<FileReader::Buffer>(buffer);
}

FileReader::BufferPtr PathReader::directory_handle() const
{
	// CR: lots of unused variables here...
	// CR: change the project settings to warning level 4 and treat warnings as errors... VisualStudio is supposed to tell you when you have unused variables
	static unsigned int SIZE_OF_NEW_LINE_CHARACTER = 2;
	static const std::wstring NEW_LINE_CHARACTER(L"\n");
	unsigned int total_bytes_read = 0;
	unsigned long append_offset = 0;
	unsigned long size_going_to_be_written = 0;
	FileReader::Buffer buffer;
	DirectoryIterator iterator(_abs_path);
	std::wstring builded_result;

	// CR: extra credit: return an html file where every file is a link so that you can navigate like in the real SimpleHTTPServer
	// not sure how easy it will be with the HttpApi and url registering...
	while (iterator.has_next())
	{
		std::wstring file_name = iterator.get_next();
		builded_result += (file_name + NEW_LINE_CHARACTER);
	}

	return wstring_to_pbuffer(builded_result);
}


FileReader::BufferPtr PathReader::read_now() const
{
	FileReader::PathAttribute path_attribute = FileReader::get_path_attribute(_abs_path);
	HandlersMapping handlers_mapping = get_path_handlers();
	MemberFunctionPathHandler selected_handler = handlers_mapping[path_attribute];
	FileReader::BufferPtr presult_buffer = (this->*selected_handler)();
	return presult_buffer;
}

