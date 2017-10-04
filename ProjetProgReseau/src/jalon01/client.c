#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


//get the socket

int do_socket(int domain, int type, int protocol) {

	int sock;
	int yes = 1;


	//create the socket

	sock = socket(domain, type, protocol);

	//check for socket validity

	if (sock == -1){
			perror("Socket");
			exit(EXIT_FAILURE);}

	// set socket option, to prevent "already in use" issue when rebooting the server right on

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			error("ERROR setting socket options");

	return sock;
}




//connect to remote socket
//do_connect()

void do_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {

	printf("Trying connection...\n \n");

	// Connection

	int res = connect(sockfd, addr, addrlen);


	if (res != 0) {
		perror("error connection");
		exit(EXIT_FAILURE);
	}

	printf("Connection succeded !\n \n");

}



//get user input
//readline()
char* readline(char* message){

	printf("tapez une phrase : \n");
	fgets(message, 1000*sizeof(char), buf);
	return message;
}



ssize_t sendline(int fd, void *str, size_t maxlen){

	int sent = 0;
	do{
		sent += write (fd, str + sent, maxlen - sent);

	}
	while (sent != maxlen);
	return sent;
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



int main(int argc,char** argv)
{
	struct sockaddr_in sock_host;
	char message[1000];
	char* received = malloc(1000*sizeof(char));
	char* quit;
	quit = "/quit\n";
	int s;

    if (argc != 3)
    {
        fprintf(stderr,"usage: RE217_CLIENT hostname port\n");
        return 1;
    }

    s = do_socket(AF_INET, SOCK_STREAM, 0);

    memset(& sock_host, '\0', sizeof(sock_host));
    sock_host.sin_family = AF_INET;
    sock_host.sin_port = htons(atoi(argv[2]));
    sock_host.sin_addr.s_addr = inet_aton(argv[1],& sock_host.sin_addr );


    do_connect(s, (const struct sockaddr*)&sock_host, sizeof(struct sockaddr));

    for(;;){

    	readline(message);
    	if (strcmp(quit,message) == 0){
    		printf("End of connection !\n\n");
    	  	sendline(s, message, 1000*sizeof(char));
    	    do_read(s, received, 1000*sizeof(char));
    	    close(s);
    	    printf("Le message est : %s\n", received);
    	//clean up client socket
    		break;

    	}

    	sendline(s, message, 1000*sizeof(char));
    	do_read(s, received, 1000*sizeof(char));
    	printf("Le message est : %s\n", received);}

    printf("Connection terminée\n");
    return (EXIT_SUCCESS);

}
