#ifndef OFFICE_FIRED_HEADER_H
#define OFFICE_FIRED_HEADER_H
#include <core/sprite.h>
#include <core/animation.h>

struct anim;

struct fired {
	struct sprite *sprite;
	struct anim *animation;
	int is_play;
};

struct fired *fired_init();
void fired_scale(struct fired *h, float scale);
void fired_step(struct fired *h);
void fired_render(struct fired *h);

#endif