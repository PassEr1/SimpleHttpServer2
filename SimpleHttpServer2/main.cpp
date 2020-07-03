
#include <iostream>
#include "SimpleHttpServer.h"

// CR: naming, and this will probably change in light of my comments in utils.h
void ConsoleLogger(const std::wstring& szMessage)
{
    std::wcout << L"[LOG_MESSAGE] " << szMessage << std::endl;
}

// CR: nice and clean. good job
// CR: try to add support for the path as a command line argument, and if no argument is given, the current directory
int main()
{
	// CR: not very important in this case... but always wrap all entry points (main, thread functions, callbacks) with 
	// try {} catch (...) {}
    std::wstring server_root(L"C:\\Users\\amitb\\\Downloads");
    SimpleHttpServer myserver(L"http://127.0.0.1", L"80", server_root, ConsoleLogger);
    myserver.start();
    std::cout << "Hello World!\n"; // CR: hmmmm...
}