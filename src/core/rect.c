#include <core/rect.h>
#include <stdint.h>
#include <core/cmath.h>
#include <shaders/2d_simple_color.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t screen_width;
extern uint32_t screen_height;

struct rect *
rect_init (int width, int height)
{
	struct rect *rt = malloc (sizeof (struct rect));

	math_ortho (rt->ortho, 0.f, screen_width, 0.f, screen_height, 0.f, 10.f);

	rt->shader = simple_color_2d_shader_render;

	float v[18] = {
		0.f, 0.f, 0.0f,
		0.f, height, 0.0f,
		width, 0.f, 0.0f,
		width, 0.f, 0.0f,
		width, height, 0.0f,
		0.f, height, 0.0f
	};

	memcpy (rt->v_data, v, 18 * sizeof (float));

	rt->width = width;
	rt->height = height;

	return rt;
}

void 
rect_set_size(struct rect *rt, int width, int height)
{
	float v[18] = {
		0.f, 0.f, 0.0f,
		0.f, height, 0.0f,
		width, 0.f, 0.0f,
		width, 0.f, 0.0f,
		width, height, 0.0f,
		0.f, height, 0.0f
	};

	memcpy (rt->v_data, v, 18 * sizeof (float));

	rt->width = width;
	rt->height = height;
}

void 
rect_set_pos (struct rect *rt, int x, int y)
{
	rt->x = x;
	rt->y = y;
	math_translate (rt->pos, x, y, 0.f);
}

void 
rect_set_scale (struct rect *rt, float sc)
{
	rt->scale = sc;
}

void 
rect_set_color (struct rect *rt, float red, float green, float blue, float alpha)
{
	rt->color[0] = red;
	rt->color[1] = green;
	rt->color[2] = blue;
	rt->color[3] = alpha;
}


void 
rect_render (struct rect *rt)
{
	rt->shader (rt);
}