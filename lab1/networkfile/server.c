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
        server->buf = malloc(DEFAULT_BUFF_SIZE);
        memset(server->buf,0,DEFAULT_BUFF_SIZE);
        
        recv(client_sock,server->buf,DEFAULT_BUFF_SIZE,0);
        if(!strncmp(server->buf,"upload",6)){
            int file_size = atoi(server->buf+16);
            char filename[DEFAULT_FILENAME_SIZE];
            memset(filename,0,DEFAULT_FILENAME_SIZE);
            strncpy(filename,strstr(server->buf,"filename=")+9,(strstr(server->buf," end"))-(strstr(server->buf,"filename=")+9));
            printf("uploaded file_size = %d\n",file_size);
            printf("uploaded filename = %s\n",filename);
            int remain_data = file_size;
            strcat(filename,"copied");
            FILE* file = fopen(filename,"w");
            
            char* real_start = strstr(server->buf," end")+4;
            fwrite(real_start,sizeof(char), strlen(real_start), file);
            int len = strlen(real_start);
            remain_data -= len;
            fprintf(stdout, "Receive %d bytes and we still hope : %d bytes\n", len, remain_data);
            while((remain_data > 0) && ((len = recv(client_sock, server->buf, DEFAULT_BUFF_SIZE, 0)) > 0)){
                fwrite(server->buf ,sizeof(char), len<remain_data?len:remain_data, file);
                remain_data -= len;
                fprintf(stdout, "Receive %d bytes and we still hope : %d bytes\n", len<remain_data?len:(remain_data<0?0:remain_data), remain_data);
            }
        }else if(!strncmp(server->buf,"download",8)){
            char filename[DEFAULT_FILENAME_SIZE];
            memset(filename,0,DEFAULT_FILENAME_SIZE);
            strncpy(filename,strstr(server->buf,"filename=")+9,(strstr(server->buf," end"))-(strstr(server->buf,"filename=")+9));
            FILE* file;
            if((file = fopen(filename,"r"))==NULL){
                perror("file open error:");
            }

            printf("uploading %s to client\n",filename);

            fseek(file,0,SEEK_END);
            int file_size = ftell(file);
            fseek(file,0,SEEK_SET);
            char str[100];

            sprintf(str, "upload filesize=%d,filename=%s end", file_size,filename);
            
            strcpy(server->buf,str);
            DEBUG(printf("server->buf = %s\n",server->buf));
            
            send(client_sock,server->buf,strlen(server->buf),0);
            memset(server->buf,0,DEFAULT_BUFF_SIZE);
            while(fread(server->buf,sizeof(char),DEFAULT_BUFF_SIZE,file)>0 ){
                DEBUG(printf("%s\n",server->buf));
                send(client_sock,server->buf,strlen(server->buf),0);
                memset(server->buf,0,DEFAULT_BUFF_SIZE);
            }
            
            printf("the file size is %d\n",file_size);
            close(server->sock);
            close(client_sock);
            fclose(file);
        }
            
        
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
    // char str[] = "Hello World!";
    // write(clnt_sock, str, sizeof(str));
   
//     //关闭套接字
//     close(clnt_sock);
//     close(serv_sock);
//     return 0;
}



