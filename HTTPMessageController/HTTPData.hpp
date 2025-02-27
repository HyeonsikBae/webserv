#ifndef HTTPDATA_HPP
# define HTTPDATA_HPP

#include <map>
#include <string>
#include <iostream>

class HTTPData {
	public:
		// Nginx Server Block num
		int										server_block;
		// server port & client IP
		int										server_port;
		std::string								client_ip;
		std::string								client_port;
		std::string								host_ip;
		std::string								host_port;
        // Start Line
		std::string								method;
		std::string								uri_dir;
		std::string								uri_file;
		std::string								query_string;
		std::string								file_extension;
		std::string								cgi_pass;
		std::string								cgi_extension;
		
		double									http_version;
		
		std::string								url_directory;
		// Header Field
		std::map<std::string, std::string>		header_field;

		// Message Body
		std::string								message_body;

		std::string								str_buffer;
		bool									is_buffer_write;
		int										status_code;

		bool        							is_cgi;
		bool									is_autoindex;
		bool									is_redirect;
		bool									is_error;

		int										client_body_size;

		int	getSBlock(void);

		HTTPData(int server_block, int server_port, std::string client_ip);
};

#endif
