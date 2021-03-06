#include "SmartHandleHolder.h"

SmartHandleHolder::SmartHandleHolder(HANDLE handle)
	:_handle(handle)
{
}

SmartHandleHolder::~SmartHandleHolder()
{
	// CR: always put new line before {
	try { 
		CloseHandle(_handle);
	}

	catch (...)
	{
	}

}

HANDLE SmartHandleHolder::get_handle() const
{
	return _handle;
}
