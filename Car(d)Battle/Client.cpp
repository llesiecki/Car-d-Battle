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
    :result(nullptr), hints({}), buffer_len(buffer_len), port(port)
{
    started = false;
    response_ready = false;
    Socket = INVALID_SOCKET;
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
        std::cerr << "WSAStartup failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
        return false;
    }
    started = true;
    return true;
}
bool Client::http_get(const std::string& address, const std::string& path)
{
    std::string sendbuf = "GET " + path + " HTTP/1.1\r\nHost: " + address + "\r\nConnection: close\r\n\r\n";
    return send_request(address, sendbuf);
}

bool Client::send_request(const std::string& address, const std::string& request)
{
    if (!started)
    {
        if(!start())
            throw std::runtime_error("Couldn't start network client.\n");
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if(getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result))
        std::cerr << "getaddrinfo failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";

    auto ptr = result;

    do
    {
        // Create a SOCKET for connecting to server
        Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Socket == INVALID_SOCKET)
        {
            std::cerr << "socket failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
            continue;
        }

        // Connect to server.
        if (connect(Socket, ptr->ai_addr, ptr->ai_addrlen))
        {
            std::cerr << "connect failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
            Socket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    while (ptr = ptr->ai_next);

    if (!ptr)
    {
        std::cerr << "connect failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
        return false;
    }

    freeaddrinfo(result);

    if(Socket == INVALID_SOCKET)
        std::cerr << "socket failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";

    if(send(Socket, request.c_str(), request.length(), 0) == SOCKET_ERROR)
        std::cerr << "send failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";

    if(shutdown(Socket, SD_SEND) == SOCKET_ERROR)
        std::cerr << "shutdown failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
    response_ready = false;
    response.clear();

    int bytes_received;
    char* buffer = new char[buffer_len];

    do
    {
        ZeroMemory(buffer, buffer_len);
        bytes_received = recv(Socket, buffer, buffer_len - 1, 0);
        response += buffer;
        if (bytes_received < 0)
        {
            std::cerr << "recv failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
            break;
        }

    } while (bytes_received);

    response_ready = true;
    delete[] buffer;
    if(closesocket(Socket))
        std::cerr << "closesocket failed: " << GetLastErrorAsString(WSAGetLastError()) << "\n";
    return true;
}

std::string& Client::get_response()
{
    return response;
}

bool Client::is_response_ready()
{
    return response_ready;
}

bool Client::is_ready()
{
	return started;
}
