#include"util.h"
#include"NetworkFileTool.h"

NetworkFileTool* create_server(const char *ip,const u_int16_t port){

    DEBUG(printf("sizeof(NetworkFileTool) = %ld\n",sizeof(NetworkFileTool)));

    NetworkFileTool* server = (NetworkFileTool*) malloc(sizeof(NetworkFileTool));
    create_tcp_socket(server,ip,port);
    DEBUG(printf("server->sock = %d\n",server->sock));
    server->msend_file = &send_file;
    return server;
}

void simple_cli(NetworkFileTool* server){
    printf("-------------------------------------\n");
    DEBUG(printf("server->sock = %d\n",server->sock));
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)server->addr;
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
    printf("The IP address is: %s\n", ipAddress);
    if(bind(server->sock,(struct sockaddr*)server->addr,sizeof(struct sockaddr))==0){
        listen(server->sock,20);//进入监听状态，等待用户发起请求
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(server->sock,(struct sockaddr*)&client_addr,&client_addr_size);
        char* buf = malloc(DEFAULT_BUFF_SIZE);
        read(client_sock,buf,DEFAULT_BUFF_SIZE);
        printf("%s\n",buf);
    }else{
        printf("server bind false");
    }
    
}

int main(int argc,char *argv[]){   
    char ip[16]; 
    u_int16_t port;
    setbuf(stdout,0);
    parse_command_line(argc,argv,ip,&port);

    NetworkFileTool* server = create_server(ip,port);
    
    simple_cli(server);

// //创建套接字
//     int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
//     //将套接字和IP、端口绑定
//     struct sockaddr_in serv_addr;
//     memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
//     serv_addr.sin_family = AF_INET;  //使用IPv4地址
//     serv_addr.sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
//     serv_addr.sin_port = htons(port);  //端口
//     // int c = serv_sock;
//     // printf("%d",c);
//     int a = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//     perror("error:");
//     printf("%d",a);
//     //进入监听状态，等待用户发起请求
//     listen(serv_sock, 20);
//     //接收客户端请求
//     struct sockaddr_in clnt_addr;
//     socklen_t clnt_addr_size = sizeof(clnt_addr);
//     int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
//     //向客户端发送数据
//     char str[] = "Hello World!";
//     write(clnt_sock, str, sizeof(str));
   
//     //关闭套接字
//     close(clnt_sock);
//     close(serv_sock);
//     return 0;
}



