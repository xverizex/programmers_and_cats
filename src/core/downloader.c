#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <core/downloader.h>
#include <textures/list.h>
#include <common/file.h>
#include <common/bites.h>

static void
set_texture_data (struct texture *t, uint8_t *data)
{
	uint32_t *ptr = (uint32_t *) data;

	uint32_t head   = *ptr++;
	uint32_t frames = *ptr++;
	uint32_t width  = *ptr++;
	uint32_t height = *ptr++;

	if (head != 1) {
		width  = bites_swap_little_big_engian_uint (width);
		height = bites_swap_little_big_engian_uint (height);
		frames = bites_swap_little_big_engian_uint (frames);
	}

	t->width_orig = width;
	t->height_orig = height;
	t->total_frames = frames;

	uint8_t *d = (uint8_t *) ptr;

	int total = width * height * 4;

	t->frame = malloc (frames * sizeof (uint32_t));

	glGenTextures (t->total_frames, t->frame);

	for (int i = 0; i < frames; i++) {
		glBindTexture (GL_TEXTURE_2D, t->frame[i]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, t->width_orig, t->height_orig, 0, GL_RGBA, GL_UNSIGNED_BYTE, d);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture (GL_TEXTURE_2D, 0);

		d += total;
	}
}

#define TYPE_INT       0
#define TYPE_FLOAT     1

static void
set_texture_vao (struct texture *t, const int type)
{
	float aspect;
	float w;
	float h;

	if (type == TYPE_FLOAT) {
		if (t->width_orig >= t->height_orig) {
			aspect = ((float) t->width_orig) / ((float) t->height_orig);
			w = 1.0f;
			h = w / aspect;
		}
		else {
			aspect = ((float) t->height_orig) / ((float) t->width_orig);
			w = 1.0;
			h = w * aspect;
		}
	} else {
		w = t->width_orig;
		h = t->height_orig;
		aspect = 1.f;
	}
	
	
	float ww = w / 2.f;
	float hh = h / 2.f;


	float v[18] = {
		-ww, -hh, 0.0f,
		-ww, hh, 0.0f,
		ww, -hh, 0.0f,
		ww, -hh, 0.0f,
		ww, hh, 0.0f,
		-ww, hh, 0.0f
	};

	memcpy (t->v_data, v, 18 * sizeof (float));

	t->w = w;
	t->h = h;
	t->aspect = aspect;
}

static void
set_texture_coord (struct texture *t)
{
#if 1
	static float tex[12] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
#else
	static float tex[12] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
#endif

	memcpy (t->t_data, tex, 12 * sizeof (float));
}

static struct texture *
load_texture (const char *filename)
{
	struct texture *t = malloc (sizeof (struct texture));

	uint8_t *data = file_get_data (filename);
	set_texture_data (t, data);
	set_texture_coord (t);
	set_texture_vao (t, TYPE_INT);

	return t;
}

static struct texture *texture[N_TEXTURES];

struct texture *
downloader_load_texture (const uint32_t index)
{
	if (index >= N_TEXTURES)
		return NULL;

	switch (index) {
		case TEXTURE_LOGO:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_logo.tex");
			}
			break;
		case TEXTURE_FLOOR:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_floor.tex");
			}
			break;
		case TEXTURE_BOARD:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_board.tex");
			}
			break;
		case TEXTURE_WALL:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_wall.tex");
			}
			break;
		case TEXTURE_MAN_1:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_man1.tex");
			}
			break;
		case TEXTURE_FIRED:
			if (texture[index] == NULL) {
				texture[index] = load_texture ("assets/texture_fired.tex");
			}
			break;
	}

	return texture[index];
}
