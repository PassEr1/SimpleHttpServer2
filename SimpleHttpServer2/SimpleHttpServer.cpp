#include "SimpleHttpServer.h"
#include <algorithm>
#include "Utils.h"

const unsigned int  SimpleHttpServer::_defult_size_for_message = 1024 * 1024;

SimpleHttpServer::SimpleHttpServer(const std::wstring& domain_name, const std::wstring& port_number, const std::wstring& server_root_path, fpLogger plogger_func)
:_h_requests_queue_handle(get_quque_handle()),
_plogger_function(plogger_func),
_server_root_path(server_root_path)
{
    std::wstring full_domain_name = reformat_domain_name(domain_name, port_number);
    _register_url(full_domain_name, _h_requests_queue_handle.get_handle());
    log_initializtion_finished(full_domain_name);
}

SimpleHttpServer::~SimpleHttpServer()
{
}

HttpRequest SimpleHttpServer::recv_new_request()
{
    // CR: I would move the api calls to the httprequest class
    static const HTTP_REQUEST_ID NO_REQUEST_ID = NULL;
    static const LPOVERLAPPED NO_OVERLAPPED = NULL;
    ULONG result;
    ULONG bytes_read;
    BytesBuffer buffer(sizeof(HTTP_REQUEST) + _defult_size_for_message);
    BytesBufferPtr p_to_buffer = std::make_shared<BytesBuffer>(buffer);
    PHTTP_REQUEST prequest_buffer = reinterpret_cast<PHTTP_REQUEST>(p_to_buffer->data());

    prequest_buffer->RequestId = HTTP_NULL_ID;
    result = HttpReceiveHttpRequest(
        _h_requests_queue_handle.get_handle(),
        NO_REQUEST_ID,
        HTTP_RECEIVE_REQUEST_FLAG_COPY_BODY,
        prequest_buffer,
        p_to_buffer->size(),
        &bytes_read,
        NO_OVERLAPPED
    );

    THROW_IF_NOT(result == NO_ERROR);
    p_to_buffer->resize(bytes_read);
    return HttpRequest(p_to_buffer, _h_requests_queue_handle.get_handle());
}

// CR: this function looks great!
void SimpleHttpServer::start()
{
    while (TRUE)
    {
        try 
        {
            HttpRequest http_request = recv_new_request();
            BytesBufferPtr response_data = handle_request(http_request);
            http_request.response(response_data); // justification: any request always has a response
            _plogger_function(L"[INFO] reponse sent.");
        }
        catch (...)
        {
            _plogger_function(L"[ERROR] could not send reponse!");
        }
        
    }
}

void SimpleHttpServer::log_initializtion_finished(const std::wstring& full_domain_name)
{
    _plogger_function(L"[INFO] Server setup completed!");
    _plogger_function(L"[INFO] Listening on: " + full_domain_name);
}

// CR: typo... queue, and probably http_queue is more clear
HANDLE SimpleHttpServer::get_quque_handle()
{
    ULONG RESERVED = 0;
    HANDLE quque_handle;

    // CR: For every call to HttpInitialize, HttpTerminate needs to be called. Make this a class / singleton!
    // CR: static const for the third variable...
    // CR: variable naming conventions
    //initialize service
    ULONG retCode = HttpInitialize(
        HTTPAPI_VERSION_1,
        HTTP_INITIALIZE_SERVER,
        NULL
    );

    THROW_IF_NOT(retCode == NO_ERROR);

    //set up requests handle:
    retCode = HttpCreateHttpHandle(
        &quque_handle,
        RESERVED);

    THROW_IF_NOT(retCode == NO_ERROR);
    return quque_handle;
}

void SimpleHttpServer::_register_url(const std::wstring url_to_listen, HANDLE h_requests_queue_handle)
{
    static const PVOID RESERVED = NULL;

    // CR: every HttpAddUrl needs to be cleaned up with HttpRemoveUrl.
    // This means... another class! 
    ULONG retCode = HttpAddUrl(
        h_requests_queue_handle,
        url_to_listen.c_str(),
        RESERVED
    );

    THROW_IF_NOT(NO_ERROR == retCode);
}

std::wstring SimpleHttpServer::reformat_domain_name(const std::wstring& domain_name, const std::wstring& port_number)
{
    // CR: once you have a StringUtils::format function, you should use it here
    static const std::wstring DOTS(L":");
    static const std::wstring  SLASH(L"/");
    return domain_name + DOTS + port_number + SLASH;
}


void SimpleHttpServer::wrap_buffer_with_html(BytesBufferPtr data_to_render)
{
    // CR: not sure how this works.. because wchar is not utf-8. Also, when returning file data you want to return the data as a download. 
    const char html_tempalte_start[] = "<html><head><meta charset=\"utf-8\"/><pre>";
    const char html_tempalte_end[] = "</pre></head> </html>";
    data_to_render->insert(data_to_render->begin(), html_tempalte_start, html_tempalte_start + sizeof(html_tempalte_start) - 1);
    data_to_render->insert(data_to_render->end(), html_tempalte_end, html_tempalte_end + sizeof(html_tempalte_start));

}

BytesBufferPtr SimpleHttpServer::handle_request(const HttpRequest& reference_request)
{
    THROW_IF_NOT(reference_request.get_http_request_type() == HttpVerbGET);

    std::wstring full_path_to_read = _server_root_path + reference_request.get_url_of_request();
    PathReader path_reader(full_path_to_read);
    BytesBufferPtr data_read = path_reader.read_now();
    wrap_buffer_with_html(data_read);
    return data_read;
}
