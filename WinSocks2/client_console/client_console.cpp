#include <network_library.h>

#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <array>
#include <vector>
#include <limits>

extern "C"
{
	char g_extern_buffer[c_buffer_length] = { 0 };
	std::mutex g_extern_buffer_mutex;
	int g_extern_buffer_length = 0;
}

enum class connect_thread_state
{
	Initializing,
	Connecting,
	Connected,
	Disconnected,
	Finalizing
};
using CTS = connect_thread_state;

class connect_thread
{
	//std::vector<SOCKET> local_sockets;
	//auto local_infos = get_addrinfo("", "48000");
	//std::for_each(local_infos.begin(), local_infos.end(), 
	//	[&](auto element) { local_sockets.emplace_back(socket(element->ai_family, element->ai_socktype, element->ai_protocol)); });
	//auto server_infos = get_addrinfo("192.168.1.3", "48000", AF_INET);
	//ADDRINFO* server_addrinfo = (server_infos.size() == 1) ? server_infos[0] : throw;

	SOCKET connect_socket = { 0 };
	SOCKADDR_IN connect_sockaddr = { 0 };

	int socket_connect()
	{
		std::cout << "[WinSock API] Connecting..." << std::endl;
		// connect returns 0 if successful
		if (connect(connect_socket, (SOCKADDR*)&connect_sockaddr, sizeof(connect_sockaddr)) == SOCKET_ERROR)
		{
			auto wsa_rc = wsa_get_last_error();
			return 1;
		}

		return 0;
	}
public:
	void operator()(std::future<void> future, std::wstring server)
	{
		CTS cts = CTS::Initializing;

		WSADATA wsa_data = { 0 };
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) throw;

		while (true)
		{
			if (cts == CTS::Initializing)
			{
				if ((connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) throw;

				connect_sockaddr.sin_family = AF_INET;
				connect_sockaddr.sin_port = htons(USHORT(48000));

				if (InetPtonW(AF_INET, server.c_str(), &connect_sockaddr.sin_addr.s_addr) == SOCKET_ERROR) auto wsa_rc = wsa_get_last_error();

				cts = CTS::Connecting;
			}

			if (cts == CTS::Connecting)
			{
				while (socket_connect())
				{
					// This is an additional wait on top of the blocking connect call timeout
					if (future.wait_for(std::chrono::milliseconds(c_wsa_polling_rate)) != std::future_status::timeout)
					{
						break;
					}
				}
				std::cout << "[WinSock API] Connected!" << std::endl;

				cts = CTS::Connected;
			}

			if (cts == CTS::Connected)
			{
				while (future.wait_for(std::chrono::milliseconds(c_wsa_polling_rate)) == std::future_status::timeout)
				{
					// Using non-blocking I/O, poll for send and recv

					auto argp = 1UL;
					if (ioctlsocket(connect_socket, FIONBIO, (u_long*)&argp) == SOCKET_ERROR) throw;

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
							if ((buffer_sent = send(connect_socket, &send_buffer[buffer_index], buffer_left, 0)) == SOCKET_ERROR)
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

					char recv_buffer[c_buffer_length] = { 0 };
					int recv_buffer_length = c_buffer_length;

					int buffer_recv = 0;
					if ((buffer_recv = recv(connect_socket, recv_buffer, c_buffer_length, 0)) == SOCKET_ERROR)
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

							//g_extern_buffer_mutex.lock();
							//std::memcpy(g_extern_buffer, recv_buffer, std::strlen(recv_buffer));
							//g_extern_buffer_mutex.unlock();

							std::cout << "[WinSock API] Buffer echo: \"" << recv_buffer << "\"" << std::endl;
							std::memset(recv_buffer, 0, sizeof(recv_buffer));
						}
						else
						{
							std::cout << "[WinSock API] Got a " << buffer_recv << " size buffer" << std::endl;
							std::cout << "[WinSock API] Shutting down connection..." << std::endl;
							break;
						}
					}
				}

				shutdown(connect_socket, SD_BOTH); // SD_SEND, SD_RECEIVE, SD_BOTH

				cts = CTS::Disconnected;
			}

