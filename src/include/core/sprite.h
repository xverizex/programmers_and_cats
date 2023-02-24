#ifndef OFFICE_SPRITE_H
#define OFFICE_SPRITE_H
#include <stdint.h>
#include <core/downloader.h>
#include <core/timer.h>

enum COORD_SYSTEM {
	COORD_SYSTEM_INT,
	COORD_SYSTEM_FLOAT,
	N_COORD_SYSTEM
};

struct sprite {
	float ortho[16];
	float pos[16];

	uint32_t cur_tex;
	struct texture *tex;

	float x;
	float y;
	float z;

	float pivot_x;
	float pivot_y;
	
	float sc[16]; // scale
	float scale;

	struct times *tm;

	void (*shader) (struct sprite *);
};


struct sprite *sprite_init(uint32_t res, const int coord_system);
void sprite_scale(struct sprite *sp, float sc);
void sprite_set_pos(struct sprite *sp, float x, float y, float z);
void sprite_set_pivot (struct sprite *sp, float x, float y);
void sprite_render(struct sprite *sp);
void sprite_move_init(struct sprite *sp);
void sprite_move_stop(struct sprite *sp);
void sprite_move(struct sprite *sp, const float x, const float y, const float z, const uint32_t ms);
void sprite_set_size(struct sprite *sp, float size);

#endif