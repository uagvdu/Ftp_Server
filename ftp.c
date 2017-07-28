#include"ftp.h"
#include"ftp_struct.h"


int startup(char* ip,int port)
{
	//建立 网络通讯端口
	int serverfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverfd < 0)
	{
		perror("serverfd failed\n");
		exit(1);
	}

	int opt = 1;
	int stat = setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);
	server.sin_family = AF_INET;
	socklen_t len = sizeof(server);

	//将端口和ip和port绑定在一起
	if(bind(serverfd,(struct sockaddr*)&server,len)<0)
	{
		perror("bind failed \n");
		exit(2);
	}

	//backlog 参数：
	// 1. 提示内核监听队列的最大长度
	// 监听队列的长度如果超过backlog，则服务器就不再受理新的客户连接
	// 
	// 监听队列长度：已经 完成三次握手的tcp连接
	//
	//
	// 分两个版本： 
	// 2.2版 之前的代表半连接状态和完全连接状态的socket的上限
	//       之后的代表 完全连接状态的socket的上限
	//	
	// /****
	// 	所能允许的已完成三次握手的连接的最大上限,不过监听队列中完整连接		   的上限一般比backlog略大一点***/
	//
	if(listen(serverfd,5)<0)
	{
		perror("failed \n");
		exit(3);
	}
	return serverfd;
	
}

void* Deal_Thread(void* fd)
{
	chdir("/");
	pthread_detach(pthread_self());

	int sockfd =*((int*)fd);

	/*发送连接成功消息 ： */
	if(send(sockfd,welcome_message,strlen(welcome_message),0)< 0 )
	{
		perror("send client data failed!\n");
		return (void*)5;
	}

	/*识别用户: 登录*/
	State *state = (State*)malloc(sizeof(State));
	state->username = NULL;
	Command * cmd = (Command*)malloc(sizeof(Command));




	char buf [_SIZE_];
	while(1)
	{
		memset(cmd,0,sizeof(Command));
		// state 暂时不能清零，因为这是一个用户在循环
		memset(buf,0,_SIZE_);	
		int sz =  recv(sockfd,buf,_SIZE_-1,0);
		
		if(sz < 0)
		{
			//打印日志
			perror("read failed！\n");
			continue;
		}
		if(sz == 0)
		{
			close(sockfd);
			printf("client quie！\n");
			return (void *)1;
		}
		buf[sz] = 0;
		printf("client [%s]: %s\n",state->username == NULL?"unknow":state->username,buf);
		parse_cmd(buf,cmd);	  // 获取buff中的命令和参数，给cmd

		state->connection= sockfd;
		if(buf[0]<=127&&buf[0]>0)
		{
			Response(cmd,state);
		}
		else
		{
			perror("buffer data exit error\n ");
			return (void*)6;
		}
		

	}

}

void Response(Command* cmd,State* state)
{
	if(Find(cmd)<0)
	{
		printf("Find failed!\n");	
		return ;
	}	
	else{
		printf("Switch %d \n",Find(cmd));
		switch(Find(cmd))
		{
			case ABOR:ftp_abor(state);
						break;
			case CWD:ftp_cwd(cmd,state);
						break;
			case DELE:ftp_dele(cmd,state);
						break;
			case LIST:ftp_list(cmd,state); 
						break;
			case MDTM:ftp_mdtm(cmd,state);
						break;
			case MKD:ftp_mkd(cmd,state);
						break;
			case NLST:ftp_mlst(cmd,state);
						break;
			case PASS:ftp_pass(cmd,state);
						break;
			case PASV:ftp_pasv(cmd,state);
						break;
			case PORT:ftp_port(cmd,state);
						break;
			case PWD:ftp_pwd(cmd,state);
						break;
			case QUIT:ftp_quit(cmd,state);
						break;
			case RETR:ftp_retr(cmd,state);
						break;
			case RMD:ftp_rmd(cmd,state);
						break;
			case RNFR:ftp_rnfr(cmd,state);
						break;
			case RNTO:ftp_rnto(cmd,state);
						break;
			case SITE:ftp_site(cmd,state);
						break;
			case SIZE:ftp_size(cmd,state);
						break;
			case STOR:ftp_stor(cmd,state);
						break;
			case TYPE:ftp_type(cmd,state);
						break;
			case USER:ftp_user(cmd,state);
						break;
			case NOOP:ftp_noop(state);
						break;
			default:
			 	state->message = "Unknow cmd!";
				send(state->connection,state->message,strlen(state->message),0);
				break;
				
				

		}
	}
}


