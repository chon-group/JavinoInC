#ifndef __JAVINO_H__

#define __JAVINO_H__

char *javino_get_msg(const char* port);

int javino_send_msg(const char* port, const char* msg_to_send);


#endif