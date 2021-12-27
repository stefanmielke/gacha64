#include "scene_loader.h"

#include "../game.h"
#include "game_scene.h"
#include "trade_scene.h"

void change_scene(short curr_scene, short next_scene) {
	switch (next_scene) {
		case SCENE_GAME:
			scene_manager_set_callbacks(scene_manager, &game_scene_create, &game_scene_tick,
										&game_scene_display, &game_scene_destroy);
			break;
		case SCENE_TRADE:
			scene_manager_set_callbacks(scene_manager, &trade_scene_create, &trade_scene_tick,
										&trade_scene_display, &trade_scene_destroy);
			break;
		default:
			abort();
	}
}