			if (cts == CTS::Disconnected)
			{
				closesocket(connect_socket);

				cts = CTS::Finalizing;
			}

			if (cts == CTS::Finalizing)
			{
				connect_socket = { 0 };

				if (future.wait_for(std::chrono::milliseconds(150)) != std::future_status::timeout) break;

				cts = CTS::Initializing;
			}
		}

		if (WSACleanup() == SOCKET_ERROR) throw;
	}
};

template <typename T>
struct node
{
	std::shared_ptr<node<T>> root;
	std::shared_ptr<node<T>> parent;
	std::vector<std::unique_ptr<node<T>>> children;
	T value;
};
struct parse_node
{
	void operator()(char c)
	{
		switch (c)
		{
		case '{': // generic object that has members
		case '[': // collection of values
		case ':': // key:value pair separator
		case '.': // floating point
		case ',': // value, member separator
		case ']': // end collection
		case '}': // end object
		default:
			// Anything else is a char/string value
			return;
		}
	}
};

struct msg
{
	int seq = 0;
};
struct msg_0 : msg // Targeting ring sequence description
{
	const int msg = 0;
	std::array<int, 7> dat = { 0, 1, 2, 3, 4, 5, 6 };
};
struct msg_1 : msg // Player collection description
{
	const int msg = 1;
	std::array<int, 5> dat = { 0, 0, 0, 0, 0 }; // Player, collection, color, count, shield
};
struct msg_2 : msg // Gem choice description
{
	const int msg = 2;
	std::array<int, 2> dat = { 0, 0 }; // Left, Right
};
struct msg_3 : msg // Collection and gem selection response
{
	const int msg = 3;
	std::array<int, 2> dat = { 0, 0 }; // Collection (0-4), Selection (l/r/7)
};
struct msg_4 : msg // Invalid data message
{
	const int msg = 4;
};

int main(int argc, char* argv[])
{
	auto x = std::string{ "\0" };

	std::cout << "Welcome to Glitter Greed!" << std::endl << std::endl;
	std::cout << "Connect? (y/n) ";
	std::cin >> x;

	if (x != "y")
	{
		std::cout << "Why are you here? Bye." << std::endl;
		return 0;
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::promise<void> p;
	std::thread t{ connect_thread(), std::move(p.get_future()), L"73.210.118.242" };

	while (true)
	{
		x = "";
		std::getline(std::cin, x);

		if (x == "x") break;

		g_extern_buffer_mutex.lock();
		std::strcpy(g_extern_buffer, x.c_str());
		g_extern_buffer_length = int(x.length());
		g_extern_buffer_mutex.unlock();

		std::cin.clear();
	}

	std::cout << "Signaling Socket Thread to Shut Down." << std::endl;
	p.set_value();

	// Client is only in control of connecting to or disconnecting from server
	// Once client is connected, client state is controlled by server

	// Client send {msg:3,seq:0,dat:collection,socket,color} // Send desired update
	// Server recv {msg:4,dat:0} // Invalid selection loop input again

	// player: 0, 1, 2, 3
	// collection: 0, 1, 2, 3, 4
	// socket: 0, 1, 2, 3, 4 = shield
	// color: 0, 1, 2, 3, 4, 5, 6, 7 = shield, 8 = health

	// Game has begun, get state data from server
	// State data is delivered, get action state from server (poll for now?)
	// Is either Your Turn or Other Turn
	// If other turn, allow chat echo
	// If your turn, get gems from server
	// Parse l/r and # input for gems, deliver choices to server
	// Wait for results, render results
	// Go back to get action state

	//std::thread::hardware_concurrency()


	t.join();
	return 0;
}
