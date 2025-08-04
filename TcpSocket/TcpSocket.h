#pragma once

#pragma warning (push)
#pragma warning (disable: 4668)
#pragma warning (disable: 4820)
#pragma warning (disable: 5039)

#include <thread>
#include <map>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#pragma warning (pop)

namespace
{
	class TcpSocket
	{
	protected:
		TcpSocket(int argc, char** argv) noexcept;
		~TcpSocket();
		TcpSocket(TcpSocket&&) = delete;
		TcpSocket(const TcpSocket&) = delete;
		TcpSocket& operator=(const TcpSocket&) = delete;
		TcpSocket& operator=(TcpSocket&&) = delete;

	private:
		bool InitSocket() const noexcept;
		bool CloseSocket() const noexcept;

	protected:
		bool CreateSocket() noexcept;
		bool RecvMsg(const SOCKET fd, char* buf, const size_t bufSize) noexcept;
		bool SendMsg(const SOCKET fd, const char* msg) const noexcept;
		virtual void ShowError(const char* msg) const noexcept = 0;

	protected:
		SOCKET m_fd;
		std::map<SOCKET, sockaddr_in*> m_pClientSocketInfo;

	protected:
		int m_argc;
	private:
		int useless = 0;
	protected:
		char** m_argv;
	};
}

class TcpServer : protected TcpSocket
{
protected:
	TcpServer(int argc, char** argv) noexcept;
	~TcpServer() = default;
	TcpServer(TcpServer&&) = delete;
	TcpServer(const TcpServer&) = delete;
	TcpServer& operator=(const TcpServer&) = delete;
	TcpServer& operator=(TcpServer&&) = delete;

private:
	bool BindSocket() const noexcept;
	bool ListenSocket(int clientNumber) const noexcept;
	bool AcceptConnect();
	void ServerWorking(SOCKET cfd) noexcept;

protected:
	virtual void ShowError(const char* msg) const noexcept = 0;

public:
	int work(const int clientNumber);

protected:
	virtual void server(int argc, char** argv) = 0;
};

class TcpClient : protected TcpSocket
{
protected:
	TcpClient(int argc, char** argv) noexcept;
	~TcpClient() = default;
	TcpClient(TcpClient&&) = delete;
	TcpClient(const TcpClient&) = delete;
	TcpClient& operator=(const TcpClient&) = delete;
	TcpClient& operator=(TcpClient&&) = delete;

private:
	bool ConnectServerSocket();
	void ClientWorking() noexcept;

protected:
	virtual void ShowError(const char* msg) const noexcept = 0;

public:
	int work();

protected:
	virtual void client(int argc, char** argv) = 0;
};
