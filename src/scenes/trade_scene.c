#include "trade_scene.h"

#include <menu.h>
#include <spritesheet.h>

#include "scene_loader.h"
#include "../game.h"
#include "../gfx/game_ui.h"
#include "../online/queue.h"
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
	Menu *menu;
	sprite_t *stickers;

	int player_1_chosen_sticker;
	int player_2_chosen_sticker;
} TradeScreen;
TradeScreen *trade_data;

void trade_scene_create() {
	trade_data = mem_zone_alloc(&memory_pool, sizeof(TradeScreen));
	trade_data->state = GS_TradeStart;

	trade_data->stickers = spritesheet_load(&memory_pool, "/gfx/stickers.sprite");

	trade_data->menu = menu_init(&memory_pool, STICKERS_MAX, 7, 70, 30, 16, NULL);
	menu_set_hand(trade_data->menu, 20, 8);

	for (size_t i = 0; i < STICKERS_MAX; i++) {
		if (sticker_count[i] > 0) {
			menu_add_item_image(trade_data->menu, trade_data->stickers, i, true, (void *)i);
		}
	}
}

short trade_scene_tick() {
	if (controller_data.c[0].A)
		trade_data->state++;

	if (controller_data.c[0].B)
		return SCENE_GAME;

	int option = menu_tick(trade_data->menu, &controller_data);
	if (option >= 0) {
		trade_data->player_1_chosen_sticker = (size_t)trade_data->menu->items[option].object;
	}

	switch (trade_data->state) {
		case GS_TradeStart:
			queue_request_server();
			trade_data->state = GS_TradeConnecting;
			trade_data->player_1_chosen_sticker = -1;
			trade_data->player_2_chosen_sticker = 0;
			break;
		case GS_TradeConnecting:
			if (network_state == NS_Connected)
				trade_data->state = GS_TradeWaitClient;
			break;
		case GS_TradeWaitClient:
			break;
		case GS_TradeClient1Chooses:
			break;
		case GS_TradeClient2Chooses:
			break;
		case GS_TradeClient1Confirms:
			break;
		case GS_TradeClient1ContinueConfirmation:
			break;
		case GS_TradeClient1Left:
			break;
		case GS_TradeClient2Left:
			break;
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

	// Draw stickers menu
	{ menu_render(trade_data->menu, disp); }

	// Draw chosen stickers
	{
		rdp_sync(SYNC_PIPE);
		rdp_set_default_clipping();
		rdp_enable_texture_copy();
		rdp_attach_display(disp);

		if (trade_data->player_1_chosen_sticker >= 0) {
			rdp_sync(SYNC_PIPE);
			rdp_load_texture_stride(0, 0, MIRROR_DISABLED, trade_data->stickers,
									trade_data->player_1_chosen_sticker);

			rdp_draw_sprite_scaled(0, 200, 70, 2, 2, MIRROR_DISABLED);
		}

		if (trade_data->player_2_chosen_sticker >= 0) {
			rdp_sync(SYNC_PIPE);
			rdp_load_texture_stride(0, 0, MIRROR_DISABLED, trade_data->stickers,
									trade_data->player_2_chosen_sticker);

			rdp_draw_sprite_scaled(0, 200, 130, 2, 2, MIRROR_DISABLED);
		}

		rdp_detach_display();
	}
}

void trade_scene_destroy() {
	queue_disconnect_server();
}
