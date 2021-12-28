#pragma once

#include <libdragon.h>

typedef enum QueueItemType {
	QIT_Notification,
	QIT_RequestServer,
	QIT_DisconnectServer,
} QueueItemType;

typedef struct QueueDataNotification {
	char message[255];
} QueueDataNotification;

typedef struct QueueDataRequestServer {
} QueueDataRequestServer;

typedef struct QueueDataDisconnectServer {
} QueueDataDisconnectServer;

typedef union QueueData {
	QueueDataNotification notification;
	QueueDataRequestServer request_server;
	QueueDataDisconnectServer disconnect_server;
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
void queue_disconnect_server();
