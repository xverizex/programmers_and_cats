#ifndef OFFICE_COMPUTER_BOARD_HEADER_H
#define OFFICE_COMPUTER_BOARD_HEADER_H
#include <core/sprite.h>
#include <core/animation.h>
#include <game/floor.h>

enum BOARD_TYPE {
	FROM_LEFT_DOWN_TO_RIGHT_UP,
	FROM_LEFT_UP_TO_RIGHT_DOWN,
	N_BOARD_TYPE
};

struct paint_tile {
	int x;
	int y;
};

struct computer_board {
	struct sprite *sprite;
	struct anim *anim;

	uint32_t tilex;
	uint32_t tiley;
	uint32_t is_free;
	uint32_t is_working;
	float scale;
	uint32_t type;
	struct paint_tile ptile[2];
};

struct computer_board *computer_board_init(int type, int tilex, int tiley, int sizeh, int sizew, struct floor ***floor);
void board_set_scale(struct computer_board *h, float sc);
void computer_board_render(struct computer_board *b, int part);
void board_working(struct computer_board *b, struct human *h);
void board_unsit(struct human *h);
void board_set_still(struct computer_board *h);

#endif