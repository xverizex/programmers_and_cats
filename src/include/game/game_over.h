#ifndef OFFICE_GAME_OVER_HEADER_H
#define OFFICE_GAME_OVER_HEADER_H
#include <stdint.h>
#include <core/rect.h>
#include <core/timer.h>
#include <core/text.h>
#include <wchar.h>

struct game_over {
	struct rect *background;
	struct text *builded;
	wchar_t *msg;

	struct times *tm;
};

struct game_over *game_over_init();
void game_over_set_pos(struct game_over *pt, int x, int y);
void game_over_render(struct game_over *pt);
void game_build_text(struct game_over *p, int seconds);

#endif