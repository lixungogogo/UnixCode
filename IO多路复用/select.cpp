int fds[128];
const int len = 128;


int startup(char *ip,int port)
{
	int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(ip);
	local.sin_port = port;
	bind(&listen_sock,(struct sockaddr*)&local,sizeof(local));
	if(listen(listen_sock,5) < 0)
	{
		perror("listen");
	}
	return listen_sock;
}

int main()
{
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}
	int i = 0;
	for(;i < len;i++)
	{
		fds[i] = -1;
	}
	
	int listen_sock = startup(argv[1],atoi(argv[2]));
	
	fd_set rfds;
	fds[0] = listen_sock;
	int done;
	while(!done)
	{
		int max_fd = -1;
		FD_ZERO(&rfds);
		for(int i = 0;i < len;i++)
		{
			if(fds[i] != -1)
			{
				FD_SET(fds[i],&rfds);
			}
			max_fd = fd[i] > max_fd ? fd[i]:max_fd;
		}
		struct timeval timeout = {5,0};
		switch(select(max_fd+1,&rfds,NULL,NULL,NULL))
		{
			case 0:
			printf("timeout\n");
			break;
			case -1:
			perror("select");
			break;
			default:
			{
				if(FD_ISSET(listen_sock,&rfds))
				{
					struct sockaddr_in peer;
					int len = sizeof(peer);
					int new_fd = accept(listen_sock,\
					(struct sockaddr*)&peer,&len);
					if(new_fd > 0)
					{
						printf("get a new client->%s:%d\n",\
						inet_addr(peer.sin_addr),\
						ntohs(peer.sin_port));
						for(int i = 0; i < len;i++)
						{
							if(fds[i] == -1)
							{
								fds[i] = new_fd;
								break;
							}
						}
						if(i == len)
						{
							close(new_fd);
						}
					}
				}
				else
				{
					char buf[1024];
					for(int i = 0; i < len;i++)
					{
						if(i != 0 && FD_ISSET(fds[i],\
										&rfds))
						{
							ssize_t _s = read(fds[i],buf,sizeof(buf))
							if(_s > 0)
							{
								buf[_s] = '\0';
								printf("client:%s\n",buf);
							}
							else if(_s == 0)
							{
								printf("client:%d is close\n",fds[i]);
								close(fds[i]);
								fds[i] = -1;
							}
							else 
								perror("read");
						}
					}
				}
			}
			break;			
		}
	}
}