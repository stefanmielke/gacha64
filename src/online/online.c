#include "online.h"

#include <memory.h>
#include "network.h"
#include "queue.h"
#include "online_types.h"

bool is_online;

char responses_total_lines;
char responses[20][255];  // 20 lines, max 255 chars each

char server_url[255];
char server_connect[255];

NetState network_state;

void online_init() {
	is_online = network_initialize();
	if (!is_online) {
		responses_total_lines = 1;
		memcpy(responses[0], "Offline Mode", 12);

		queue_init();
	} else {
		network_state = NS_GetNotification;
	}
}

void online_tick() {
	if (!is_online)
		return;

	if (network_state > NS_AwaitingResponse) {
		int header;
		if ((header = usb_poll())) {
			int type = USBHEADER_GETTYPE(header);
			int size = USBHEADER_GETSIZE(header);
			char buffer[size];
			usb_read(buffer, size);

			switch (type) {
				case NETTYPE_TEXT: {
					memset(responses, 0, 20 * 255);
					responses_total_lines = 0;

					int cur_line = 0;
					int cur_column = 0;
					for (size_t i = 0; i < size; ++i) {
						if (buffer[i] == '\n') {
							++cur_line;
							cur_column = 0;
							if (cur_line >= 20)
								break;
						} else {
							if (cur_column < 255) {
								responses[cur_line][cur_column] = buffer[i];
							}
							++cur_column;
						}
					}

					responses_total_lines = cur_line + 1;
					network_state = NS_Paused;
				} break;
				case NETTYPE_UDP_CONNECT: {
					network_state = NS_Connected;
				} break;
				case NETTYPE_UDP_DISCONNECT: {
					network_state = NS_Paused;
				} break;
				case NETTYPE_URL_POST: {
					switch (network_state) {
						case NS_SendingNotification:
							network_state = NS_GetNotification;
							break;
						case NS_RequestingServer: {
							memset(server_url, 0, 255);
							memset(server_connect, 0, 255);

							int cur_line = 0;
							int cur_column = 0;
							for (size_t i = 0; i < size; ++i) {
								if (buffer[i] == '\n') {
									++cur_line;
									cur_column = 0;
									if (cur_line >= 2)
										break;
								} else {
									if (cur_column < 255) {
										if (cur_line == 0)
											server_url[cur_column] = buffer[i];
										else
											server_connect[cur_column] = buffer[i];
									}
									++cur_column;
								}
							}

							network_state = NS_ConnectingServer;
							network_udp_connect(server_connect);
						} break;
						default:
							break;
					}
				} break;
				default:
					break;
			}
		}
	} else {
		QueueItem *item = queue_dequeue();
		if (item) {
			switch (item->type) {
				case QIT_Notification: {
					char url[255];
					snprintf(url, 255, "http://localhost:5050/notifications?message=%s",
							 item->data.notification.message);
					network_url_post(url);
					network_state = NS_SendingNotification;
				} break;
				case QIT_RequestServer: {
					network_url_post("http://localhost:5050/exchanges");
					network_state = NS_RequestingServer;
				} break;
				case QIT_DisconnectServer: {
					network_udp_disconnect();
					network_state = NS_DisconnectingServer;
				} break;
				case QIT_SelectSticker: {
					NetPacket packet;
					packet.type = NPT_SendStickerId;
					packet.data.send_sticker_id.sticker_id = item->data.select_sticker.sticker_id;

					network_udp_send_data(&packet, sizeof(NetPacket));
				} break;
				default:
					break;
			}
		} else if (network_state == NS_GetNotification) {
			network_url_fetch("http://localhost:5050/notifications/");
			network_state = NS_GettingNotifications;
		}
	}
}

void online_notify(char *message) {
	if (!is_online)
		return;

	queue_notification(message, strlen(message));
}
