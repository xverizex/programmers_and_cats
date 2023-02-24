#include <game/floor.h>
#include <game/board.h>
#include <shaders/list.h>
#include <textures/list.h>
#include <GLES3/gl3.h>
#include <stdlib.h>
#include <game/human.h>

enum ANIM {
	ANIM_CODE,
	ANIM_CATS,
	N_ANIM
};

#define SPEED_ANIM         200

struct computer_board *
computer_board_init (int type, int tilex, int tiley, int sizeh, int sizew, struct floor ***f)
{
	struct computer_board *b = malloc (sizeof (struct computer_board));
	b->sprite = sprite_init (TEXTURE_BOARD, COORD_SYSTEM_INT);
	b->sprite->shader = simple_2d_shader_render;
	b->is_free = 1;
	b->is_working = 0;
	b->type = type;

	switch (type) {
		case FROM_LEFT_DOWN_TO_RIGHT_UP:
			f[tiley][tilex + 0]->com_board = b;
			f[tiley][tilex + 0]->com_board_part = 0;
			f[tiley][tilex + 1]->com_board = b;
			f[tiley][tilex + 1]->com_board_part = 1;
			b->sprite->cur_tex = 0;
			b->tilex = tilex;
			b->tiley = tiley + 1;
			b->ptile[0].x = tilex;
			b->ptile[0].y = tiley + 1;
			b->ptile[1].x = tilex + 1;
			b->ptile[1].y = tiley + 1;
			b->anim = anim_init (N_ANIM);
			anim_set (b->anim, ANIM_CODE, 1, 2, SPEED_ANIM);
			anim_set (b->anim, ANIM_CATS, 3, 4, SPEED_ANIM);
			break;
		case FROM_LEFT_UP_TO_RIGHT_DOWN:
			f[tiley + 0][tilex + 0]->com_board = b;
			f[tiley + 0][tilex + 0]->com_board_part = 0;
			f[tiley + 1][tilex + 0]->com_board = b;
			f[tiley + 1][tilex + 0]->com_board_part = 1;
			b->sprite->cur_tex = 5;
			b->tilex = tilex + 1;
			b->tiley = tiley;
			b->ptile[0].x = tilex + 1;
			b->ptile[0].y = tiley + 0;
			b->ptile[1].x = tilex + 1;
			b->ptile[1].y = tiley + 1;
			b->anim = anim_init (N_ANIM);
			anim_set (b->anim, ANIM_CODE, 6, 7, SPEED_ANIM);
			anim_set (b->anim, ANIM_CATS, 8, 9, SPEED_ANIM);
			break;
	}

	return b;
}

void
board_working (struct computer_board *b, struct human *h)
{
	switch (b->type) {
		case FROM_LEFT_DOWN_TO_RIGHT_UP:
			floor_assign (h->floor[h->tiley][h->tilex], h);
			floor_assign (h->floor[h->tiley][h->tilex + 1], h);
			h->sprite->cur_tex = 12;

			sprite_set_pos (h->sprite, b->sprite->x, b->sprite->y - (16 * h->scale), 0.f);
			break;
		case FROM_LEFT_UP_TO_RIGHT_DOWN:
			floor_assign (h->floor[h->tiley][h->tilex], h);
			floor_assign (h->floor[h->tiley + 1][h->tilex + 1], h);
			h->sprite->cur_tex = 16;

			sprite_set_pos (h->sprite, b->sprite->x + (2 * h->scale), b->sprite->y - (16 * h->scale), 0.f);
			break;
	}	
}

void
board_set_still (struct computer_board *h)
{
	switch (h->type) {
		case FROM_LEFT_DOWN_TO_RIGHT_UP: 
			h->sprite->cur_tex = 0;
			break;
		case FROM_LEFT_UP_TO_RIGHT_DOWN:
			h->sprite->cur_tex = 5;
			break;
	}
}

void
board_unsit (struct human *h)
{
	switch (h->com_board->type) {
		case FROM_LEFT_DOWN_TO_RIGHT_UP:
			h->floor[h->tiley][h->tilex]->human = NULL;
			h->floor[h->tiley][h->tilex + 1]->human = NULL;
			break;
		case FROM_LEFT_UP_TO_RIGHT_DOWN:
			h->floor[h->tiley][h->tilex]->human = NULL;
			h->floor[h->tiley + 1][h->tilex + 1]->human = NULL;
			break;
	}
}


void
board_set_scale (struct computer_board *h, float sc)
{
	h->scale = sc;
	sprite_scale (h->sprite, sc);
}

void 
computer_board_render (struct computer_board *b, int part)
{
	//glEnable (GL_SCISSOR_TEST);

	int w = b->sprite->tex->width_orig / 2 * b->sprite->scale;
	int h = b->sprite->tex->height_orig * b->sprite->scale;

	int posx = part * w;
	int posy = 0;

	int x = b->sprite->x + b->sprite->pivot_x;
	int y = b->sprite->y + b->sprite->pivot_y;

	//glScissor (x + posx, y + posy, w, h);
	sprite_render (b->sprite);
	//glDisable (GL_SCISSOR_TEST);
}