#ifndef OFFICE_ANIMATION_HEADER_H
#define OFFICE_ANIMATION_HEADER_H
#include <core/sprite.h>
#include <stdint.h>

struct anim *anim_init(const int size);
void anim_set(struct anim *an, const uint32_t index, const uint32_t first, const uint32_t last, const uint32_t milliseconds);
uint32_t anim_run(struct anim *an, const uint32_t index, const uint32_t once);
void anim_stop(struct anim *an, const uint32_t index);

#endif