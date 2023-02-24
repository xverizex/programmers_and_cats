#ifndef OFFICE_RECT_HEADER_H
#define OFFICE_RECT_HEADER_H

#include <stdint.h>

struct rect {
	float ortho[16];
	float pos[16];
	float color[4];

	float x;
	float y;
	float z;

	float v_data[18];

	uint32_t width;
	uint32_t height;

	float pivot_x;
	float pivot_y;
	
	float sc[16]; // scale
	float scale;

	void (*shader) (struct rect *);
};

struct rect *rect_init(int width, int height);
void rect_set_pos(struct rect *rt, int x, int y);
void rect_set_scale(struct rect *rt, float sc);
void rect_set_size (struct rect *rt, int width, int height);
void rect_set_color(struct rect *rt, float red, float green, float blue, float alpha);
void rect_render(struct rect *rt);

#endif