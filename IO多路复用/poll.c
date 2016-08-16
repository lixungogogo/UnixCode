int startup(char* ip,int port)
{
	
}

int main(int argc,char* argv[])
{
	struct pollfd pfd[1];
	int len = 1;
	
	pfd[0].fd = 0;
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	
	int done = 0;
	while(!done)
	{
		switch(poll(pfd,1,-1))
		{
			case 0:
				printf("timeout\n");
				break;
			case -1:
				perror("poll");
				break;
			default:
			{
				char buf[1024];
				if(pfd[0].revents & POLLIN)
				{
					ssize_t _s = read(pfd[0].fd,buf\
					sizeof(buf)-1);
					if(_s > 0)
					{
						buf[_s] = '\0';
						printf("echo:%s\n",buf);
					}
				}
			}
			break;
		}
	}
}