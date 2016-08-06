#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void usage(const char*proc){
	printf("%s [ip] [port]\n",proc);
}
int main(int argc,char **argv){
//	if(argc != 3){
//		usage(argv[0]);
//		return 3;
//	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("socket");
		return 1;
	}
	struct sockaddr_in remote; 
	int reuse = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)	{	
		perror("setsockopet error\n");				
		return -1;				  
   	}
	
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_port = htons(8080);
	//inet_pton(AF_INET,argv[1],&remote.sin_addr);
	int len = sizeof(remote);
	if(connect(sock,(struct sockaddr*)&remote,sizeof(remote)) < 0){
		perror("connect");
		return 1;
	}
	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	printf("connect success\n");//,now please enter data:\n");
	fflush(stdout);
	while(1){
		printf("please enter data#");//\n
		fflush(stdout);
		int _s = read(0,buf,sizeof(buf)-1);
		if(_s > 0){
			buf[_s-1] = '\0';
		}
		//printf("data is :%s!write to server\n",buf);
		int ret = write(sock,buf,strlen(buf));
		ssize_t sz = read(sock,buf,sizeof(buf)-1);
		if(sz < 0){
			perror("read");
			return 2;
		}
		else if(sz == 0){
			printf("read done\n");
			break;
		}
		else 
		{
			fflush(stdout);
			printf("sever echo:%s \n",buf);
		}
		
	}
	return 0;
}





