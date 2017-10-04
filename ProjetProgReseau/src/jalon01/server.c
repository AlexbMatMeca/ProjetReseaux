#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#define MAX_MSG 256


//manage error

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


//create the socket, check for validity!
//do_socket()

int do_socket(int domain, int type, int protocol){

	//initialization of variable

	int sockfd;
	int yes = 1;

	//create the socket

	sockfd = socket(domain, type, protocol);

	//check for socket validity

	if (sockfd == -1){
		perror("Socket");
		exit(EXIT_FAILURE);}

	// set socket option, to prevent "already in use" issue when rebooting the server right on

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		error("ERROR setting socket options");

	return sockfd;}


//init the serv_add structure
//init_serv_addr()

void init_serv_addr(const char* port, struct sockaddr_in* serv_addr){

	int portno;

	// clean the serv_add structure

	memset(serv_addr, 0, sizeof (struct sockaddr_in));

	// cast the port from a string to an int

	portno = atoi(port);

	// internet family protocol

	serv_addr->sin_family = AF_INET;

	// we bind to any ip form the host

	serv_addr->sin_addr.s_addr = INADDR_ANY;

	// we bind on the tcp port specified

	serv_addr->sin_port = htons(portno);

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

void do_listen(int sock, int backlog){//, const struct sockaddr *adr, int adrlen){

	int l = listen(sock, backlog);

	if (l == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}
}



int do_accept(int socket, struct sockaddr* addr, socklen_t* addrlen){

	int a = accept(socket, addr, addrlen);

	if (a == -1){
		perror("accept");
		exit(EXIT_FAILURE);
	}

	return a;

}


int do_read(int sockfd, char* buf, int len){

	if (sockfd == -1){
		perror("read");
		exit(EXIT_FAILURE);
	}
	int sent = 0;
	do{
		sent += read (sockfd, buf + sent, len - sent);

	}
	while (sent != len);
	return sent;
}


int do_write(int fd, const void *buf, size_t count){
	int sent = 0;
	if (fd == -1){
		perror("write");
		exit(EXIT_FAILURE);
	}
	do{
		sent += write(fd, buf + sent, count - sent);

	}
	while (sent != count);

	return sent;
}



int main(int argc, char** argv)

{
	int i;
	fd_set fd_set_read;
	char* buf = malloc(1000*sizeof(char));
	if (argc != 2)

	{
		fprintf(stderr, "usage: RE216_SERVER port\n");
		return 1;
	}
	struct sockaddr_in serv_addr;
	int sock = do_socket(AF_INET, SOCK_STREAM, 0);

	init_serv_addr(argv[1], &serv_addr);

	do_bind(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr));

	do_listen(sock, 20);

	struct sockaddr client;
	socklen_t taille;
	UP:

	//accept connection from client
	//do_accept()
	printf("Waiting connection...\n\n");
	int newsock = do_accept(sock, &client,&taille);
	printf("Connected !\n\n");

	for (;;){

		char* quit;
		quit = "/quit\n";

		//read what the client has to say
		//do_read(newsock, buf, 1000*sizeof(char));
		//do_write(newsock, buf, 1000*sizeof(char));


		if (strcmp(quit,buf) == 0){
			printf("End of connection !\n\n");

			//clean up client socket
			close(newsock);
			goto UP;
		}
		
		
		//clean the set before adding file descriptors
		FD_ZERO(&fd_set_read);
		//add the fd for server connection
		FD_SET(sock, &fd_set_read);
		//add the fd for user-input
		FD_SET(fileno(stdin), &fd_set_read);
		//as per man, nfds is the highest-numbered file descriptor in any of the three sets, plus 1.
		int max_fd = sock + 1;
		//we know wait for any file descriptor to be available for reading
		int select_ret_val = select(max_fd, &fd_set_read, NULL, NULL,
				NULL);
		//now we loop over file descriptors until we have reached the highest one
		//open for reading OR we stop looping if we have already treated all file descriptors we should have.
		for (i = 0;
				i < max_fd && select_ret_val > 0;
				i++)
		{
			//don't forget to wipe buffer before using it
			memset(buf, 0, sizeof buf);
			//we check if the file descriptor is into the FD_ISSET
			if (FD_ISSET(i, &fd_set_read))
			{
				//yes it is! now, we have 2 cases, whether it's a user input or a server input.
				//that's a user input
				if (i == fileno(stdin))
				{
					//read from stdin
					do_read(i, buf, sizeof buf);
					//this is wrong, we should use readline
					//write to the server
					do_write(sock, buf, strlen(buf));
					//this is wrong, we should use writeline
				}
				else
				{
					//that's a server input
					//read from the server
					do_read(i, buffer, sizeof buf);
					//this is wrong, we should use readline
					//write to stdout
					do_write(fileno(stdout), buf, strlen(buf));
					//this is wrong, we should use writeline
				}
				//decrement the count of fd we should treat
				select_ret_val--;
			}
		}
		
	}

	//clean up server socket

	close (sock);
	return 0;
}
