#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int create_daemon(){
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct sigaction sa;
	umask(0);
	

	if((pid = fork()) < 0){
		printf("fork");
		return 1;
	}
	else if(pid != 0){//father
		exit(0);
	}

	setsid();//保证不是会话首进程

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGCHLD,&sa,NULL) < 0){
		return 2;
	}
	if((pid = fork()) < 0){
		return 1;
	}
	else if(pid != 0)
		exit(0);

	if(chdir("/") < 0)//更改目录为根目录
		return 3;
		close(0);
	fd0 = open("/dev/null",O_RDWR);
	dup2(fd0,1);
	dup2(fd0,2);
	return 0;
}

int main(){
	//printf("aaaa\n");
	int i = 0;
	i = create_daemon();
	if(i == 0)
		i = 2;
//		return 0;
//	printf("%d\n",i);
	while(1){
		sleep(1);
	}
	return 0;
}
