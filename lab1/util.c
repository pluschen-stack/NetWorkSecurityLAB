#include"util.h"

/**
 * @brief Check if a string ends in specific length.
 * @param buf the string buffer.
 * @param max_length the max length desired. Ending '\0' is not included.
 * @return int 1 if valid, 0 if invalid
 */
int valid_String(char * buf,unsigned max_length){
    for (int i = 0; i < max_length + 1; ++i)
    {
        if (buf[i] == '\0')
            return 1;
    }
    return 0;
}

/**
 * @brief parse for destip and destport
 * 
 * @param argc the number of argument
 * @param argv the argument array
 * @param ip the ip address
 * @param port the port between 1 and 65535
 */
void parse_command_line(int argc,char **argv,char *ip,u_int16_t* port){
    printf("argc = %d\n",argc);
    if(argv[1] != NULL && valid_String(argv[1],15)){
        strcpy(ip,argv[1]);
    }else{
        strcpy(ip,DEFAULT_IP);
    }
    if(argv[2] != NULL && atoi(argv[2]) <= 65535 && atoi(argv[2]) >=1){
        *port = atoi(argv[2]);
    }else{
        *port = DEFAULT_PORT;
    }
    printf("ip:%s,port:%d\n",ip,*port);
}

