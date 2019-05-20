#include "SimpleServer.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "util/debug.h"


SimpleServer::SimpleServer(): mFd(0) {
}

SimpleServer::~SimpleServer() { 
  if (mFd > 0) {
    ::close(mFd);
  }
}

int
SimpleServer::start(const char* sockname, unsigned int port){
    return 0;
}

int
SimpleServer::accept() {
	return 0;
}

UnixServer::UnixServer(): mFd(0) {
}

UnixServer::~UnixServer() {
    if (mFd > 0) {
        ::close(mFd);
    }
}

int
UnixServer::start(const char* sockname, unsigned int portTCP){
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sfd < 0) {
    return sfd;
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(&addr.sun_path[1], sockname, strlen(sockname));

  if (::bind(sfd, (struct sockaddr*) &addr,
      sizeof(sa_family_t) + strlen(sockname) + 1) < 0) {
    ::close(sfd);
    return -1;
  }

  ::listen(sfd, 1);

  mFd = sfd;
    MCINFO("Listen at unix named %s", sockname);
  return mFd;
}

int
UnixServer::accept() {
    struct sockaddr_un addr;
  socklen_t addr_len = sizeof(addr);
  return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
}

TCPServer::TCPServer(): mFd(0) {
}

TCPServer::~TCPServer() {
    if (mFd > 0) {
        ::close(mFd);
    }
}

int
TCPServer::start(const char* sockname, unsigned int port){
    int sfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sfd < 0) {
		return sfd;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (::bind(sfd, (struct sockaddr*) &addr,sizeof(struct sockaddr)) < 0) {
		::close(sfd);
		return -1;
	}

	if (::listen(sfd, 1) < 0) {
		return -1;
	}

	mFd = sfd;
    MCINFO("Listen at port %d", port);
	return mFd;
}

int
TCPServer::accept() {
    struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
}


ServerFactory::ServerFactory(){
}

ServerFactory::~ServerFactory() {
}

SimpleServer * ServerFactory::Create(bool protocolTCP) {
    if (protocolTCP) {
        return new TCPServer();
    }
    return new UnixServer();
}

/*
 * 使用 AF_INET 的模式来实现通信
 * */
//int
//SimpleServer::start(const char* sockname) {
//	int sfd = socket(AF_INET, SOCK_STREAM, 0);
//
//	if (sfd < 0) {
//		return sfd;
//	}
//
//	struct sockaddr_in addr;
//	memset(&addr, 0, sizeof(addr));
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(14636);
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//	if (::bind(sfd, (struct sockaddr*) &addr,sizeof(struct sockaddr)) < 0) {
//		::close(sfd);
//		return -1;
//	}
//
//	if (::listen(sfd, 1) < 0) {
//		return -1;
//	}
//
//	mFd = sfd;
//	return mFd;
//}
//
//int
//SimpleServer::accept() {
//	struct sockaddr_in addr;
//	socklen_t addr_len = sizeof(addr);
//	return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
//}


/*
 * 原始的代码： 使用 AF_UNIX模式来通信
 *
 * */

//int
//SimpleServer::start(const char* sockname) {
//  int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
//
//  if (sfd < 0) {
//    return sfd;
//  }
//
//  struct sockaddr_un addr;
//  memset(&addr, 0, sizeof(addr));
//  addr.sun_family = AF_UNIX;
//  strncpy(&addr.sun_path[1], sockname, strlen(sockname));
//
//  if (::bind(sfd, (struct sockaddr*) &addr,
//      sizeof(sa_family_t) + strlen(sockname) + 1) < 0) {
//    ::close(sfd);
//    return -1;
//  }
//
//  ::listen(sfd, 1);
//
//  mFd = sfd;
//
//  return mFd;
//}
//
//int
//SimpleServer::accept() {
//  struct sockaddr_un addr;
//  socklen_t addr_len = sizeof(addr);
//  return ::accept(mFd, (struct sockaddr *) &addr, &addr_len);
//}
