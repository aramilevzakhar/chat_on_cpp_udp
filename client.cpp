// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <thread>

#define PORT 8080
#define MAXLINE 1024



char buffer[MAXLINE];

void listening(const int& sock, const struct sockaddr_in& cli) {
	using namespace std;
	int len = sizeof(cli);

	while (true) {
		recvfrom(sock, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cli, (socklen_t*)&len);
		cout << buffer << endl;
		memset(buffer, 0, MAXLINE);
	}

}

void sending(const int& sock, const struct sockaddr_in& serv_addr) {
	using namespace std;
	string message;
	while (true) {
		getline(cin, message); 
		sendto(sock, message.c_str(), message.length(), MSG_CONFIRM, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		message = "";
	}
}



int main(int argc, char const *argv[]) {
	using namespace std;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8080);
	
	string ip_str;
	cout << "Type your ip address(127.0.0.1): " << endl;
	getline(cin, ip_str);
	if (ip_str == "") { ip_str = "127.0.0.1"; }
	cout << ip_str << endl;
	while (true) {}


	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ip_str.c_str(), &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	/*
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	*/
	int len = sizeof(serv_addr);



	cout << "sturt client" << endl;

	thread listening_thread;
	thread sending_thread;
	listening_thread = thread(listening, sock, serv_addr);
	sending_thread = thread(sending, sock, serv_addr);

	listening_thread.join();
	sending_thread.join();




	return 0;
}

