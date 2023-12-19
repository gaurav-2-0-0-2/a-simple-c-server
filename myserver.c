#include<arpa/inet.h>
#include<errno.h>
#include<stdio.h>
#include<sys/socket.h>

#define PORT 8080

int main(){
	// Create a socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // return a file descriptor: 'sockfd'
	if (sockfd == -1){
		perror("webserver (socket)");
		return 1;
	}
	printf("socket created successfully\n");

	// Create the address to bind the socket to
	struct sockaddr_in host_addr;
	int host_addrlen = sizeof(host_addr);

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind the socket to the address
	int sock_bind = bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen);

	if ( sock_bind != 0 ){
		
		perror("webserver (bind)");
		return 1;
	}	
	printf("scoket successfully bound to address\n");


	return 0;
}
