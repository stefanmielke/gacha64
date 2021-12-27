#include "queue.h"

#include <memory.h>

#define MAX_NOTIFICATIONS 20
QueueItem notifications[MAX_NOTIFICATIONS];
int current_enqueue_pos;
int current_dequeue_pos;

void queue_init() {
	current_enqueue_pos = 0;
	current_dequeue_pos = 0;
}

void queue_enqueue(QueueItem *item) {
	memcpy(&notifications[current_enqueue_pos], item, sizeof(QueueItem));

	++current_enqueue_pos;
	if (current_enqueue_pos >= MAX_NOTIFICATIONS)
		current_enqueue_pos = 0;
}

void queue_notification(const char *message, size_t size) {
	if (size > 255)
		size = 255;

	QueueItem queue_item;
	queue_item.type = QIT_Notification;

	strcpy(queue_item.data.notification.message, message);

	queue_enqueue(&queue_item);
}

void queue_request_server() {
	QueueItem queue_item;
	queue_item.type = QIT_RequestServer;

	queue_enqueue(&queue_item);
}

QueueItem *queue_dequeue() {
	if (current_dequeue_pos == current_enqueue_pos) {
		return NULL;
	}

	QueueItem *item = &notifications[current_dequeue_pos];
	++current_dequeue_pos;

	if (current_dequeue_pos >= MAX_NOTIFICATIONS)
		current_dequeue_pos = 0;

	return item;
}