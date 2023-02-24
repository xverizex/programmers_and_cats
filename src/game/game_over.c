#include <game/game_over.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/text.h>

extern uint32_t screen_width;
extern uint32_t screen_height;

struct game_over *
game_over_init ()
{
	struct game_over *pt = malloc (sizeof (struct game_over));
	pt->background = rect_init (1300, 200);
	rect_set_color (pt->background, 0xac / 255.f, 0xac / 255.f, 0xac / 255.f, 1.f);

	return pt;
}

void
game_build_text (struct game_over *p, int seconds)
{
	p->msg = malloc (sizeof (wchar_t) * 255);
	swprintf (p->msg, 255, L"Your company lasted: %d seconds", seconds);

	p->builded = text_build (p->msg, 28);
}

void 
game_over_set_pos (struct game_over *pt, int x, int y)
{
	rect_set_pos (pt->background, x, y);
}

void 
game_over_render (struct game_over *pt)
{
	rect_render (pt->background);

	text_render (pt->builded, screen_width / 2 - pt->builded->size * 28 / 2, screen_height / 2, 1, 0, 0, 0);

}