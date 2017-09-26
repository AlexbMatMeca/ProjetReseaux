#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


struct sockaddr_in serv_addr;


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char** argv)

{

	if (argc != 2)

	{
		fprintf(stderr, "usage: RE216_SERVER port\n");
		return 1;
	}

	int sock = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	init_serv_addr(4000, serv_addr);

	struct in_addr addr = serv_addr.sin_addr;

	do_bind(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr));

	//printf("%i\n",(serv_addr.sin_addr.s_addr));

	do_listen(sock, 20, serv_addr.sin_addr,sizeof(addr));


	return 0;
}




//create the socket, check for validity!
//do_socket()

int do_socket(int domain, int type, int protocol){
	int sockfd;
	int yes = 1;

	//create the socket

	sockfd = socket(domain, type, protocol);

	// check for socket validity

	if (sockfd == -1){
		perror("Socket");
		exit(EXIT_FAILURE);}

	// set socket option, to prevent "already in use" issue when rebooting the server right on

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		error("ERROR setting socket options");

	return sockfd;}


//init the serv_add structure
//init_serv_addr()


void init_serv_addr(const char* port, struct sockaddr_in serv_addr){


	int portno;

	// clean the serv_add structure

	memset(&serv_addr, 0, sizeof (serv_addr));

	// cast the port from a string to an int

	int portno2 = atoi(port);
	portno = 4000;

	// internet family protocol

	serv_addr.sin_family = AF_INET;

	// we bind to any ip form the host

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// we bind on the tcp port specified
	serv_addr.sin_port = htons(portno);

}



//perform the binding
//we bind on the tcp port specified
//do_bind()

int do_bind(int sock, const struct sockaddr *adr, int adrlen){

	int b = bind(sock, adr , adrlen);

	if (b == -1){
		perror("bind");
		exit (EXIT_FAILURE);
	}
	return b;
}

//specify the socket to be a server socket and listen for at most 20 concurrent client
//listen()

int do_listen(int sock, int backlog, const struct sockaddr *adr, int adrlen){

	int l = listen(sock, backlog);

	if (l == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int k;
	for (k = 1;k<=backlog;k++){

		//accept connection from client
		//do_accept()

		do_accept(sock, adr, adrlen);

		//read what the client has to say
		//do_read()

		//char ptr = NULL;
		//ptr = 'a';
		//do_read(sock, ptr, 1);
		//we write back to the client
		//do_write()
		//do_write(sock, ptr,1);


	}

	//clean up server socket
	return sock;
}


int do_accept(int socket, struct sockaddr* addr, socklen_t* addrlen){
	int a = accept(socket, addr, addrlen);

	if (a == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return a;

}

int do_read(int sockfd, char* buf, int len){

	int r = 10;
	//&r = read(sockfd, buf, len);

	if (r == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	return r;
}

int do_write(int fd, const void *buf, size_t count){

	write(fd, buf, count);

	if (write(fd, buf, count) == -1){
		perror("write");
		exit(EXIT_FAILURE);
	}
	return 1;
}

