#include "game_scene.h"

#include <menu.h>

#include "scene_loader.h"
#include "../game.h"
#include "../online/online.h"

typedef struct GameScreen {
	Menu *menu;
} GameScreen;
GameScreen *game_data;

void game_scene_create() {
	game_data = mem_zone_alloc(&memory_pool, sizeof(GameScreen));

	game_data->menu = menu_init(&memory_pool, 2, 2, 30, 30, 16, NULL);
	menu_add_item(game_data->menu, "Open Gacha", true, NULL);
	menu_add_item(game_data->menu, "Trade Online", is_online, NULL);
}

short game_scene_tick() {
	int option = menu_tick(game_data->menu, &controller_data);
	switch (option) {
		case 0: {
			char notification[255];
			snprintf(notification, 255, "%s_got_a_'%s'", "Mielke", "Bearly");
			online_notify(notification);
		} break;
		case 1:
			return SCENE_TRADE;
		default:
			break;
	}

	return SCENE_GAME;
}

void game_scene_display(display_context_t disp) {
	graphics_fill_screen(disp, 0);
	graphics_set_color(0xFFFFFFFF, 0);

	const int start_x = 400, start_y = 50;
	graphics_draw_text(disp, start_x, start_y - 20, "LATEST MESSAGES:");
	for (size_t i = 0; i < responses_total_lines; ++i) {
		graphics_draw_text(disp, start_x, start_y + (i * 16), &responses[i][0]);
	}

	menu_render(game_data->menu, disp);
}

void game_scene_destroy() {
}
