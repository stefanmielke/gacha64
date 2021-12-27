#pragma once

#include <libdragon.h>

void notification_queue_init();
void notification_enqueue(const char *message, size_t size);
char *notification_dequeue();
