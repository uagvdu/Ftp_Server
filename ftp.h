#ifndef __FTP_H_
#define  __FTP_H_

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
int startup(char* ip,int port);
void* Deal_Thread(void *fd);

void parse_cmd(char* buf,Command *cmd);
void Response(Command *cmd,State* state);
int Find(Command* cmd);
int _Find(char* cmd,const char** array,int count);

void get_port(Port* port);
void get_ip(int sock,int *ip);

void SendClient(State* state);

void ftp_abor(State* state);
void ftp_cwd(Command* cmd,State* state);
void ftp_dele(Command* cmd,State* state);
void ftp_list(Command* cmd,State* state);
void ftp_mdtm(Command* cmd,State* state);
void ftp_mkd(Command* cmd,State* state);
void ftp_mlst(Command* cmd,State* state);
void ftp_pass(Command* cmd,State* state);
void ftp_pasv(Command* cmd,State* state);
void ftp_port(Command* cmd,State* state);
void ftp_pwd(Command* cmd,State* state);
void ftp_quit(Command* cmd,State* state);
void ftp_retr(Command* cmd,State* state);
void ftp_rmd(Command* cmd,State* state);
void ftp_rnfr(Command* cmd,State* state);
void ftp_rnto(Command* cmd,State* state);
void ftp_site(Command* cmd,State* state);
void ftp_size(Command* cmd,State* state);
void ftp_stor(Command* cmd,State* state);
void ftp_type(Command* cmd,State* state);
void ftp_user(Command* cmd,State* state);
void ftp_noop(State* state);



#endif

/*
 * 第一个进行服务器进行监听的fd：用来接收客户端进行连接的fd
 * 第二个是服务器与客户端进行交互的fd
 *
 *
 * */
