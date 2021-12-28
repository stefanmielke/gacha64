#include "color.h"

uint32_t MESSAGE_TEXT_COLOR;

void colors_init() {
	MESSAGE_TEXT_COLOR = graphics_make_color(50, 50, 50, 255);
}