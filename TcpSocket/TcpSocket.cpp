#include "TcpSocket.h"

constexpr int PORT = 32196;

#pragma warning(push)
#pragma warning(disable : 5245)
#pragma warning(disable : 26485)
#pragma warning(disable : 26493)

bool TcpSocket::InitSocket() const noexcept
{
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		ShowError("WSAStartup");
		return false;
	}
	return true;
}

bool TcpSocket::CloseSocket() const noexcept
{
	if (0 != WSACleanup())
	{
		ShowError("WSACleanup");
		return false;
	}
	return true;
}

bool TcpSocket::CreateSocket() noexcept
{
	const SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ((unsigned long long)-1 == fd)
	{
		ShowError("socket create");
		return false;
	}
	m_fd = fd;
	return true;
}

bool TcpSocket::RecvMsg(const SOCKET fd, char *buf, const size_t bufSize) noexcept
{
	if (INT_MAX < bufSize)
	{
		ShowError("buf too long");
		return false;
	}
	const int len = recv(fd, buf, (int)bufSize, 0);
	if (0 < len)
	{
		return true;
	}
	else if (0 == len)
	{
		if (closesocket(fd) == 0)
		{
			m_pClientSocketInfo.erase(fd);
		}
		return false;
	}
	else
	{
		ShowError("recv");
	}
	return false;
}

bool TcpSocket::SendMsg(const SOCKET fd, const char *msg) const noexcept
{
	if (INT_MAX < strlen(msg))
	{
		ShowError("msg too long");
		return false;
	}
	if (-1 == send(fd, msg, (int)strlen(msg) + 1, 0))
	{
		ShowError("send");
		return false;
	}
	return true;
}

TcpServer::TcpServer(int argc, char** argv)
	: TcpSocket(argc, argv)
{

}

bool TcpServer::BindSocket() const noexcept
{
	sockaddr_in saddr{};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (-1 == bind(m_fd, (sockaddr *)&saddr, sizeof(saddr)))
	{
		ShowError("bind");
		return false;
	}
	return true;
}

bool TcpServer::ListenSocket(const int clientNumber) const noexcept
{
	if (-1 == listen(m_fd, clientNumber))
	{
		ShowError("listen");
		return false;
	}
	return true;
}

bool TcpServer::AcceptConnect()
{
	int addrLen = sizeof(sockaddr_in);
	while (true)
	{
		sockaddr_in outAddr{};
		SOCKET cfd = accept(m_fd, (sockaddr *)&outAddr, &addrLen);
		if ((unsigned long long)-1 == cfd)
		{
			ShowError("accept");
			return false;
		}
		m_pClientSocketInfo.insert(std::pair<SOCKET, sockaddr_in *>(cfd, &outAddr));

		SendMsg(cfd, "Welcome to tcp auto trade");
		std::thread Worker(&TcpServer::ServerWorking, this, cfd);
		Worker.detach();
	}
}

void TcpServer::ServerWorking(SOCKET cfd) noexcept
{
	server(m_argc, m_argv);
	if (closesocket(cfd) == 0)
	{
		m_pClientSocketInfo.erase(cfd);
	}
	return;
}

int TcpServer::work(const int clientNumber)
{
	if (!CreateSocket())
	{
		return -1;
	}
	if (!BindSocket())
	{
		return -1;
	}
	if (!ListenSocket(clientNumber))
	{
		return -1;
	}
	if (!AcceptConnect())
	{
		return -1;
	}
	return 0;
}

TcpClient::TcpClient(int argc, char** argv)
	: TcpSocket(argc, argv)
{

}

bool TcpClient::ConnectServerSocket()
{
	struct sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.S_un.S_addr);

	if (-1 == connect(m_fd, (sockaddr *)&addr, sizeof(addr)))
	{
		ShowError("connect");
		return false;
	}

	std::thread ClientWork(&TcpClient::ClientWorking, this);
	ClientWork.detach();

	return true;
}

void TcpClient::ClientWorking() noexcept
{
	client(m_argc, m_argv);
	return;
}

int TcpClient::work() noexcept
{
	if (!CreateSocket())
	{
		return -1;
	}
	if (!ConnectServerSocket())
	{
		return -1;
	}
	return 0;
}

TcpSocket::TcpSocket(int argc, char** argv) noexcept
	: m_argc(argc), m_argv(argv)
{
	InitSocket();
}

TcpSocket::~TcpSocket()
{
	CloseSocket();
}

#pragma warning(pop)
