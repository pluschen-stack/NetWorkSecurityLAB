#ifndef __MUDP_H
#define __MUDP_H

#include "util.h"

//udp报文头
typedef struct udp{
    u_short udp_srcport;//源端口 两字节
    u_short udp_dstport;//目的端口 两字节
    u_short udp_len; //总长度 两字节
    u_short udp_checksum; //校验和 两字节
}udp_t;

#endif