#ifndef __METHERNET_H
#define __METHERNET_H

#define ETHERNET_ADDR_LENGTH 6

#include "util.h"

//以太网帧头
typedef struct ethernet{
    u_char eth_dsthost[ETHERNET_ADDR_LENGTH];//目的主机Mac地址
    u_char eth_srchost[ETHERNET_ADDR_LENGTH];//源主机Mac地址
    u_short eth_type;//协议类型
}ethernet_t;

#endif