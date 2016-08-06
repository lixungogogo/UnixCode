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
void colect_child(int sig){
	printf("pid %d\n",getpid());
	while(waitpid(-1,0,WNOHANG) > 0){
		printf("colect child done...\n");
	}
}
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
	printf("listen success\n");
	int len = sizeof(peer);
	
	signal(SIGCHLD, colect_child);
		
	while(1){
		if((new_sock = accept(listen_sock,(struct sockaddr*)&peer,&len)) < 0){
			perror("accept");
			return 4;
		}
		printf("connect success\n");	
		printf("father :%d\n",getpid());
		pid_t id = fork();
		if(id < 0){
			perror("fork");
			return 5;
		}
		else if(id == 0){//子进程
			printf("fork done pid :%d, ... get new client IP :%s,port:%d\n",\
				getpid(),inet_ntoa(peer.sin_addr),peer.sin_port);

			char buf[1024];
			memset(buf,'\0',sizeof(buf));
			while(1){		
				ssize_t _s = 0;
			//	printf("read from client...\n");
				_s = read(new_sock,buf,sizeof(buf));
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
			//	printf("write %s to client\n",buf);
				write(new_sock,buf,strlen(buf));		
				}
			}
			close(new_sock);
			//exit(0);
		}
		else 
			close(new_sock);
	}
	return 0;
}








