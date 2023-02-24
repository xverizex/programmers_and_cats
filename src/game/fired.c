#include <game/fired.h>
#include <stdlib.h>
#include <stdio.h>
#include <textures/list.h>
#include <shaders/list.h>


struct fired *
fired_init ()
{
	struct fired *f = malloc (sizeof (struct fired));
	f->sprite = sprite_init (TEXTURE_FIRED, COORD_SYSTEM_INT);
	f->sprite->shader = simple_2d_shader_render;

	f->animation = anim_init(1);
	anim_set (f->animation, 0, 0, 13, 50);
	f->is_play = 0;

	return f;
}

void 
fired_scale (struct fired *h, float scale)
{
	sprite_scale (h->sprite, scale);
}

void 
fired_step (struct fired *h)
{
	
}

void 
fired_render (struct fired *h)
{
	if (h->is_play) {
		h->sprite->cur_tex = anim_run (h->animation, 0, 1);
		sprite_render (h->sprite);
		if (h->sprite->cur_tex == 13) h->is_play = 0;
	}
		
}
