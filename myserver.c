#include<arpa/inet.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h> // This directive (Unix Standard Library), so that we can use close(), read(), write(), etc

#define PORT 8080
#define BUFFER_SIZE 1204

int main(){

	
	char buffer[BUFFER_SIZE];
        char resp[] = "HTTP/1.0 200 OK\r\n"
                      "Server: webserver-c\r\n"
                      "Content-type: text/html\r\n\r\n"
                      "<html>hello, world</html>\r\n";


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

	//Create client address
	struct sockaddr_in client_addr;
        int client_addrlen = sizeof(client_addr);
	memset(&client_addr, 0, client_addrlen);
	
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(8080);

	// Bind the socket to the address
	int sock_bind = bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen);

	if ( sock_bind != 0 ){
		
		perror("webserver (bind)");
		return 1;
	}	
	printf("scoket successfully bound to address\n");

	// Listen for incoming connections
	int sock_listen = listen(sockfd, SOMAXCONN);
	if (sock_listen != 0){
		perror("webserver (listen)");
		return 1;
	}
	printf("server listening for connection");

	for(;;){  // we are using an infinite loop to continuously handle incoming requests 
	// Accepting incoming connections
        	int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
	
          	if(newsockfd < 0){
          		perror("webserver (accept)");
          		continue;
          	}
          	printf("connection accepted");
          
          
          	//Read from the socket
          	int valread = read(newsockfd, buffer, BUFFER_SIZE);
          	if (valread < 0){
          		perror("webserver (read)");
          		continue;
          	}

		//Read the request
		char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
		sscanf(buffer, "%s %s %s", method, uri, version);
		printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, uri, version);

          
          	// Write to the socket
          	int valwrite = write(newsockfd, resp, strlen(resp));
          	if(valwrite < 0){
          		perror("webserver (write)");
          		continue;
          	}
            	
                close(newsockfd);
	
	} 
	return 0;
}
