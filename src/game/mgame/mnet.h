#ifndef __M_NET_H__
#define __M_NET_H__

#include "mgame.h"


typedef struct net_data{
	TCPsocket socket;
	Uint32 timeout;
	int id;
} net_data;
#define Net_data(s,t,i) {socket:s, timeout:t, id:i}


void net_data_test(void);
// net_data net_data_make(TCPsocket socket, Uint32 timeout, int id);





#endif
