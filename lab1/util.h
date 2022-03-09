#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUGON
#define DEBUG(stmt) stmt
#else
#define DEBUG(stmt)
#endif


#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 1234

int valid_String(char * buf,unsigned max_length);
void parse_command_line(int argc,char **argv,char *ip,u_int16_t* port);

#endif