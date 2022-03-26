#include "HTTPData.hpp"

int	HTTPData::getSBlock(void) { return(this->server_block); }
		
HTTPData::HTTPData(int server_block, int server_port, std::string client_ip) {
	this->status_code = 200;
	this->server_block = server_block;
	this->server_port = server_port;
	this->client_ip = client_ip;
	this->is_autoindex = false;
	this->is_buffer_write = false;
	this->is_cgi = false;
	this->is_error = false;
	this->is_redirect = false;
	this->cgi_extension = "";
	this->cgi_pass = "";
}
