#include <game/productivity.h>
#include <stdlib.h>

struct productivity *
productivity_init ()
{
	struct productivity *pt = malloc (sizeof (struct productivity));
	pt->background = rect_init (1024, 64);
	pt->back_progress = rect_init (512, 50);
	pt->progress = rect_init (510, 48);

	rect_set_color (pt->background, 0xac / 255.f, 0xac / 255.f, 0xac / 255.f, 1.f);
	rect_set_color (pt->back_progress, 0x4c / 255.f, 0x4c / 255.f, 0x4c / 255.f, 1.f);
	rect_set_color (pt->progress, 0x00 / 255.f, 0xff / 255.f, 0x00 / 255.f, 1.f);

	pt->tm = timer_init (1);

	return pt;
}

void 
productivity_set_pos (struct productivity *pt, int x, int y)
{
	rect_set_pos (pt->background, x, y);
	rect_set_pos (pt->back_progress, x + pt->background->width / 2 - pt->back_progress->width / 2, y + pt->background->height / 2 - pt->back_progress->height / 2);
	rect_set_pos (pt->progress, x + pt->background->width / 2 - pt->back_progress->width / 2 + 1, y + pt->background->height / 2 - pt->back_progress->height / 2 + 1);
}

void
productivity_set (struct productivity *pt, int percent)
{
	if (percent <= 0) percent = 0;
	if (percent >= 100) percent = 100;
	
	int new_width = percent * (pt->back_progress->width - 2) / 100;

	pt->percent = percent;

	rect_set_size (pt->progress, new_width, pt->progress->height);

	if (new_width <= 10) {
		rect_set_color (pt->progress, 0xff / 255.f, 0x00 / 255.f, 0x00 / 255.f, 1.f);
	} else if (new_width <= 60) {
		rect_set_color (pt->progress, 0xc7 / 255.f, 0xb6 / 255.f, 0x4f / 255.f, 1.f);
	} else {
		rect_set_color (pt->progress, 0x00 / 255.f, 0xff / 255.f, 0x00 / 255.f, 1.f);
	}
}

void 
productivity_render (struct productivity *pt)
{
	rect_render (pt->background);
	rect_render (pt->back_progress);
	rect_render (pt->progress);
}