#include "de/de.h"

#include "game/mgame/mgame.h"

#include "game/ex/shd-metaball.h"
// void wschat(void );

int SDL_main(int argc, char**argv){ (void)argv, (void)argc;

	// Dsnd_init_conf.bufsize = 128;
	// Dsnd_init_conf.sample_rate = 22050;
	// Dsnd_init_conf.format = AUDIO_S16SYS;
	de_init(0,0,0);
	// de_init(512,512,0);
	// de_init(1024,512,0);

	mgame_main();
	// ex_shd_metaball();
	// golf_main();

// dmouse_hide(0);
// dmouse_grab(0);
// dvsync(0);

	de_quit();

	exit(0);
	return 0;
}






	/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * This is a WebSocket client example.
 * Prints WS frames to stdout and forwards stdin to server.
 */
#ifdef __cplusplus
extern "C" {
#endif

// #include "3rdparty/mongoose.h"

#ifdef __cplusplus
}
#endif


// static int s_done = 0;
// static int s_is_connected = 0;
// int said_hi=0;




// static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
//   (void) nc;

//   switch (ev) {
//     case MG_EV_CONNECT: {
//       int status = *((int *) ev_data);
//       if (status != 0) {
//         printf("-- Connection error: %d\n", status);
//       }
//       break;
//     }
//     case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
//       struct http_message *hm = (struct http_message *) ev_data;
//       if (hm->resp_code == 101) {
//         printf("-- Connected\n");
//         s_is_connected = 1;
//       } else {
//         printf("-- Connection failed! HTTP code %d\n", hm->resp_code);
//         /* Connection will be closed after this. */
//       }
//       break;
//     }
//     case MG_EV_POLL: {
//       // char msg[500];
//     	if (said_hi) break;
//     	said_hi=1;
//       char* msg="hallo";
//       int n = 5;
//       mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, msg, n);
//       break;
//     }
//     case MG_EV_WEBSOCKET_FRAME: { // 
//       struct websocket_message *wm = (struct websocket_message *) ev_data;
//       printf("%.*s\n", (int) wm->size, wm->data);
//       break;
//     }
//     case MG_EV_CLOSE: {
//       if (s_is_connected) printf("-- Disconnected\n");
//       s_done = 1;
//       break;
//     }
//   }
// }

// void wschat(void ){
//   struct mg_mgr mgr;
//   struct mg_connection *nc;
//   // const char *chat_server_url = "ws://127.0.0.1:7777";
//   // const char *chat_server_url = "ws://127.0.0.1:7777";
//   // const char *chat_server_url = "ws://192.168.0.220:8000";
//   // const char *chat_server_url = "ws://xb17.duckdns.org:7777";
//   // const char *chat_server_url = "ws://xb17.duckdns.org:1717";
//   const char *chat_server_url = "ws://192.168.0.220:1717";
//   // const char *chat_server_url = "ws://181.167.117.180:1717";
//   // const char *chat_server_url = "ws://192.168.0.25:8000";

//   mg_mgr_init(&mgr, NULL);

//   nc = mg_connect_ws(&mgr, ev_handler, chat_server_url, "ws_chat", NULL);
//   if (nc == NULL) {
//     fprintf(stderr, "Invalid address\n");
//     return ;
//   }
// int timePrev=0, time=SDL_GetPerformanceCounter();
// float dt=0, ftime=0;
//   // while (!s_done) { 
//   for (int i=0; i<300; i++) { 
//   	dvsync(1);
//   		timePrev=time;
// 		time=SDL_GetPerformanceCounter();
// 		dt = (float)((time - timePrev)*1000.0 / (double)SDL_GetPerformanceFrequency() )/1000.f;
// 		ftime+=dt;
// // DE_LOG("dt=%f",dt);
//   	dclear_color(v4f_rand());
//   	dclear(0);
//   	ddisplay();
//     mg_mgr_poll(&mgr, 16);
//   }
//   mg_mgr_free(&mgr);

//   return ;

// }









