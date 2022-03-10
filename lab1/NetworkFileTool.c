#include "NetworkFileTool.h"
#include "util.h"

/**
 * @brief Check if the file name is valid.
 * 
 * @param s a valid C string.
 * @return int 1 if valid, 0 if invalid.
 */
int valid_FileName(char * filename){
    char c;
    int length = 0;
    while ((c = *filename++) != '\0')
    {
        length++;
        if (c == '\\' || c == '/' || c == '*' 
            || c == '?' || c == '<' || c == '>' || c == '|' || length >= DEFAULT_FILENAME_SIZE)
            return 0;
    }
    return 1;
}

void create_tcp_socket(NetworkFileTool* networkFiletool,const char * ip,const u_int16_t port){
	//创建套接字
    networkFiletool->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    networkFiletool->addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    networkFiletool->addr->sin_family = AF_INET;  //使用IPv4地址
    networkFiletool->addr->sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
    networkFiletool->addr->sin_port = htons(port);  //端口
    networkFiletool->connect_flag = connect(networkFiletool->sock,(struct sockaddr*)networkFiletool->addr,
        sizeof(struct sockaddr));
}

int send_file(NetworkFileTool* networkFiletool){
    if(!networkFiletool->connect_flag){
        DEBUG(printf("strlen(networkFiletool->buf) = %ld\n",strlen(networkFiletool->buf)));
        if(send(networkFiletool->sock,networkFiletool->buf,strlen(networkFiletool->buf),0)<=0){
            DEBUG(perror(""));
            networkFiletool->connect_flag = -1;
        }
        return 1;
    }else{
        printf("connect false");
        return 0;
    }
    
}