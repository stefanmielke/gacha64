#pragma once

#include <libdragon.h>

typedef enum NetState {
	NS_GetNotification,
	NS_Paused,

	NS_AwaitingResponse,
	NS_SendingNotification,
	NS_GettingNotifications,
	NS_RequestingServer,
	NS_ConnectingServer,
	NS_Connected,
	NS_DisconnectingServer,
} NetState;

extern bool is_online;
extern NetState network_state;

extern char responses_total_lines;
extern char responses[20][255];	 // 20 lines, max 255 chars each

void online_init();
void online_tick();
void online_notify(char *message);
