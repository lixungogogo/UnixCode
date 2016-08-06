#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(){
	int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	int new_sock;
	struct sockaddr_in peer;
	if(listen_sock < 0){
		perror("socket");
		return 1;
	}

	int reuse = 1;
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)	{	
		perror("setsockopet error\n");				
		return -1;				  
   	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr("127.0.0.1");
	local.sin_port = htons(8080);

	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("bind");
		return 2;
	}
	printf("bind success\n");
	if(listen(listen_sock,5) < 0){
		perror("listen");
		return 3;
	}
//	bzero(&peer,sizeof(peer));
	printf("listen success\n");
	int len = sizeof(peer);
		if((new_sock = accept(listen_sock,(struct sockaddr*)&peer,&len)) < 0){
			perror("accept");
			return 4;
		}
	
	printf("client IP :%s,port:%d\n",\
				inet_ntoa(peer.sin_addr),peer.sin_port);
	while(1){


		char buf[1024];
		memset(buf,'\0',sizeof(buf));
		int _s = 0;
//		printf("read from client...");
		_s = read(new_sock,buf,sizeof(buf));
		if(_s < 0){
			perror("Read");
			break;
		}
		else if(_s == 0){
			printf("client is done\n");
			break;
		}
		buf[_s]	= '\0';
		printf("read done!client send : %s\n",buf);
		//printf("write %s to client\n",buf);
		write(new_sock,buf,strlen(buf));		
	}
	return 0;
}








