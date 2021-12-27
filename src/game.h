#pragma once

#include <libdragon.h>

#include <mem_pool.h>
#include <scene_manager.h>

typedef enum GameState {
	GS_OpenGacha,

	GS_Trade,
	GS_TradeStart,
	GS_TradeWaitStart,
} GameState;

extern struct controller_data controller_data;
extern SceneManager *scene_manager;
extern MemZone memory_pool;
extern GameState state;
