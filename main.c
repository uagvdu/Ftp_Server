#include"ftp.h"
#include"ftp_struct.h"


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("%s [server_ip][server_port]",argv[0]);
		}
	
	if(chroot("/home/admin")<0)
	{
		perror("root dir set failed!\n");
		exit(1);
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));

	while(1)
	{
		struct  sockaddr_in client;
		socklen_t len = sizeof(client);

		int sockfd = accept(listen_sock,(struct sockaddr*)&client,&len ); 
		if(sockfd < 0)
		{
			// 打印日志
			perror("accept falied \n");
		}
		
		pthread_t ptid;

		if(pthread_create(&ptid,NULL,Deal_Thread,(void*)&sockfd)<0)
		{
			perror("pthread failed\n");
		}
		
		/*pthread_join() 获取Deal_Thread函数的返回值
		 *调用该函数的线程将被挂起等待，直至所等待的线程终止：此处前后两个线程不同
		 *pthread_detach() :使线程分离，当线程结束自动回收资源：调用该函数的线程将不能被pthread_join
		 * 对一个尚未分离的线程调用pthread_join/thread_detach，那么该线程就处于detach状态：所以同一个线程不能被连续两次join
		 *
		 *因为会挂起等待，所以选择pthread_detach
		
		*/
	}
	return 0;

	}
