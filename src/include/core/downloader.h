#ifndef OFFICE_DOWNLOADER_HEADER_H
#define OFFICE_DOWNLOADER_HEADER_H
#include <stdint.h>

struct texture {
	uint32_t vao;
	uint32_t vbo;
	uint32_t *tex;
	uint32_t total_frames;
	uint32_t width_orig;
	uint32_t height_orig;
	uint32_t *frame;
	float aspect;
	float w;
	float h;
	float t_data[12];
	float v_data[18];
};

#define to_str(val, ext) "assets/"#val#ext

struct texture *downloader_load_texture(const uint32_t index);

#endif
