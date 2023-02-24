#ifndef OFFICE_A_STAR_HEADER_H
#define OFFICE_A_STAR_HEADER_H
#include <stdint.h>
#include <game/floor.h>
#include <game/path.h>

struct map_a_star {
	uint32_t ceil;
	uint32_t calced;
	uint32_t link;
	uint32_t next;
	uint32_t weight;
	uint32_t x;
	uint32_t y;
	uint32_t walk;
};

struct a_star {
	struct map_a_star *map;
	uint32_t size_w;
	uint32_t size_h;
};

struct a_star *a_star_init(uint32_t width, uint32_t height, struct floor ***floor);
struct path *a_star_get_path(struct a_star *a, uint32_t index_path, uint32_t fromx, uint32_t fromy, uint32_t tilex, uint32_t tiley);
void a_star_clear(struct a_star *a, uint32_t width, uint32_t height, struct floor ***floor, int clearx, int cleary);

#endif