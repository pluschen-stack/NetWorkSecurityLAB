#ifndef __UTIL_H
#define __UTIL_H

#ifndef DEBUGON
#define DEBUG(stmt) stmt
#else
#define DEBUG(stmt)
#endif

#define u_char u_int8_t
#define u_short u_int16_t

#endif