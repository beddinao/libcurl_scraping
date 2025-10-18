#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <fstream>
#define		RED "\x1B[31m"
#define		NRM "\x1B[0m"

size_t	write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(reinterpret_cast<std::string*>(userdata))->append(ptr, nmemb * size);
	return size*nmemb;
};

std::string	get_response(CURL *curl, std::string url) {
	std::string	res;
	//CURL		*curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:124.0) Gecko/20100101 Firefox/124.0");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
	if (curl_easy_perform(curl))	res = "failure";
	//curl_easy_cleanup(curl);
	return	res;
}
/*
std::string	clean_up_stuff(std::string input, std::string garbage, std::string target) {
	if (input.empty()||garbage.empty()||garbage.size() != target.size())	return "";
	size_t				pos;
	for (size_t i=0; i < garbage.size(); i++)
	{
		std::string	shgarbage(1, garbage[i]);
		while (true) {
			pos = input.find(shgarbage);
			if (pos == input.npos)	break;
			std::string	sec_tar(1, target[i]);
			input.replace(pos, 1, sec_tar);
		}
	}
	return	input;
}

std::string	_generate_random_string(std::string seed, int length) {
	std::string	output = seed;
	for (int i=0;i<length;i++) {
		std::string    r(1, static_cast<char>(std::rand() % (122 - 48) + 48 ));
		output.append( r );
	}
	return	clean_up_stuff(output, "[\\]^`:;<>=?/ ", "_____________");
}

std::vector<std::string>		generate_random_string(int num) {
	std::vector<std::string>		results;
	for (; num; num--)
		results.push_back(_generate_random_string("som", 5));
	return		results;
}
*/

int	main(int c, char **v) {

	if (c != 4)	std::cout << "usage: " << v[0] << " wordslist url output_file" << std::endl;

	CURL	*curl = curl_easy_init();
	std::fstream _file(v[1], std::ios::in);
	std::fstream o_file(v[3], std::ios::out);
	if (!_file.is_open() || !o_file.is_open())	return 1;
	std::string	url(v[2]);
	std::string	buffer, response, s_url;

	while (std::getline(_file, buffer)) {
		buffer = buffer.substr(0, buffer.size() - 1);
		s_url = url + "/" + buffer;
		response = get_response(curl, s_url);
		if (response != "failure" && response.find("Not Found") == std::string::npos) {
			//std::cout << RED << s_url << NRM" : " << response.substr( response.find("<title>"), response.find("</title>") + 8 ) << std::endl;
			std::cout << RED << s_url << NRM" : " << "FOUND" << std::endl;
			o_file << RED << s_url << NRM" : " << response << std::endl;
		}
		else	std::cout << s_url << " : " << RED"NOT_FOUND"NRM << std::endl;
		buffer.clear();
		response.clear();
	}
	curl_easy_cleanup(curl);
	_file.close();
	o_file.close();

}
/*int	main() {
	int	sock = socket(AF_INET, SOCK_STREAM, 0);
	struct		sockaddr_in	address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("104.18.32.248");
	address.sin_port = htons(80);
	socklen_t si = sizeof(address);

	connect(sock, (struct sockaddr*)&address, si);

	int		i(0);
	std::string		message("GET / HTTP/1.1\r\nHost: 1337.ma\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:124.0) Gecko/20100101 Firefox/124.0\r\n\r\n");
	while (true) {
		i = send(sock, message.c_str(), message.size(), 0);
		if (i <= 0)	break;
		message = message.substr(i);
	}
	char			buffer[1000];
	message.clear();

	while (true) {
		std::memset(buffer, 0, sizeof(buffer));
		i = recv(sock, buffer, 1000, 0);
		if (i <= 0)	break ;
		message.append(buffer, i);
	}
	std::cout << message << std::endl;
	shutdown(sock, SHUT_RDWR);
	close(sock);
}*/
