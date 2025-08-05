# TcpSocket
Welcome to the repository `TcpSocket`! 
## What is it
TcpSocket is a C++ API that can transmit `const char*` messages between the server and clients.
## What is it like
### TcpSocket:
+ have interfaces very easy to use
+ 

### But it still:
- 

## How to use it
1. Download the files `TcpSocket.h` and `TcpSocket.cpp`. 
2. Copy them to your project. 
3. `#include` `TcpSocket.h` in your own `.h` file. 
4. Derive `TcpServer` or `TcpClient` `pubilc`ly in your own class. 
5. Rewrite `virtual void ShowError(const char* msg) const noexcept` to show error messages. 
6. Rewrite `virtual void server(int argc, char** argv)` or `virtual void client(int argc, char** argv)` to deal with messages. You can use `bool RecvMsg(const SOCKET fd, char* buf, const size_t bufSize) noexcept` and `bool SendMsg(const SOCKET fd, const char* msg) const noexcept`. 
7. Call `your own class.work()` in `main()`. 
  
_**Note:**_  
+ _Exemples in [`/Examples`](https://github.com/U-235238/TcpSocket/tree/1.0-Beta/Examples). _
