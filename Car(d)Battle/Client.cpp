#include "Client.h"

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString(int errorMessageID)
{
    if (errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}



Client::Client(unsigned short port, int buffer_len)
{
    this->buffer_len = buffer_len;
    this->port = port;
    started = false;
    response_ready = false;
}

Client::~Client()
{
    if(started)
        WSACleanup();
}

bool Client::start()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa))
    {
        cerr << "WSAStartup failed.\n";
        return false;
    }
    started = true;
    return true;
}
bool Client::http_get(const std::string& address, std::string path)
{
    std::string sendbuf = "GET " + path + " HTTP/1.1\r\nHost: " + address + "\r\nConnection: close\r\n\r\n";
    return send_request(address, sendbuf);
}

bool Client::send_request(const std::string& address, const std::string& request)
{
    if (!started)
        return false;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);

    for (auto ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {

        // Create a SOCKET for connecting to server
        Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Socket == INVALID_SOCKET)
        {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        cout << "Connecting...\n";
        if (connect(Socket, ptr->ai_addr, ptr->ai_addrlen) != 0)
        {
            cout << "Could not connect";
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    send(Socket, request.c_str(), request.length(), 0);
    shutdown(Socket, SD_SEND);
    response_ready = false;
    response.clear();
    char* buffer = new char[buffer_len] {};

    while (recv(Socket, buffer, buffer_len - 1, 0))
    {
        response += buffer;
        ZeroMemory(buffer, buffer_len);
    }

    response_ready = true;
    delete[] buffer;
    closesocket(Socket);
    return false;
}

std::string& Client::get_response()
{
    return response;
}

bool Client::is_response_ready()
{
    return response_ready;
}
