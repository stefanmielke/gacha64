#include <stdio.h>

#include <libdragon.h>

#include "online/online.h"

typedef enum GameState {
	GS_OpenGacha,

	GS_Trade,
	GS_TradeStart,
	GS_TradeWaitStart,
} GameState;
GameState state;

struct controller_data controller_data;

void setup();
void update();
void render();

int main(void) {
	setup();

	while (1) {
		update();

		render();
	}
}

void setup() {
	display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
	dfs_init(DFS_DEFAULT_LOCATION);
	rdp_init();
	timer_init();
	controller_init();

	online_init();

	state = GS_OpenGacha;
}

void update() {
	online_tick();

	controller_scan();
	controller_data = get_keys_down();

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
}

void render() {
	static display_context_t disp = 0;
	while (!(disp = display_lock()))
		;

	graphics_fill_screen(disp, 0);
	graphics_set_color(0xFFFFFFFF, 0);

	const int start_x = 400, start_y = 50;
	graphics_draw_text(disp, start_x, start_y - 20, "LATEST MESSAGES:");
	for (size_t i = 0; i < responses_total_lines; ++i) {
		graphics_draw_text(disp, start_x, start_y + (i * 16), &responses[i][0]);
	}

	display_show(disp);
}
