#ifndef __MTCP_H
#define __MTCP_H

#include "util.h"

#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) > 4)
u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

//TCP报文头
typedef struct tcp{
    u_short tcp_srcport;//源端口 两字节
    u_short tcp_dstport;//目的端口 两字节
    u_int tcp_seq;//序列号 四字节
    u_int tcp_ack;//确认号 四字节
    u_char tcp_headlen;//头部长度 一字节的前四位，后面两位保留
    u_char tcp_flag;//标志位 一字节的低六位，前四位保留
    u_short tcp_win;//窗口大小 两字节
    u_short tcp_checksum;//校验和 两字节
    u_short tcp_urp;//紧急指针 两字节
}tcp_t;

#endif