#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
void usage(const char* proc){
	printf("%s [ip] [port]\n",proc);
}

int main(int argc,char* argv[]){
	if(argc != 3){
		usage(argv[0]);
		exit(1);
	}
	int sock;
	if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}
	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr(argv[1]);
	remote.sin_port = htons(atoi(argv[2]));

	if((connect(sock,(struct sockaddr*)&remote,sizeof(remote)))< 0){
		perror("connect");
		exit(6);
	}
	printf("connect success:sock %d\n",sock);
	int maxfd = sock + 1;
	while(1){
		fd_set wfds;//,wfds;
		FD_ZERO(&wfds);
	//	FD_ZERO(&wfds);
	//	FD_SET(sock,&wfds);
		FD_SET(0,&wfds);
		FD_SET(sock,&wfds);
		if(select(maxfd,NULL,&wfds,NULL,NULL) < 0){
			perror("Select");
			exit(4);
		}
	//	printf("select success\n");
		if(FD_ISSET(0,&wfds) != 0){
			char buf[1024];
			memset(buf,'\0',sizeof(buf));
			printf("client#");
			fflush(stdout);
			int _s = read(0,buf,sizeof(buf));
			if(_s > 0){
				buf[_s - 1] = '\0';
			//	printf("client:%s\n",buf);
			}
			else if(_s == 0){
				printf("client done\n");
				break;
			}
			else {
				perror("read");
				exit(5);
			}
			if(FD_ISSET(sock,&wfds) != 0){		
			//	printf("write:%s\n",buf);
				write(sock,buf,strlen(buf));
			}
		}
		
	}
}

