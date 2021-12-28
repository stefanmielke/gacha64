#include "trade_scene.h"

#include "scene_loader.h"
#include "../game.h"
#include "../online/online.h"

typedef enum TradeState {
	GS_TradeStart,
	GS_TradeConnecting,
	GS_TradeWaitClient,
	GS_TradeClient1Chooses,
	GS_TradeClient2Chooses,
	GS_TradeClient1Confirms,
	GS_TradeClient1ContinueConfirmation,
	GS_TradeClient1Left,
	GS_TradeClient2Left,
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
	if (controller_data.c[0].A)
		trade_data->state++;

	switch (trade_data->state) {
		case GS_TradeStart:
			online_start_exchange();
			trade_data->state = GS_TradeConnecting;
			break;
		case GS_TradeConnecting:
			if (network_state == NS_Connected)
				trade_data->state = GS_TradeWaitClient;
			break;
		case GS_TradeWaitClient:
		case GS_TradeClient1Chooses:
		case GS_TradeClient2Chooses:
		case GS_TradeClient1Confirms:
		case GS_TradeClient1ContinueConfirmation:
		case GS_TradeClient1Left:
		case GS_TradeClient2Left:
		default:
			break;
	}

	return SCENE_TRADE;
}

void trade_scene_display(display_context_t disp) {
	graphics_fill_screen(disp, 0);
	graphics_set_color(0xFFFFFFFF, 0);

	graphics_draw_text(disp, 30, 20, "Online Trade");

	// Draw status
	{
		const int x = 30, y = 40;
		switch (trade_data->state) {
			case GS_TradeStart:
				graphics_draw_text(disp, x, y, "Creating the trade server...");
				break;
			case GS_TradeConnecting:
				graphics_draw_text(disp, x, y, "Connecting to server...");
				break;
			case GS_TradeWaitClient:
				graphics_draw_text(disp, x, y, "Waiting for another\nplayer to connect...");
				break;
			case GS_TradeClient1Chooses:
				graphics_draw_text(disp, x, y, "Waiting for Player 1\nto choose...");
				break;
			case GS_TradeClient2Chooses:
				graphics_draw_text(disp, x, y, "Waiting for Player 2\nto choose...");
				break;
			case GS_TradeClient1Confirms:
				graphics_draw_text(disp, x, y, "Waiting for Player 1\nconfirmation...");
				break;
			case GS_TradeClient1ContinueConfirmation:
				graphics_draw_text(disp, x, y, "Waiting for Player 1\nto continue trading...");
				break;
			case GS_TradeClient1Left:
				graphics_draw_text(disp, x, y, "Player 1 left.\nLeaving the trade...");
				break;
			case GS_TradeClient2Left:
				graphics_draw_text(disp, x, y, "Player 2 left.\nRestarting the trade...");
				break;
			default:
				break;
		}
	}
}

void trade_scene_destroy() {
}