void parse_cmd(char* buf,Command *cmd)
{
	sscanf(buf,"%s %s",cmd->command,cmd->arg);
}


void ftp_noop(State* state)
{
	if(state->logged_in)
	{
		state->message ="200 Ftp server is OK!";
		send(state->connection,state->message,strlen(state->message),0);
	}
	else{
		state->message = "530 NOOP here!";
		 send(state->connection,state->message,strlen(state->message),0);
				}
}


void ftp_user(Command* cmd,State* state)
{
	int index;
	int sz = sizeof(usernames)/sizeof(usernames[0]);
	if(index=_Find(cmd->arg,usernames,sz)>0)
	{
		//用户名这里可以使用字符串拷贝的方式cmd->arg
		state->username=usernames[index];
		state->username_ok =1;
		state->message = "331 Please specify the passwd!\n Password:\n ";
		
	}else
	{
		state->logged_in = 0;
		state->username_ok=0;
		state->message = "530 Login incorrect!\n";

	}
	SendClient(state);

}

void ftp_abor(State* state)
{
	
}

void ftp_cwd(Command* cmd,State* state)
{

}
void ftp_dele(Command* cmd,State* state)
{
}
void ftp_list(Command* cmd,State* state)
{
}
void ftp_mdtm(Command* cmd,State* state)
{
}
void ftp_mkd(Command* cmd,State* state)
{
}
void ftp_mlst(Command* cmd,State* state)
{
}
void ftp_pass(Command* cmd,State* state)
{	
	if(state->username_ok>0)
	{
		state->logged_in =1;
		state->message="230 login successful\n";
		
	}
	else
	{
		state->message="500 Invalid username or password!\n";
	}
	SendClient(state);
	
}
void ftp_pasv(Command* cmd,State* state)
{
	if(state->logged_in)	
	{
		int ip[4];
		char buf[255];
		char *response="227 Entering PASV Mode(%d,%d,%d,%d,%d,%d)\n";
		Port *port = (Port*) malloc(sizeof(Port));
		get_port(port);
		state->sock_pasv = create_socket((255*port->p1)+port->p2);
		printf("port %d\n",256*port->p1+port->p2);
		sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);
		
		state->message = buff;
		state->mode=SERVER;
		puts(state->message);
		

	}
	else
	{
		state->message= "530 Please login with USER or PASS\n"
	}
		SendClient(state);
}
void ftp_port(Command* cmd,State* state)
{
}
void ftp_pwd(Command* cmd,State* state)
{
}
void ftp_quit(Command* cmd,State* state)
{
}
void ftp_retr(Command* cmd,State* state)
{
}
void ftp_rmd(Command* cmd,State* state)
{
}
void ftp_rnfr(Command* cmd,State* state)
{
}
void ftp_rnto(Command* cmd,State* state)
{
}
void ftp_site(Command* cmd,State* state)
{
}
void ftp_size(Command* cmd,State* state)
{
}
void ftp_stor(Command* cmd,State* state)
{
}
void ftp_type(Command* cmd,State* state)
{

}
void SendClient(State* state)
{
	send(state->connection,state->message,strlen(state->message),0);

}

int Find(Command* cmd)
{
	
	int sz = sizeof(cmdlist_str)/sizeof(char*);
	return _Find(cmd->command,cmdlist_str,sz);
}

int _Find(char* str,const char** array,int count)
{
	int i = 0;
	for(;i<count;i++)
	{
		if(strcmp(str,array[i])==0)
			return i;
	}
	return -1;
}


void get_port(Port* port)
{
	rand(time(NULL));
	port->p1=128+(rand()%64);
	port->p2 = rand()%0xff;
	printf("Port : %d, %d\n",p1,p2);
}


void get_ip(int sockfd,int *ip)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	
	getsockname(sockfd,(struct sockaddr*)&addr,&addrlen);
	int host ,i ;
	host = (addr.sin_addr.s_addr);
	for(i=0;i<4;i++)
	{
		ip[i]=(host>>i)&0xff;
	}

}
