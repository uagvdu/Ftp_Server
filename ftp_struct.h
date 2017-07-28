#ifndef FTP_STRUCT_H
#define FTP_STRUCT_H


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"ftp_struct.h"

#define _SIZE_ 1024
//vsftp: 每次连接成功客户端都会接收到来自服务器的问候消息

static char* welcome_message = "200 welcome using ftp\n";


typedef struct Command
{
	char command[5];
	char arg[_SIZE_];
}Command;


 enum cmdlist 
{ 
    ABOR, CWD, DELE, LIST, MDTM, MKD, NLST, PASS, PASV,
    PORT, PWD, QUIT, RETR, RMD, RNFR, RNTO, SITE, SIZE,
    STOR, TYPE, USER, NOOP
} ;


/*command list : 该服务器所支持的命令*/
static const char* cmdlist_str[]=
{
	
    "ABOR", "CWD", "DELE", "LIST","MDTM", "MKD", "NLST", "PASS", "PASV",
    "PORT", "PWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
    "STOR", "TYPE", "USER", "NOOP"
};

/*
 * FTP服务器有同常有两个端口：20 21
 * */
typedef struct Port
{
	int p1;
	int p2;

}Port; 


typedef struct State
{
    /* Connection mode: NORMAL, SERVER, CLIENT */
    int mode;

    /* Is user loggd in? */
	/*尽管是允许匿名登录，但是也会有实体用户登录*/
    int logged_in;//in代表是否有用户登录

    /* Is this username allowed? */
    int username_ok; //OK代表是否在允许链表里的用户
    char *username;

    /* Response message to client e.g. 220 Welcome */
    char *message;

    /* Commander connection */
    int connection;

    /* Socket for passive connection (must be accepted later) */
    int sock_pasv;

    /* Transfer process id */
    int tr_pid;

} State;

/*vsftp : 中有一个禁止连接的用户文件，*/
static const char* usernames[] = 
{
	"root","ftp","anonymous","public","anno"
	
};


#endif



