#ifndef SERVERPROCESS_HPP
# define SERVERPROCESS_HPP

// #include "./../HTTPMessageController/HTTPMessageController.hpp"
// #include "RequestMessageController.hpp"
#include "SocketController.hpp"
#include "KernelQueueController.hpp"
#include "ConfigController.hpp"
#include "ResponseMessageController.hpp"
#include <dirent.h>
#include <sys/stat.h>

# define TEMP_BUFSIZ 1024
# define ISCGI 1
# define ISNOTCGI NULL

extern ConfigController config;

class ServerProcess {
	public:
		static int				serverProcess(SocketController* Socket, KernelQueueController* Kqueue) {
			while (true) {
				// queue에 event 적용
				Kqueue->setPollingCount(
					kevent(Kqueue->getKq(),
					Kqueue->getChangeList(),
					Kqueue->getChangeCount(),
					Kqueue->getEventList(),
					BUFSIZ,
					NULL)
				);

				// 적용된 event 초기화
				Kqueue->clearChangeList();
				Kqueue->resetChangeCount();

				// queue내 남아있는 이벤트만큼 반복
				for (int i = 0; i < Kqueue->getPollingCount(); i++) {
					try {
						if (Kqueue->getEventList(i)->udata != ISCGI) {
							if (Kqueue->getEventList(i)->filter == EVFILT_READ) {
								// server read
								//요 ident가 fd number,점마를 찾으면 점마에 대한 동작함
								if ((int)Kqueue->getEventList(i)->ident == Socket->getSocketServer()) {
									Socket->run();
									fcntl(Socket->getSocketClient(), F_SETFL, O_NONBLOCK);
									Kqueue->setReadKqueue(Socket->getSocketClient());
									std::cout << "Server connect : [" << Socket->getSocketClient() << "]" << std::endl;
								}
								// TODO: file 크기가 큰 경우 나눠서 통신하는 기능 구현
								// client read
								else {
									// Socket, CGI
									int fd = Kqueue->getEventList(i)->ident;
									char buf[TEMP_BUFSIZ];
									int n;
									n = read(fd, buf, TEMP_BUFSIZ - 1);
									std::cout << "N : " << n << std::endl;
									if (n == -1) {
										throw ErrorHandler(__FILE__, __func__, __LINE__, "RECV ERROR");
									}
									else if (n == TEMP_BUFSIZ - 1) {
										buf[n] = '\0';
										Kqueue->sumMessage(fd, buf);
									}
									else {
										std::cout << "Client read : [" << fd << "]" << std::endl;
										buf[n] = '\0';
										Kqueue->sumMessage(fd, buf);
										if (Kqueue->addRequestMessage(fd)) // cgi 라면
											Kqueue->setWriteKqueue(fd, static_cast<void *>(ISCGI));
										else
											Kqueue->setWriteKqueue(fd, NULL);
										std::string temp = ResponseMessage::setResponseMessage(Kqueue->getRequestMessage(fd));
										Kqueue->saveResponseMessage(fd, temp);
									}
								}
							}

							// write
							else if (Kqueue->getEventList(i)->filter == EVFILT_WRITE) {
								// Socket, CGI
								int fd = Kqueue->getEventList(i)->ident;
								if (Kqueue->writeResponseMessage(fd, TEMP_BUFSIZ) != TEMP_BUFSIZ) {
									Kqueue->removeRequestMessage(fd);
									close(fd);
								}
							}
						}
						else if (Kqueue->getEventList(i)->udata == ISCGI) {
							// cgi
							if (Kqueue->getEventList(i)->filter == EVFILT_WRITE) {
							}

							else if (Kqueue->getEventList(i)->filter == EVFILT_READ) {
							}
						}
					}//try
					//루프 안에 try문을 하나씩 넣는다
					//그리해서 특정 fd에 문제가 생기면 
					//치명적이지 않을경우 해당 작업을 중단하고 에러 페이지를 뱉고
					//치명적인 경우엔 fd자체를 닫고 에러페이지를 
					//근데 한 클라이언트에 여러 fd가 오게 되는 것도 고려해야 하나?
					catch (const std::exception& err) {
						std::cerr << err.what() << std::endl;
						//일단은 에러가 났으니 해당 fd에 해당된 것들을 삭제한다
						if (err.getLevel() == CRIT) {
							//throw(__file__, __func__, __line__, getMsg, CRIT);
						}
						//	대충 critical일때
						else if (err.getLevel == NON_CRIT) {
							//아마 에러페이지 만들었다면 그것에 대하여 write하게 되겠지
						}
						//	대충 non-critical 아닐때
					}
				}
			}
		};
};

#endif
