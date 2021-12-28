#include "game.h"

#include <stdio.h>
#include <libdragon.h>
#include <menu.h>
#include <memory.h>

#include "color.h"
#include "online/online.h"
#include "scenes/scene_loader.h"

struct controller_data controller_data;
SceneManager *scene_manager;
MemZone memory_pool;

size_t sticker_count[STICKERS_MAX];

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
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
	dfs_init(DFS_DEFAULT_LOCATION);
	rdp_init();
	timer_init();
	controller_init();

	online_init();

	mem_zone_init(&memory_pool, 1024 * 1024);

	menu_global_init();
	colors_init();

	scene_manager = scene_manager_init(NULL, &memory_pool, change_scene);
	scene_manager_change_scene(scene_manager, SCENE_GAME);

	memset(sticker_count, 0, sizeof(size_t) * STICKERS_MAX);
}

void update() {
	online_tick();

	controller_scan();
	controller_data = get_keys_down();

	scene_manager_tick(scene_manager);
}

void render() {
	static display_context_t disp = 0;
	while (!(disp = display_lock()))
		;

	scene_manager_display(scene_manager, disp);

	display_show(disp);
}
