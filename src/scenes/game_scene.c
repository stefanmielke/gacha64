#include "game_scene.h"

#include "scene_loader.h"
#include "../game.h"
#include "../online/online.h"

typedef struct MainScreen {
} MainScreen;
MainScreen *main_data;

void game_scene_tween_color_callback(void *target, float current);
void game_scene_tween_color_callback_end(void *target);

void game_scene_create() {
	main_data = mem_zone_alloc(&memory_pool, sizeof(MainScreen));
}

short game_scene_tick() {
	if (state == GS_OpenGacha) {
		if (controller_data.c[0].A) {
			// open gacha!
			char notification[255];
			snprintf(notification, 255, "%s_got_a_'%s'", "Mielke", "Bearly");
			online_notify(notification);
		} else if (controller_data.c[0].B) {
			// connect to exchange server
			state = GS_TradeStart;
		}
	} else if (state > GS_Trade) {
		switch (state) {
			case GS_TradeStart:
				online_start_exchange();
				state = GS_TradeWaitStart;
				break;
			case GS_TradeWaitStart:
			default:
				break;
		}
	}

	return SCENE_MAIN;
}

void game_scene_display(display_context_t disp) {
	graphics_fill_screen(disp, 0);
	graphics_set_color(0xFFFFFFFF, 0);

	const int start_x = 400, start_y = 50;
	graphics_draw_text(disp, start_x, start_y - 20, "LATEST MESSAGES:");
	for (size_t i = 0; i < responses_total_lines; ++i) {
		graphics_draw_text(disp, start_x, start_y + (i * 16), &responses[i][0]);
	}
}

void game_scene_destroy() {
}
