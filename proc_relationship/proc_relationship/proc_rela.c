#include <stdio.h>
#include <unistd.h>
int main(){
	pid_t id = fork();

	if(id < 0){//err
		printf("fork erro\n");
		return -1;
	}
	else if(id == 0){//child
		printf("child id is :%d\n",getpid());
		while(1){
			printf("child\n");	
			sleep(2);
		}
	}
	else{//father
		printf("father id is :%d\n",getpid());
	//	while(1){
			printf("father\n");
			sleep(5);
	//	}
	}
	return 0;
}


