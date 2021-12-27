#include "online.h"

#include <memory.h>
#include "network.h"
#include "notification_queue.h"

bool is_online;

char responses_total_lines;
char responses[20][255];  // 20 lines, max 255 chars each

typedef enum NetState {
	NS_GetNotification,
	NS_Paused,

	NS_AwaitingResponse,
	NS_SendingNotification,
	NS_GettingNotifications,
} NetState;

void online_init() {
	is_online = network_initialize();
	if (!is_online) {
		responses_total_lines = 1;
		memcpy(responses[0], "Offline Mode", 12);

		notification_queue_init();
	}
}

void online_tick() {
	static NetState network_state = NS_GetNotification;
	if (is_online) {
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
					case NETTYPE_URL_POST: {
						switch (network_state) {
							case NS_SendingNotification:
								network_state = NS_GetNotification;
								break;
							default:
								break;
						}
					} break;
					default:
						break;
				}
			}
		} else {
			char *message = notification_dequeue();
			if (message) {
				char url[255];
				snprintf(url, 255, "http://localhost:5050/notifications?message=%s", message);
				network_url_post(url);
				network_state = NS_SendingNotification;
			} else if (network_state == NS_GetNotification) {
				network_url_fetch("http://localhost:5050/notifications/");
				network_state = NS_GettingNotifications;
			}
		}
	}
}

void online_notify(char *message) {
	if (!is_online)
		return;

	notification_enqueue(message, strlen(message));
}
