#ifndef _EPOLLS_HPP_
#define _EPOLLS_HPP_

#define BUF_SIZE 1024

int createSocket();
int registerToEpoll(int);
void startEpollWait(int, int);


#endif //_MY_EPOLL_HPP_
