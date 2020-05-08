#pragma once
#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>

class Client
{
	bool started, response_ready;
	unsigned short port;
	int buffer_len;
	SOCKET Socket;
	ADDRINFOA hints, * result;
	std::string response;
public:
	Client(unsigned short, int);
	~Client();
	bool start();
	bool http_get(const std::string&, std::string);
	bool send_request(const std::string&, const std::string&);
	std::string& get_response();
	bool is_response_ready();
	bool is_ready();
};
