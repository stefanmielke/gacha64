#include "game_scene.h"

#include <menu.h>
#include <spritesheet.h>
#include <random.h>

#include "scene_loader.h"
#include "../color.h"
#include "../game.h"
#include "../gfx/game_ui.h"
#include "../online/online.h"

typedef struct GameScreen {
	Menu *menu;
	sprite_t *background;
	sprite_t *stickers;
	sprite_t *game_ui;
	size_t gacha_count;
} GameScreen;
GameScreen *game_data;

void game_scene_create() {
	game_data = mem_zone_alloc(&memory_pool, sizeof(GameScreen));

	game_data->menu = menu_init(&memory_pool, 2, 2, 30, 30, 16, NULL);
	menu_add_item(game_data->menu, "Open Gacha", true, NULL);
	menu_add_item(game_data->menu, "Trade Online", is_online, NULL);

	game_data->background = spritesheet_load(&memory_pool, "/gfx/background-stickers.sprite");
	game_data->stickers = spritesheet_load(&memory_pool, "/gfx/stickers.sprite");
	game_data->game_ui = spritesheet_load(&memory_pool, "/gfx/game_ui.sprite");

	game_data->gacha_count = 5;
}

short game_scene_tick() {
	int option = menu_tick(game_data->menu, &controller_data);
	switch (option) {
		case 0: {
			if (game_data->gacha_count == 0) {
				game_data->menu->items[0].enabled = false;
				break;
			}

			int gacha_id = RANDN(STICKERS_MAX);
			sticker_count[gacha_id]++;

			char notification[255];
			snprintf(notification, 255, "%s[%d]", "Mielke", gacha_id);
			online_notify(notification);

			game_data->gacha_count--;
			if (game_data->gacha_count == 0) {
				game_data->menu->items[0].enabled = false;
			}
		} break;
		case 1:
			return SCENE_TRADE;
		default:
			break;
	}

	return SCENE_GAME;
}

void game_scene_display(display_context_t disp) {
	graphics_draw_sprite(disp, 0, 0, game_data->background);

	// render stickers the player has
	{
		graphics_set_color(0xFFFFFF, 0);
		int start_x = 20, start_y = 88;
		for (size_t i = 0; i < STICKERS_MAX; ++i) {
			if (sticker_count[i] == 0)
				continue;

			const int x = start_x + ((i % 12) * 24);
			const int y = start_y + ((i / 12) * 24);

			graphics_draw_sprite_trans_stride(disp, x, y, game_data->stickers, i);

			char text[3];
			if (sticker_count[i] <= 9)
				snprintf(text, 3, "%d", sticker_count[i]);
			else
				snprintf(text, 3, "9+");

			graphics_draw_text(disp, x, y + 16, text);
		}
	}

	// render gacha count
	{
		int start_x = 182, start_y = 15;
		for (size_t i = 0; i < game_data->gacha_count; ++i) {
			int x = start_x - ((i % 2) * 14);
			int y = start_y + ((i / 2) * 14);
			graphics_draw_sprite_trans_stride(disp, x, y, game_data->game_ui, SPRITE_game_ui_gacha);
		}
	}

	graphics_set_color(MESSAGE_TEXT_COLOR, 0);

	const int start_x = 200, start_y = 15;
	for (size_t i = 0; i < responses_total_lines; ++i) {
		graphics_draw_text(disp, start_x, start_y + (i * 10), &responses[i][0]);
	}

	menu_render(game_data->menu, disp);
}

void game_scene_destroy() {
}
