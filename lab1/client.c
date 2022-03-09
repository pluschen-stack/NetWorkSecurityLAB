#include "util.h"
#include "NetworkFileTool.h"

NetworkFileTool* create_client(const char *ip,const u_int16_t port){

    DEBUG(printf("sizeof(NetworkFileTool) = %ld\n",sizeof(NetworkFileTool)));

    NetworkFileTool* client = (NetworkFileTool*) malloc(sizeof(NetworkFileTool));
    create_tcp_socket(client,ip,port);
    client->msend_file = &send_file;
    return client;
}

void simple_cli(NetworkFileTool* client){
    printf("-------------------------------------\n");
    printf("what do you want to do?\n");
    printf("1.download file from server\n");
    printf("2.upload file to server\n");
    printf("please input (1 or 2 for select) or (0 for exit):");
    int select = -1;
    scanf("%d",&select);
    while(select != 1 && select != 2 && select != 0){
        printf("invalid input,please input again (1 or 2 for select) or (0 for exit):");
        if(scanf("%d",&select)!=1){
            //set buf 0
            char c;
            while ((c = getchar()) != EOF && c != '\n');
        }
    }
    char filename[50]; 
    if(select == 1){
        printf("please input file name to download:");
        if(scanf("%s",filename) == 1 && valid_FileName(filename)){
            printf("downloading %s from server\n",filename);
        }
    }else if(select == 2){
        printf("please input file name to upload:");
        if(scanf("%s",filename) == 1 && valid_FileName(filename)){
            FILE* file;
            if((file = fopen(filename,"r"))!=NULL){
                printf("uploading %s to server\n",filename);
                char* buf = malloc(sizeof(DEFAULT_BUFF_SIZE));
                memset(buf,0,sizeof(DEFAULT_BUFF_SIZE));
                fread(buf,1,DEFAULT_BUFF_SIZE,file);
                printf("buf = %s\n",buf);
                client->buf = buf;
                client->msend_file(client);
                close(client->sock);
            }else{
                printf("%s does not exist\n",filename);
            }
            
        }
    }
}

int main(int argc,char *argv[]) {
    char ip[16]; 
    u_int16_t port;
    parse_command_line(argc,argv,ip,&port);
    
    NetworkFileTool* client = create_client(ip,port);
    
    simple_cli(client);
    // connect(client->sock, (struct sockaddr*)client->addr, sizeof(struct sockaddr));
    // perror("error:");
    // //读取服务器传回的数据
    // char buffer[40];
    // read(client->sock ,buffer, sizeof(buffer)-1);
   
    // printf("Message form server: %s\n", buffer);
   
    // //关闭套接字
    // close(client->sock);
    return 0;
}