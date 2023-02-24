#ifndef OFFICE_LOGIC_FLOOR_HEADER_H
#define OFFICE_LOGIC_FLOOR_HEADER_H
#include <core/sprite.h>
#include <core/animation.h>
#include <stdint.h>

struct anim;
struct human;
struct computer_board;

struct floor {
	struct sprite *sprite;
	struct anim *animation;
	struct human *human;
	struct computer_board *com_board;
	uint32_t com_board_part;
	int tilex;
	int tiley;
};

struct floor *floor_init();
void floor_assign(struct floor *h, struct human *hu);
void floor_scale(struct floor *h, float scale);
void floor_step(struct floor *h);
void floor_render(struct floor *h, int without_tile);

#endif