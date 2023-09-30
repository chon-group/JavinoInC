#ifndef __JAVINO_H__

#define __JAVINO_H__

#include <stdio.h>

#define JAVINO_HEADER_LEN 6

int javino_init(const char *wr_port, const char *rd_port);

int avaliable_msg();

char* javino_get_msg();

int javino_send_msg(const char* msg_to_send);

#endif