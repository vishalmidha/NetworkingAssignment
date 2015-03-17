#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>

char buffer[512];
int n;
/*
void revstr(char str[]){
	int len = strlen(str) -1;
	int i=0;
	char temp[len];
	for(i=0;i<len;i++){
		temp[len-i] = str[i];
	}
	str = temp;
	return 0;
}*/

void* messaging(void* newsid){
	
	//send and recieve data using read() and write() system calls
	bzero(buffer,512);
	n = read(newsid,buffer,511);
	if (n<0)
	{
		fprintf(stderr, "Error rading from socket");
	}
	int len = strlen(buffer) -1;
	int j=0;
	char temp[len];
	for(j=0;j<len;j++){
		temp[len-j] = buffer[j];
	}
	//buffer = temp;
	printf("Here is the message: %s \n", buffer);
	//count
	int count_s=0,count_w=1,count_l=0,i=0;
	for(i=0;buffer[i]!= '\0';i++){
		
		if(buffer[i] == ' ')
			count_s++;
		if(buffer[i] == ' ' && buffer[i+1] != ' ' && buffer[i+1] !='\t' && buffer[i+1] != '\n')
			count_w++;
		if ((buffer[i]>='a'&&buffer[i]<='z')||(buffer[i]>='A'&&buffer[i]<='Z'))
			count_l++;
		}

	char string[100]; 
	sprintf(string, "Inverted Message:\n%s\nNo of words: %d \nNo of spaces: %d \nNo of letters: %d \n",temp, count_w,count_s,count_l);
	n = write(newsid,string, 100);
	//n = write(newsid, "got your message", 16);
	if(n<0){
		fprintf(stderr, "Error in writing socket");
	}
	pthread_exit(NULL);
}

int main()
{
	int portno, clilen;
	
	struct sockaddr_in serv_add, cli_add;
	
	pthread_t threads[10];
	
	//create socket using socket()
	socklen_t sid, newsid;
	sid = socket(AF_INET, SOCK_STREAM, 0);
	if(sid<0){
		fprintf(stderr, "Error: socket not created");
		exit(1);
	}
	bzero((char*) &serv_add, sizeof(serv_add));
	portno = 2000;
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY;
	serv_add.sin_port = htons(portno);
	//bind the socket to an adress using bind() 
	if(bind(sid, (struct sockaddr *) &serv_add, sizeof(serv_add))<0){
		fprintf(stderr, "Error: couldn't bind\n");
		exit(1);
	}
	//listen using listen()
	listen(sid,10);

	while(1){
		printf("IP adress is: %s\nWaiting for client to connect...\n", inet_ntoa(serv_add.sin_addr));
		int i=0;
		//accept a connection with accept()
		clilen = sizeof(cli_add);
		newsid = accept(sid,(struct sockaddr *) &cli_add, &clilen);
		if (newsid<0)
		{
			fprintf(stderr, "Error in accepting client");
		}
		else{
			pthread_create(&threads[i], NULL, messaging, newsid);
			
			if(i<10){
				i++;
			}
		}

	}
	return 0;
}