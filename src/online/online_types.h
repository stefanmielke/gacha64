#pragma once

#include <stddef.h>

typedef unsigned int u32;

typedef enum NetPacketType {
	NPT_SendStickerId,
} NetPacketType;

typedef struct NetPacketSendStickerId {
	u32 sticker_id;
} NetPacketSendStickerId;

typedef union NetPacketData {
	NetPacketSendStickerId send_sticker_id;
} NetPacketData;

typedef struct NetPacket {
	NetPacketType type;
	NetPacketData data;
} NetPacket;
