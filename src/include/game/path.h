#ifndef OFFICE_PATH_HEADER_H
#define OFFICE_PATH_HEADER_H
#include <stdint.h>

struct coord {
	uint32_t x;
	uint32_t y;
};

struct path {
	struct coord *coord;
	uint32_t size_coord;
};

void path_init(const int size);
struct path *path_get(int index);
void path_add(int index, uint32_t x, uint32_t y);
void path_clear(int index);

#endif