#!/bin/bash

#export CONTENT_LENGTH=30
export CONTENT_TYPE="php-cgi"
export DOCUMENT_ROOT="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
export DOCUMENT_URI="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
#export GATEWAY_INTERFACE="CGI/1.1"
export LANG="ko_KR.UTF-8"
export PATH="usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
export PATH_INFO="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
export PWD="Users/mac/Documents/websv/webserv/juhpark/mmm/new/CGI"
export QUERY_STRING="123=abc"
export REMOTE_ADDR="127.0.0.1"
#export REQUEST_METHOD="GET"
export REQUEST_SCHEME="GET"
export REQUEST_URI="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
export SCRIPT_FILENAME="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
export SCRIPT_NAME="Users/mac/Documents/websv/webserv/juhpark/mmm/new/cgiBinary/php-cgi"
export SERVER_PORT=4242
export SERVER_PROTOCOL="HTTP/1.1"
#export SERVER_SOFTWARE="webserv/1.0"
export USER="juhpark"


#echo "pval=1234&name=asd" | ../cgiBinary/php-cgi "val=value"
./../cgiBinary/php-cgi ./../cgiBinary/sample.php "123=abc"
