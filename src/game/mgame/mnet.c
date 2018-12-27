#include "mnet.h"


void net_data_test(void){



#if 0 //crazy slovek cracker
//lol

//server
	int curid=0;
	int playernum=0;

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);
	darr* socketvector = darr_new(sizeof(net_data), NULL, 1000) ;
	char tmp[1400];
	SDLNet_SocketSet sockets;
bool running = true;
	TCPsocket server= SDLNet_TCP_Open(&ip);

	while(running){ if(dusek(DK_ESC)) running=false;;

		TCPsocket tmpsocket = SDLNet_TCP_Accept(server);

		if(tmpsocket){
			if(playernum<30){
				SDLNet_TCP_AddSocket(sockets, tmpsocket);
				net_data data = Net_data(tmpsocket, SDL_GetTicks(), curid);
				darr_add(socketvector, &data);
				playernum++;

				sprintf(tmp, "0 %d \n", curid);
				curid++;
				DE_LOG("new connection: %d", curid);
			}else{
				sprintf(tmp, "3 \n");
			}
			SDLNet_TCP_Send(tmpsocket, tmp, strlen(tmp)+1);
		}


		while(SDLNet_CheckSockets(sockets, 0)>0){
			for (int i = 0; i < darr_len(socketvector); ++i){
				net_data data = darr_read(net_data, socketvector, i);
				if(SDLNet_SocketReady(data.socket)){
					//incoming data
					data.timeout=SDL_GetTicks();
					SDLNet_TCP_Recv(data.socket, tmp, 1400);
					int num=tmp[0]-'0';
					int j=1;
					while(tmp[j]>='0' && tmp[j]<='9'){ //text number to numer
						num*=10;
						// num+=data[j]-'0'; ??
						num+=tmp[j]-'0';
						j++;
					}
					if(num==1){
						for (int k = 0; k < darr_len(socketvector); ++k){
							if(k==i) continue;
							SDLNet_TCP_Send(data.socket, tmp, strlen(tmp)+1);
						}
					} else if(num==2){
						for (int k = 0; k < darr_len(socketvector); ++k){
							if(k==i) continue;
							SDLNet_TCP_Send(data.socket, tmp, strlen(tmp)+1);
							SDLNet_TCP_DelSocket(sockets, data.socket);
							SDLNet_TCP_Close(data.socket);
							darr_rem(socketvector, i);
							playernum--;
						}
					} else if(num==3){ //player shot another???
						int tmp2;
						sscanf(tmp2,"3 %d", &tmp2);

					}

				}
			}
		}

	}
	// SDLNet_TCP_Recv
#endif
}

