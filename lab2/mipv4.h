#ifndef __MIPV4_H
#define __MIPV4_H

#include "util.h"


#define IP_ADDR_LEN 4

//IPV4报文头
typedef struct ipv4{
    u_char ip_hlv; //版本号+头部长度 一字节
    u_char ip_tos; //区分服务 一字节
    u_short ip_len; //总长度 两字节
    u_short ip_id; //标识 两字节
    u_short ip_foff; //标志3位+片偏移13位 两字节
    u_char ip_ttl; //跳步限制 一字节
    u_char ip_pro; //上层协议 一字节
    u_short ip_checksum; //首部校验和 两字节
    u_char ip_src[IP_ADDR_LEN]; //源IP地址 四字节
    u_char ip_dst[IP_ADDR_LEN]; //目的IP地址 四字节
}ipv4_t;

#endif