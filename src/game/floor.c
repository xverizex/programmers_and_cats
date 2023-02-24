#include <game/floor.h>
#include <shaders/list.h>
#include <textures/list.h>
#include <stdlib.h>
#include <stdio.h>
#include <game/human.h>
#include <game/board.h>

struct floor *
floor_init (int yy, int xx)
{
	struct floor *h = malloc (sizeof (struct floor));
	h->sprite = sprite_init (TEXTURE_FLOOR, COORD_SYSTEM_INT);
	h->sprite->shader = simple_2d_shader_render;

	sprite_move_init (h->sprite);
	
	sprite_set_pos (h->sprite, 0.f, 0.f, 0.f);
	sprite_scale (h->sprite, 1.f);

	h->human = NULL;
	h->com_board = NULL;
	h->com_board_part = 0;

	h->tilex = xx;
	h->tiley = yy;

	return h;
}

void 
floor_assign(struct floor *h, struct human *human)
{
	h->human = human;
	human->is_assigned = 1;
}

void
floor_scale (struct floor *h, float scale)
{
	sprite_scale (h->sprite, scale);
}

void
floor_step (struct floor *h)
{

}

void
floor_render (struct floor *h, int without_tile)
{
	if (!without_tile)
		sprite_render (h->sprite);
	
	if (h->com_board) {
		computer_board_render (h->com_board, h->com_board_part);
	}

	if (h->human && h->human->is_visible) {
		human_render (h->human);
	}
	
}