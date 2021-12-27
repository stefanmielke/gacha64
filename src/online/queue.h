#pragma once

#include <libdragon.h>

typedef enum QueueItemType {
	QIT_Notification,
	QIT_RequestServer,
} QueueItemType;

typedef struct QueueDataNotification {
	char message[255];
} QueueDataNotification;

typedef struct QueueDataRequestServer {
} QueueDataRequestServer;

typedef union QueueData {
	QueueDataNotification notification;
	QueueDataRequestServer request_server;
} QueueData;

typedef struct QueueItem {
	QueueItemType type;
	QueueData data;
} QueueItem;

void queue_init();
void queue_enqueue(QueueItem *item);
QueueItem *queue_dequeue();

void queue_notification(const char *message, size_t size);
void queue_request_server();
