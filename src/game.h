#pragma once

#include <libdragon.h>

#include <mem_pool.h>
#include <scene_manager.h>

#include "stickers.h"

extern struct controller_data controller_data;
extern SceneManager *scene_manager;
extern MemZone memory_pool;

extern sprite_t *sprite_game_ui;

extern size_t sticker_count[STICKERS_MAX];
