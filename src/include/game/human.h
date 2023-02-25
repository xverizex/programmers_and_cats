#ifndef OFFICE_LOGIC_HUMAN_HEADER_H
#define OFFICE_LOGIC_HUMAN_HEADER_H
#include <core/sprite.h>
#include <core/animation.h>
#include <game/path.h>
#include <game/floor.h>
#include <game/a_star.h>
#include <core/timer.h>
#include <game/board.h>

struct anim;

struct human {
	struct sprite *sprite;
	struct anim *animation;
	struct path *path;
	struct a_star *a_star;
	struct times *not_found_path;
	struct times *cats;
	struct computer_board *com_board;
	struct floor ***floor;

	uint32_t need_to_clear_a_star;
	uint32_t *speed_time;

	int index_path;
	int num_path;

	int need_to_watch_cats;

	uint32_t random_cats;

	int is_working;
	int movex;
	int movey;
	int scale;
	int pivot_x;
	int pivot_y;

	int tilex;
	int tiley;
	int oldtilex;
	int oldtiley;
	int dstx;
	int dsty;

	int is_assigned;

	int is_visible;
};

struct human *human_init(int *speed_time);
void human_set_scale(struct human *h, float sc);
void human_set_path(struct human *h, struct path *p);
void human_step(struct human *h, int sizeh, int sizew, struct floor ***floor);
void human_render(struct human *h);

#endif