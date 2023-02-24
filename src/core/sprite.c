#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <core/sprite.h>
#include <core/cmath.h>
#include <textures/list.h>
#include <string.h>

extern uint32_t screen_width;
extern uint32_t screen_height;

struct sprite *
sprite_init (uint32_t res, const int coord_system)
{
	if (res >= N_TEXTURES) {
		return NULL;
	}

	struct sprite *sp = malloc (sizeof (struct sprite));
	sp->tex = downloader_load_texture (res);
	sp->cur_tex = 0;

	float aspect = ((float) screen_width) / ((float) screen_height);
	float width = screen_width;
	float height = screen_height;

	switch (coord_system) {
		case COORD_SYSTEM_FLOAT:
			math_ortho (sp->ortho, -(0.5f * aspect), 0.5f * aspect, 0.5f, -0.5f, 0.0f, 10.f);
			break;
		case COORD_SYSTEM_INT:
			math_ortho (sp->ortho, 0.f, width, 0.f, height, 0.f, 10.f);
			break;
	}

	memset (sp->sc, 0, sizeof (float) * 16);	

	sprite_scale (sp, 1.f);	

	sp->tm = NULL;

	return sp;
}

void 
sprite_set_pivot (struct sprite *sp, float x, float y)
{
	sp->pivot_x = x;
	sp->pivot_y = y;
}


void
sprite_set_size (struct sprite *sp, float size)
{
	#if 0
	
	#endif

	float w = size;
	float h = w * sp->tex->aspect;

	float ww = w / 2.f;
	float hh = h / 2.f;

	float v[18] = {
		-ww, -hh, 0.0f,
		-ww, hh, 0.0f,
		ww, -hh, 0.0f,
		ww, -hh, 0.0f,
		ww, hh, 0.0f,
		-ww, hh, 0.0f
	};

	memcpy (sp->tex->v_data, v, sizeof (float) * 18);

	sp->tex->w = w;
	sp->tex->h = h;
}

void
sprite_scale (struct sprite *sp, float sc)
{
	sp->scale = sc;
	math_scale (sp->sc, sc);
}

void
sprite_set_pos (struct sprite *sp, float x, float y, float z)
{
	sp->x = x;
	sp->y = y;
	sp->z = z;

	math_translate (sp->pos, x + sp->pivot_x * sp->scale, y + sp->pivot_y * sp->scale, z);
}

void
sprite_render (struct sprite *sp)
{
	if (sp->shader)
		sp->shader (sp);
}

void
sprite_move_init (struct sprite *sp)
{
	sp->tm = timer_init (1);
}

void
sprite_move_stop (struct sprite *sp)
{
	timer_stop (sp->tm, 0);
}

void
sprite_move (struct sprite *sp, const float x, const float y, const float z, const uint32_t ms)
{
	if (timer_run (sp->tm, 0, ms)) {
		float xx = sp->x -= x;
		float yy = sp->y += y;
		float zz = sp->z += z;
		sprite_set_pos (sp, xx, yy, zz);
	}
}