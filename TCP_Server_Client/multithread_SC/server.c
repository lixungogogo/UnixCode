#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

void* handler_data(void* arg){
	int sock = (int)arg;
	printf("create new thread...new socket:%d\n",sock);
	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	while(1){		
		ssize_t _s = 0;
		printf("read from client...\n");
		_s = read(sock,buf,sizeof(buf));
		if(_s < 0){
			perror("Read");
			break;
		}
		else if(_s == 0){
			printf("client is done\n");
			break;
		}
		else{
			buf[_s]	= '\0';
			printf("read done!client send : %s\n",buf);
			printf("write %s to client\n",buf);
			write(sock,buf,strlen(buf));		
		}
	}
	close(sock);
	pthread_exit((void*)0);
}

int main(){
	int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	int sock;
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
	printf("listen success\n");
	int len = sizeof(peer);
	
		
	while(1){
		if((sock = accept(listen_sock,(struct sockaddr*)&peer,&len)) < 0){
			perror("accept");
			return 4;
		}
		printf("connect success\n");	
		printf("father :%d\n",getpid());
		pthread_t ntid ;
		int ret;
	   	ret = pthread_create(&ntid,NULL,handler_data,(void*)sock);
		if(ret != 0){
			perror("pthread_Create");
			return 5;
		}
		pthread_detach(ntid);
	}
	return 0;
}








