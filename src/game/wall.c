#include <game/wall.h>
#include <shaders/list.h>
#include <textures/list.h>
#include <stdlib.h>
#include <stdio.h>

struct wall *
wall_init ()
{
	struct wall *h = malloc (sizeof (struct wall));
	h->sprite = sprite_init (TEXTURE_WALL, COORD_SYSTEM_INT);
	h->sprite->shader = simple_2d_shader_render;

	sprite_move_init (h->sprite);
	
	sprite_set_pos (h->sprite, 0.f, 0.f, 0.f);
	sprite_scale (h->sprite, 1.f);

	return h;
}

void
wall_scale (struct wall *h, float scale)
{
	sprite_scale (h->sprite, scale);
}

void
wall_step (struct wall *h)
{

}

void
wall_render (struct wall *h)
{
	sprite_render (h->sprite);
}