#include "notification_queue.h"

#include <memory.h>

#define MAX_NOTIFICATIONS 20
char notifications[MAX_NOTIFICATIONS][255];
int current_enqueue_pos;
int current_dequeue_pos;

void notification_queue_init() {
	current_enqueue_pos = 0;
	current_dequeue_pos = 0;
}

void notification_enqueue(const char *message, size_t size) {
	if (size > 255)
		size = 255;

	strcpy(notifications[current_enqueue_pos], message);

	++current_enqueue_pos;
	if (current_enqueue_pos >= MAX_NOTIFICATIONS)
		current_enqueue_pos = 0;
}

char *notification_dequeue() {
	if (current_dequeue_pos == current_enqueue_pos) {
		return NULL;
	}

	char *message = notifications[current_dequeue_pos];
	++current_dequeue_pos;

	if (current_dequeue_pos >= MAX_NOTIFICATIONS)
		current_dequeue_pos = 0;

	return message;
}