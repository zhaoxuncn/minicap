#ifndef MINICAP_SIMPLE_SERVER_HPP
#define MINICAP_SIMPLE_SERVER_HPP

class SimpleServer {
public:
    SimpleServer();
    virtual ~SimpleServer();

    virtual int start(const char* sockname, unsigned int port);

    virtual int accept();

private:
    int mFd;
};

class UnixServer:public SimpleServer {
public:
    UnixServer();
    ~UnixServer();

    int start(const char* sockname, unsigned int port);

    int accept();
private:
    int mFd;
};


class TCPServer:public SimpleServer {
public:
    TCPServer();
    ~TCPServer();

    int start(const char* sockname, unsigned int port);

    int accept();
private:
    int mFd;
};

class ServerFactory{
public:
    ServerFactory();
    ~ServerFactory();
    static SimpleServer* Create(bool protocolTCP);
};

/*
 * 原始 simpleServer
 * */
//class SimpleServer {
//public:
//  SimpleServer();
//  ~SimpleServer();
//
//  int
//  start(const char* sockname);
//
//  int accept();
//
//private:
//  int mFd;
//};

#endif
