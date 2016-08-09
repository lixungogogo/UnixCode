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
	int listen_sock = 1;
	if((listen_sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}
	printf("listensock:%d\n",listen_sock);
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(argv[1]);
	local.sin_port = htons(atoi(argv[2]));
	printf("%s\n",argv[1]);
	printf("%s\n",argv[2]);
	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("bind");
		exit(2);
	}
	
	if(listen(listen_sock,5) < 0){
		perror("listen");
		exit(5);
	}
	struct sockaddr_in peer;
	int sock;
	socklen_t len = sizeof(peer);
	
	if((sock = accept(listen_sock,(struct sockaddr*)&peer,&len)) < 0){
		perror("accept");
		exit(3);
	}

	int maxfd = sock + 1;
	while(1){
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(sock,&rfds);
		if(select(maxfd,&rfds,NULL,NULL,NULL) < 0){
			perror("Select");
			exit(4);
		}
		else if(FD_ISSET(sock,&rfds) != 0){
			char buf[1024];
			memset(buf,'\0',sizeof(buf));
			int _s = read(sock,buf,sizeof(buf));
			if(_s > 0){
				buf[_s] = '\0';
				printf("client:%s\n",buf);
			}
			else if(_s == 0){
				printf("client done\n");
				break;
			}
			else {
				perror("read");
				exit(5);
			}
		}
	}
}

