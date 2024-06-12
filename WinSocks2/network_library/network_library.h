#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include <vector>
#include <string>
#include <iostream>

constexpr auto c_buffer_length = 256ULL;
constexpr auto c_wsa_polling_rate = 50LL;

int wsa_get_last_error()
{
	switch (int wsa_ec = WSAGetLastError())
	{
	case WSAEWOULDBLOCK:
		// std::cerr << "[WinSock API] WSAEWOULDBLOCK" << std::endl;
		return wsa_ec;
	case WSAECONNABORTED:
		std::cerr << "[WinSock API] Connection aborted!" << std::endl;
		return wsa_ec;
	case WSAECONNRESET:
		std::cerr << "[WinSock API] Connection reset!" << std::endl;
		return wsa_ec;
	case WSAECONNREFUSED:
		std::cerr << "[WinSock API] Connection refused!" << std::endl;
		return wsa_ec;
	case WSAETIMEDOUT:
		std::cerr << "[WinSock API] Timed out!" << std::endl;
		return wsa_ec;
	case WSAEADDRINUSE:
		std::cerr << "[WinSock API] Address in use!" << std::endl;
		return wsa_ec;
	case 10038:
		std::cerr << "[Win32 API] An operation was attempted on something that is not a socket." << std::endl;
		return wsa_ec;
	case 10056:
		std::cerr << "[Win32 API] A connect request was made on an already connected socket." << std::endl;
		return wsa_ec;
	case 10049:
		std::cerr << "[Win32 API] The requested address is not valid in its context." << std::endl;
		return wsa_ec;
	case 10022:
		std::cerr << "[Win32 API] An invalid argument was supplied." << std::endl;
		return wsa_ec;
	case 10014:
		std::cerr << "[Win32 API] The system detected an invalid pointer address in attempting to use a pointer argument in a call." << std::endl;
		return wsa_ec;
	default:
		std::cerr << "[WinSock API] Error code: " << wsa_ec << std::endl;
		return wsa_ec;
	}
}

std::vector<ADDRINFO*> get_addrinfo(std::string hostname, std::string port)
{
	ADDRINFO ai_hints = { 0 };
	ai_hints.ai_flags = (hostname == "") ? AI_PASSIVE : 0;
	ai_hints.ai_family = AF_UNSPEC; // AF_UNSPEC, AF_INET, AF_INET6
	ai_hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM, SOCK_DGRAM
	ai_hints.ai_protocol = IPPROTO_TCP; // IPPROTO_TCP, IPPROTO_UDP

	ADDRINFO* ai_head = NULL;
	if (getaddrinfo(hostname.c_str(), port.c_str(), &ai_hints, &ai_head) == WSANO_DATA) throw std::exception("[WinSock API] getaddrinfo() returned no data!");

	std::vector<ADDRINFO*> ai_collection;
	ADDRINFO* ai_element = ai_head;
	while (ai_element != NULL)
	{
		ai_collection.push_back(ai_element);
		ai_element = ai_element->ai_next;
		ai_collection.back()->ai_next = NULL;
	}
	freeaddrinfo(ai_head);
	return std::move(ai_collection);
}
std::vector<ADDRINFO*> get_addrinfo(std::string hostname, std::string port, int family, int type, int protocol)
{
	ADDRINFO ai_hints = { 0 };
	ai_hints.ai_flags = (hostname == "") ? AI_PASSIVE : 0;
	ai_hints.ai_family = family; // AF_UNSPEC, AF_INET, AF_INET6
	ai_hints.ai_socktype = type; // SOCK_STREAM, SOCK_DGRAM
	ai_hints.ai_protocol = protocol; // IPPROTO_TCP, IPPROTO_UDP

	ADDRINFO* ai_head = NULL;
	if (getaddrinfo(hostname.c_str(), port.c_str(), &ai_hints, &ai_head) == WSANO_DATA) throw std::exception("[WinSock API] getaddrinfo() returned no data!");

	std::vector<ADDRINFO*> ai_collection;
	ADDRINFO* ai_element = ai_head;
	while (ai_element != NULL)
	{
		ai_collection.push_back(ai_element);
		ai_element = ai_element->ai_next;
	}
	freeaddrinfo(ai_head);
	return std::move(ai_collection);
}

ADDRINFO* get_addrinfo(std::string hostname, std::string port, int family)
{
	ADDRINFO ai_hints = { 0 };
	ai_hints.ai_flags = (hostname == "") ? AI_PASSIVE : 0;
	ai_hints.ai_family = family; // AF_UNSPEC, AF_INET, AF_INET6
	ai_hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM, SOCK_DGRAM
	ai_hints.ai_protocol = IPPROTO_TCP; // IPPROTO_TCP, IPPROTO_UDP

	ADDRINFO* ai_head = NULL;
	if (getaddrinfo(hostname.c_str(), port.c_str(), &ai_hints, &ai_head) == WSANO_DATA) throw std::exception("[WinSock API] getaddrinfo() returned no data!");
	return ai_head;

	//std::vector<ADDRINFO*> ai_collection;
	//ADDRINFO* ai_element = ai_head;
	//while (ai_element != NULL)
	//{
	//	ai_collection.push_back(ai_element);
	//	ai_element = ai_element->ai_next;
	//	ai_collection.back()->ai_next = NULL;
	//}
	//freeaddrinfo(ai_head);
	//return std::move(ai_collection);
}
