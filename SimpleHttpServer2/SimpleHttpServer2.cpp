
#include <iostream>
#include "SimpleHttpServer.h"

void ConsoleLogger(const std::wstring& szMessage)
{
    std::wcout << L"[LOG_MESSAGE] " << szMessage << std::endl;
}

int main()
{
    std::wstring server_root(L"C:\\Users\\amitb\\\Downloads");
    SimpleHttpServer myserver(L"http://127.0.0.1", L"80", server_root, ConsoleLogger);
    myserver.start();
    std::cout << "Hello World!\n";
}