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
            client->buf = malloc(DEFAULT_BUFF_SIZE);
            memset(client->buf,0,DEFAULT_BUFF_SIZE);
            char str[100];
            sprintf(str, "download filename=%s end", filename);
            strcpy(client->buf,str);
            client->msend_file(client);
            while(recv(client->sock,client->buf,DEFAULT_BUFF_SIZE,0)<=0);
            
            if(!strncmp(client->buf,"upload",6)){
                int file_size = atoi(client->buf+16);
                char filename[DEFAULT_FILENAME_SIZE];
                memset(filename,0,DEFAULT_FILENAME_SIZE);
                strncpy(filename,strstr(client->buf,"filename=")+9,(strstr(client->buf," end"))-(strstr(client->buf,"filename=")+9));
                printf("received file_size = %d\n",file_size);
                printf("received filename = %s\n",filename);
                int remain_data = file_size;
                strcat(filename,"copied");
                FILE* file = fopen(filename,"w");
                
                char* real_start = strstr(client->buf," end")+4;
                fwrite(real_start,sizeof(char), strlen(real_start), file);
                int len = strlen(real_start);
                remain_data -= len;
                fprintf(stdout, "Receive %d bytes and we still hope : %d bytes\n", len, remain_data);
                while((remain_data > 0) && ((len = recv(client->sock, client->buf, DEFAULT_BUFF_SIZE, 0)) > 0)){
                    fwrite(client->buf ,sizeof(char), len<remain_data?len:remain_data, file);
                    remain_data -= len;
                    fprintf(stdout, "Receive %d bytes and we still hope : %d bytes\n", len<remain_data?len:(remain_data<0?0:remain_data), remain_data);
                }
            }else{
                printf("client->buf = %s",client->buf);
            }
            
            
        }
    }else if(select == 2){
        printf("please input file name to upload:");
        if(scanf("%s",filename) == 1 && valid_FileName(filename)){
            FILE* file;
            if((file = fopen(filename,"r"))!=NULL){
                printf("uploading %s to server\n",filename);

                fseek(file,0,SEEK_END);
                int file_size = ftell(file);
                fseek(file,0,SEEK_SET);

                client->buf = malloc(DEFAULT_BUFF_SIZE);
                memset(client->buf,0,DEFAULT_BUFF_SIZE);
                
                char str[100];
                sprintf(str, "upload filesize=%d,filename=%s end", file_size,filename);
                strcpy(client->buf,str);
                client->msend_file(client);

                while(fread(client->buf,sizeof(char),DEFAULT_BUFF_SIZE,file)>0 ){
                    DEBUG(printf("%s",client->buf));
                    client->msend_file(client);
                    
                }
                
                printf("the file size is %d\n",file_size);
                close(client->sock);
                fclose(file);
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
    // //??????????????????????????????
    // char buffer[40];
    // read(client->sock ,buffer, sizeof(buffer)-1);
   
    // printf("Message form server: %s\n", buffer);
   
    // //???????????????
    // close(client->sock);
    return 0;
}