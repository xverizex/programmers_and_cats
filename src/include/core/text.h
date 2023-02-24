#ifndef OFFICE_TEXT_HEADER_H
#define OFFICE_TEXT_HEADER_H
#include <wchar.h>

struct character {
	uint32_t tex;
	uint32_t bw;
	uint32_t br;
	uint32_t bl;
	uint32_t bt;
	uint32_t advance;
};

struct text {
	struct character **chars;
	uint32_t size;
};

void init_text ();
struct text *text_build (wchar_t *str, int pixel_size);
void text_render (struct text *text, float x, float y, float scale, float red, float green, float blue);
void text_render_center (struct text *text, float w, float h, float scale, float red, float green, float blue);

#endif