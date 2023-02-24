#ifndef OFFICE_LOGIC_WALL_HEADER_H
#define OFFICE_LOGIC_WALL_HEADER_H
#include <core/sprite.h>
#include <core/animation.h>

struct anim;

struct wall {
	struct sprite *sprite;
	struct anim *animation;
};

struct wall *wall_init();
void wall_scale(struct wall *h, float scale);
void wall_step(struct wall *h);
void wall_render(struct wall *h);

#endif