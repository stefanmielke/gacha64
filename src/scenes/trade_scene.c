#include "trade_scene.h"

#include "scene_loader.h"
#include "../game.h"
#include "../online/online.h"

typedef enum TradeState {
	GS_TradeStart,
	GS_TradeWaitStart,
} TradeState;

typedef struct TradeScreen {
	TradeState state;
} TradeScreen;
TradeScreen *trade_data;

void trade_scene_create() {
	trade_data = mem_zone_alloc(&memory_pool, sizeof(TradeScreen));
	trade_data->state = GS_TradeStart;
}

short trade_scene_tick() {
	switch (trade_data->state) {
		case GS_TradeStart:
			online_start_exchange();
			trade_data->state = GS_TradeWaitStart;
			break;
		case GS_TradeWaitStart:
		default:
			break;
	}

	return SCENE_TRADE;
}

void trade_scene_display(display_context_t disp) {
	graphics_fill_screen(disp, 0);
	graphics_set_color(0xFFFFFFFF, 0);

	const int start_x = 400, start_y = 50;
	graphics_draw_text(disp, start_x, start_y - 20, "TRADING");
}

void trade_scene_destroy() {
}
