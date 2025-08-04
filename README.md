# TcpSocket
Welcome to the repository `TcpSocket`!
## What is this
An C++ API that can transmit `const char*` messages between the server and clients.
## How to use
1. Download the files `TcpSocket.h` and `TcpSocket.cpp`.
2. Copy them to your project.
3. `#include` `TcpSocket.h` in your own `.h` file.
4. Derive `TcpServer` or `TcpClient` `pubilc`ly in your own class.
5. Rewrite `ShowError(const char*)` to show error messages.
6. Rewrite `server()` or `client()` to deal with messages.You can use `RecvMsg()` and `SendMsg()`.
7. Call your own class .`work()` in `main()`.

**Note:**
+ _Detailed requirements in wiki pages._
+ _Exemples in `/Example`._
