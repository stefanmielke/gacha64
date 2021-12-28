#pragma once

#include <stddef.h>

typedef enum NetPacketType {
	NPT_SendStickerId,
} NetPacketType;

typedef struct NetPacketSendStickerId {
	size_t sticker_id;
} NetPacketSendStickerId;

typedef union NetPacketData {
	NetPacketSendStickerId send_sticker_id;
} NetPacketData;

typedef struct NetPacket {
	NetPacketType type;
	NetPacketData data;
} NetPacket;
