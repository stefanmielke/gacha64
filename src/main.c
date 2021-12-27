#include <stdio.h>

#include <libdragon.h>

#include "online/online.h"

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
}

void update() {
	online_tick();

	controller_scan();
	controller_data = get_keys_down();

	// open gacha!
	if (controller_data.c[0].A) {
		char notification[255];
		snprintf(notification, 255, "%s_got_a_'%s'", "Mielke", "Bearly");
		online_notify(notification);
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
