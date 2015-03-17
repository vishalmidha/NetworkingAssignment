#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int portno, n;
	char buffer[512];
	socklen_t sid;
	struct sockaddr_in serv_add, cli_add;
	struct hostent *server;
	
	if (argc<3)
	{
		fprintf(stderr, "Error:IP address or port number not provided \n");
		exit(1);
	}
	portno = atoi(argv[2]);

// create socket using socket()
	sid = socket(AF_INET, SOCK_STREAM, 0);
	if(sid<0){
		fprintf(stderr, "Error in creating socket\n");
		exit(1);
	}
	server = gethostbyname(argv[1]);
	if (server==NULL)
	{
		fprintf(stderr, "Error: no such host\n");
		exit(1);
	}
	bzero((char *) &serv_add, sizeof(serv_add));
	serv_add.sin_family = AF_INET;

	bcopy((char *) server->h_addr, (char *)&serv_add.sin_addr.s_addr, server->h_length);

	serv_add.sin_port = htons(portno);
	
// connect a socket with connect()
	if(connect(sid, &serv_add, sizeof(serv_add))<0){
		fprintf(stderr, "Error connecting\n");
		exit(1);
	}
	getsockname(sid, (struct sockaddr *) &cli_add, sizeof(struct  sockaddr_in));
	printf("connecting via port number: %d\n",cli_add.sin_port);

	printf("please enter the message:\n");
	bzero(buffer,512);

// send and recieve data using read() and write() system calls
	fgets(buffer,511,stdin);
	n = write(sid,buffer,strlen(buffer));
	if(n<0){
		fprintf(stderr, "Error writing to socket\n");
	}http://stackoverflow.com/questions/5759031/getsockname-always-returning-0-0-0-0
	bzero(buffer,512);
	n=read(sid,buffer,511);
	if(n<0){
		fprintf(stderr, "Error reading from socket \n");
	}
	printf("%s\n", buffer);

	return 0;
}