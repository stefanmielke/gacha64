#include "online.h"

#include <memory.h>
#include "network.h"
#include "notification_queue.h"

bool is_online;

bool awaiting_http_response;
char responses_total_lines;
char responses[20][255];  // 20 lines, max 255 chars each

void online_init() {
	is_online = network_initialize();
	if (!is_online) {
		responses_total_lines = 1;
		memcpy(responses[0], "Offline Mode", 12);

		notification_queue_init();
	}
}

void online_tick() {
	static bool already_done = false;
	if (is_online) {
		if (awaiting_http_response) {
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
					} break;
					case NETTYPE_URL_POST:
						already_done = false;
						break;
					default:
						break;
				}

				awaiting_http_response = false;
			}
		} else {
			char *message = notification_dequeue();
			if (message) {
				char url[255];
				snprintf(url, 255, "http://localhost:5050/notifications?message=%s", message);
				network_url_post(url);
				awaiting_http_response = true;
			} else {
				if (!already_done) {
					network_url_fetch("http://localhost:5050/notifications/");
					awaiting_http_response = true;
					already_done = true;
				}
			}
		}
	}
}

void online_notify(char *message) {
	if (!is_online)
		return;

	notification_enqueue(message, strlen(message));
}
