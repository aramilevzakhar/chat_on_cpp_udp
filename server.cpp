#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <future>

#define PORT 8080
#define MAXLINE 1024

struct sockaddr_in address, cli;
char buffer[1024];
int sock;

namespace sdf {
	void lissen(const int& server_fd) {
		using namespace std;
		int len = sizeof(cli);

		while (true) {
			recvfrom(server_fd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cli, (socklen_t*)&len);
			cout << buffer << endl;
			memset(buffer, 0, strlen(buffer));
		}

	}
}

int main() {
	using namespace std;
	//sleep(10);
  int server_fd, valread;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

     
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
     
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }



  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
     
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }

	int len = sizeof(cli);
	//recvfrom(sock, buffer, 1024, MSG_WAITALL, (struct sockaddr*)&cli, len);
	//recvfrom(server_fd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cli, (socklen_t*)&len);
	thread listening_thread;
	//cout << buffer << endl;
	//auto p = (struct sockaddr*)&server_fd;

	listening_thread = thread(sdf::lissen, server_fd);

	

	string message;
	char chose[MAXLINE];
	while (true) {

		// cin >> chose;
		// cout << chose;
		// strcpy(buffer, "hello\n");

		getline(cin, message);

		sendto(server_fd, message.c_str(), message.length(), MSG_CONFIRM, (struct sockaddr*)&cli, len);
		message = "";
	}


	listening_thread.join();

	return 0;
}
