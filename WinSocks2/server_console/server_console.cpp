#include <network_library.h>

#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <tuple>
#include <array>
#include <queue>
#include <iterator>

extern "C"
{
	char g_extern_buffer[c_buffer_length] = { 0 };
	std::mutex g_extern_buffer_mutex;
	int g_extern_buffer_length = 0;
}

struct server_client_socket
{
	SOCKET client_socket;
	HANDLE player_object;
	WSABUF inbound_buf;
	WSABUF outbound_buf;
	std::string inbound_str;
	std::string outbound_str;
};

struct client_thread
{
	void operator()(std::future<void> future, SOCKET s)
	{
		while (true)
		{
			char recv_buffer[c_buffer_length] = { 0 };
			int recv_buffer_length = c_buffer_length;

			int buffer_recv = 0;
			if ((buffer_recv = recv(s, recv_buffer, c_buffer_length, 0)) == SOCKET_ERROR)
			{
				auto wsa_rc = wsa_get_last_error();
				if (wsa_rc != WSAEWOULDBLOCK) throw; // If wsa_rc is WSAEWOULDBLOCK, fall through and loop
				std::this_thread::sleep_for(std::chrono::milliseconds(c_wsa_polling_rate / 10));
			}
			else
			{
				if (buffer_recv > 0)
				{
					std::cout << "[WinSock API] Received " << buffer_recv << " bytes." << std::endl;

					g_extern_buffer_mutex.lock();
					std::memcpy(g_extern_buffer, recv_buffer, std::strlen(recv_buffer));
					g_extern_buffer_mutex.unlock();

					if (std::strcmp(g_extern_buffer, "") != 0)
					{
						std::cout << "[WinSock API] Buffer echo: \"" << recv_buffer << "\"" << std::endl;
					}
					std::memset(recv_buffer, 0, sizeof(recv_buffer));
				}
				else
				{
					std::cout << "[WinSock API] Got a " << buffer_recv << " size buffer" << std::endl;
					std::cout << "[WinSock API] Shutting down connection..." << std::endl;
					shutdown(s, SD_BOTH);
					closesocket(s);
					return;
				}
			}

			if (*g_extern_buffer != 0)
			{
				char send_buffer[c_buffer_length] = { 0 };
				int send_buffer_length = 0;

				g_extern_buffer_mutex.lock();
				std::memcpy(send_buffer, g_extern_buffer, std::strlen(g_extern_buffer));
				send_buffer_length = std::strlen(g_extern_buffer);

				int buffer_left = send_buffer_length;
				int buffer_index = 0;

				while (buffer_left > 0)
				{
					int buffer_sent = 0;
					if ((buffer_sent = send(s, &send_buffer[buffer_index], buffer_left, 0)) == SOCKET_ERROR)
					{
						auto wsa_rc = wsa_get_last_error();
						if (wsa_rc != WSAEWOULDBLOCK) throw;
						std::this_thread::sleep_for(std::chrono::milliseconds(c_wsa_polling_rate / 10));
					}
					else
					{
						std::cout << "[WinSock API] Sent " << buffer_sent << " bytes." << std::endl;
					}
					buffer_left -= buffer_sent;
					buffer_index += buffer_sent;
				}

				std::memset(g_extern_buffer, 0, sizeof(char) * c_buffer_length);
				g_extern_buffer_mutex.unlock();

				std::memset(send_buffer, 0, sizeof(send_buffer));
			}
		}
	}
};

const unsigned long long c_max_threads = 64;
const unsigned long long c_num_threads = 8;

class listen_thread
{
	//ADDRINFO local_hints = { 0 };
	//local_hints.ai_flags = AI_PASSIVE;
	//local_hints.ai_family = AF_INET; // AF_UNSPEC, AF_INET, AF_INET6
	//local_hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM, SOCK_DGRAM
	//local_hints.ai_protocol = IPPROTO_TCP; // IPPROTO_TCP, IPPROTO_UDP
	//ADDRINFO* local_addrinfo = NULL;
	//if (getaddrinfo("", "48000", &local_hints, &local_addrinfo) == WSANO_DATA) throw;
	//freeaddrinfo(local_addrinfo);

	std::tuple<int, SOCKET, SOCKADDR_IN> socket_accept(SOCKET listen_socket)
	{
		SOCKADDR_IN client_sockaddr = { 0 };
		int client_sockaddr_size = sizeof(SOCKADDR_IN);
		auto client_socket = accept(listen_socket, (SOCKADDR*)&client_sockaddr, &client_sockaddr_size);

		//auto argp = 1UL;
		//if (ioctlsocket(client_socket, FIONBIO, (u_long*)&argp) == SOCKET_ERROR) throw;

		int wsa_rc = 0;
		if (client_socket == INVALID_SOCKET) wsa_rc = wsa_get_last_error();
		if (wsa_rc != WSAEWOULDBLOCK)
		{
			std::cout << "[WinSock API] Connection accepted! client_sockaddr_size = " << client_sockaddr_size << std::endl;
			return std::make_tuple(1, client_socket, client_sockaddr);
		}

		return std::make_tuple(0, NULL, client_sockaddr);
	}
public:
	void operator()(std::future<void> future)
	{
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) throw;

		SOCKET listen_socket = { 0 };
		if ((listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) throw;

		auto argp = 1UL;
		if (ioctlsocket(listen_socket, FIONBIO, (u_long*)&argp) == SOCKET_ERROR) throw;

		SOCKADDR_IN listen_sockaddr = { 0 };
		listen_sockaddr.sin_family = AF_INET;
		listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		listen_sockaddr.sin_port = htons(USHORT(48000));

		if (bind(listen_socket, (SOCKADDR*)&listen_sockaddr, sizeof(listen_sockaddr)) == SOCKET_ERROR) auto wsa_rc = wsa_get_last_error();
		if (listen(listen_socket, c_num_threads) == SOCKET_ERROR) auto wsa_rc = wsa_get_last_error();

		while (future.wait_for(std::chrono::milliseconds(c_wsa_polling_rate)) == std::future_status::timeout)
		{
			auto [success, s, sin] = socket_accept(listen_socket);
			if (success)
			{
				//old_sso->s = std::move(s);
				//old_sso->sin = std::move(sin);

				//CreateIoCompletionPort((HANDLE)old_sso->s, completion_port, (ULONG_PTR)&old_sso, 0);

				std::promise<void> p;
				std::thread t{ client_thread(), std::move(p.get_future()), s };
				t.detach();
			}
		}

		shutdown(listen_socket, SD_BOTH);
		closesocket(listen_socket);

		//for (auto& element : workers)
		//{
		//	element.p.set_value();
		//	element.t.join();
		//}

		if (WSACleanup() == SOCKET_ERROR) throw;
	}
};





int main(int argc, char* argv[])
{
	std::promise<void> p;
	std::thread t{ listen_thread(), std::move(p.get_future()) };

	std::cout << "Glitter Greed Server" << std::endl;

	std::string x = { 0 };
	while (true)
	{
		std::cin >> x;
		if (x == "x") break;
	}

	p.set_value();
	t.join();

	return 0;
}
