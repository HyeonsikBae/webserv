
#ifndef CGISESSION_H
#define CGISESSION_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <unistd.h>
#include <signal.h>
#include "../ErrorHandler/ErrorHandler.hpp"
#include "../HTTPMessageController/HTTPMessageController.hpp"

// class RequestMessage;

class CGIProcess {
    private:
        char** _env;
        char** _arg;
        pid_t _pid;
        int _inputPair[2];
        int _outputPair[2];

        char    **generateEnvp(std::map<std::string, std::string> env)
        {
            char    **envp = new char*[env.size() + 1];
            int i = 0;
            std::map<std::string, std::string>::iterator it;
            for (it = env.begin(); it != env.end(); ++it)
            {
                envp[i] = new char[(it->first.length() + it->second.length() + 2)];
                strcpy(envp[i], it->first.c_str());
                strcat(envp[i], "=");
                strcat(envp[i], it->second.c_str());
                i++;
            }
            envp[i] = NULL;
            return envp;
        }

        void    setCGIarg() {
            this->_arg = new char*[4];

            this->_arg[0] = new char[22];
            strcpy(_arg[0], "./cgiBinary/php-cgi");  // cgi file 경로. config 파일에서 파싱해서 사용
            
			this->_arg[1] = new char[25];
            strcpy(_arg[1], "./cgiBinary/sample.php");  // 실행할 파일 경로. request message의 uri에서 파싱해서 사용
            
            this->_arg[2] = new char[10];
            strcpy(_arg[2], "var=1234");
            /*
            querystring 값
            if (requestMessage->_URIQueryString.empty())
                _arg[2] = NULL;
            else
                _arg[2] = const_cast<char*>(requestMessage->_URIQueryString.c_str());
            */
            _arg[3] = NULL;
        }
    public:
        void setEnvp(RequestMessage* requestMessage)
        {
            std::map<std::string, std::string> _envMap;

            // local 환경변수에 이미 존재하는 아이들
            _envMap[std::string("USER")] = std::string(std::getenv("USER"));
            _envMap[std::string("PATH")] = std::string(std::getenv("PATH"));
            _envMap[std::string("LANG")] = std::string(std::getenv("LANG"));
            _envMap[std::string("PWD")] = std::string(std::getenv("PWD"));

            // parsing으로 가져온 아이들
            _envMap[std::string("REQUEST_METHOD")] = requestMessage->getMethod();
            // _envMap[std::string("PATH_INFO")] = requestMessage->getUriDir(); // /data/index.html
            _envMap[std::string("PATH_INFO")] = std::string("/index.html");
            _envMap[std::string("SERVER_PROTOCOL")] = std::string("HTTP/1.1");
            _envMap[std::string("REQUEST_SCHEME")] = requestMessage->getMethod();
            _envMap[std::string("GATEWAY_INTERFACE")] = std::string("CGI/1.1");
            _envMap[std::string("SERVER_SOFTWARE")] = std::string("webserv/1.0");

            // _envMap[std::string("CONTENT_TYPE")] = requestMessage->_reqContentType;
            _envMap[std::string("CONTENT_TYPE")] = "application/x-www-form-urlencoded";
            // _envMap[std::string("CONTENT_LENGTH")] = requestMessage->_reqContentLength;
            _envMap[std::string("CONTENT_LENGTH")] = std::to_string(22);

            // _envMap[std::string("REMOTE_ADDR")] = requestMessage->_clientIP; // server socket addr
            // _envMap[std::string("REMOTE_PORT")] = requestMessage->_clientPort; // client port
            // _envMap[std::string("SERVER_ADDR")] = requestMessage->_hostIP; // client socket addr
            // _envMap[std::string("SERVER_PORT")] = requestMessage->_hostPort; // host port

            _envMap[std::string("QUERY_STRING")] = requestMessage->getQueryString();

            _envMap[std::string("SCRIPT_NAME")] = requestMessage->getUriFile();

            // _envMap[std::string("REQUEST_URI")] = requestMessage->_originURI;
            _envMap[std::string("DOCUMENT_ROOT")] = "./cgiBinary";
            _envMap[std::string("REQUEST_URI")] = "./cgiBinary/sample.php";
            _envMap[std::string("DOCUMENT_URI")] = "./cgiBinary/sample.php";
            // _envMap[std::string("SERVER_NAME")] = requestMessage->_serverName; // config 파일의 서버 이름
            _envMap[std::string("SCRIPT_FILENAME")] = "./cgiBinary/sample.php";

            // for (std::map<std::string, std::string>::iterator iter = requestMessage->_HTTPCGIENV.begin(); iter != requestMessage->_HTTPCGIENV.end(); iter++) {
            //     _envMap[iter->first] = iter->second;
            // } // CGI binary 파일의 헤더 (HTTP_)
            // request의 헤더가 cgi 의 환경변수로 들어가야 한다.
            
            _env = generateEnvp(_envMap);
            setCGIarg();
        }

        ~CGIProcess()
        {
            int status;
            if (_pid > 0) {
                waitpid(_pid, &status, WNOHANG);
                if (status & 0177) {
                    kill(_pid, SIGTERM);
                }
            }
            // if (getInputPair() > 0 && close(getInputPair()) == -1) {
            //     throw ErrorHandler("File Descriptor closing Error3.");
            // }
            // if (getOutputPair() > 0 && close(getOutputPair()) == -1) {
            //     throw ErrorHandler("File Descriptor closing Error.");
            // }
            if (_env != NULL) {
                int i = 0;
                while (_env[i] != NULL) {
                    delete _env[i];
                    i++;
                }
                delete _env;
            }
        }

        int& getInputPair(void)
        {
            return (_inputPair[1]);
        }

        int& getOutputPair(void)
		{
            return (_outputPair[0]);
        }

        void CGIprocess(void)
        {
            if (pipe(this->_inputPair) == -1 || pipe(this->_outputPair) == -1) {
				throw ErrorHandler(__FILE__, __func__, __LINE__, "Pipe Making Erro.r");
            }
            if ((_pid = fork()) < 0) {
                throw ErrorHandler(__FILE__, __func__, __LINE__, "Process Making Error.");
            }
            if (_pid == 0) {
                if ((dup2(this->_inputPair[0], STDIN_FILENO) == -1) || (dup2(this->_outputPair[1], STDOUT_FILENO) == -1)) {
                    throw ErrorHandler(__FILE__, __func__, __LINE__, "duplicate File Descriptor Error.");
                }
                if ((close(this->_inputPair[1]) == -1) || (close(this->_outputPair[0]) == -1)) {
                    throw ErrorHandler(__FILE__, __func__, __LINE__, "File Descriptor closing Error1.");
                }
                if (execve(this->_arg[0], _arg, _env) == -1) {
                    throw ErrorHandler(__FILE__, __func__, __LINE__, "execve File Descriptor Error.");
                }
            } else {
                if ((close(this->_inputPair[0]) == -1) || (close(this->_outputPair[1]) == -1)) {
                    throw ErrorHandler(__FILE__, __func__, __LINE__, "File Descriptor closing Error2.");
                }
            }
        }
};

#endif

