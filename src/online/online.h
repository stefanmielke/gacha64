#pragma once

#include <libdragon.h>

extern bool is_online;

extern char responses_total_lines;
extern char responses[20][255];	 // 20 lines, max 255 chars each

void online_init();
void online_tick();
void online_notify(char *message);
