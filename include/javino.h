#ifndef __JAVINO_H__

#define __JAVINO_H__

#include <stdio.h>


#define JAVINO_HEADER_LEN 6

char *javino_get_msg(int port);

int javino_send_msg(int port, const char* msg_to_send);

#endif