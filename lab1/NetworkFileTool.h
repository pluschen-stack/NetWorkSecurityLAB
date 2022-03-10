#ifndef __NETWORKFILETOOL_H
#define __NETWORKFILETOOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define DEFAULT_FILENAME_SIZE 50
#define DEFAULT_BUFF_SIZE 1024


typedef struct NetworkFileTool NetworkFileTool;
typedef int func(NetworkFileTool *);

struct NetworkFileTool{
    int sock;//sockect description
    struct sockaddr_in* addr;//address
    int connect_flag;
    char* buf;

    func *msend_file;
    func *mrecevie_file;
    func *mget_directory;//client get file directory from server 
    func *mdelete;//delete the struct
};


void create_tcp_socket(NetworkFileTool* networkFileTool,const char * ip,const u_int16_t port);
int valid_FileName(char * filename);
int send_file(NetworkFileTool* networkFiletool);

#endif