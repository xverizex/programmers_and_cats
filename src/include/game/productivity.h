#ifndef OFFICE_PRODUCTIVITY_HEADER_H
#define OFFICE_PRODUCTIVITY_HEADER_H
#include <stdint.h>
#include <core/rect.h>
#include <core/timer.h>

struct productivity {
	struct rect *background;
	struct rect *back_progress;
	struct rect *progress;

	struct times *tm;

	uint32_t percent;
};

struct productivity *productivity_init();
void productivity_set_pos(struct productivity *pt, int x, int y);
void productivity_render(struct productivity *pt);
void productivity_set(struct productivity *pt, int percent);

#endif