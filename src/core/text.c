#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdint.h>
#include <wchar.h>
#include <GLES3/gl3.h>
#include <core/shaders.h>
#include <core/text.h>
#include <core/cmath.h>

extern uint32_t screen_width;
extern uint32_t screen_height;

static FT_Library ft;
static FT_Face face;
static uint32_t program;
static uint32_t uniform_projection;
static uint32_t uniform_color;
static uint32_t uniform_model;
static uint32_t uniform_tex;
static uint32_t uniform_translate;

static uint32_t vao;
static uint32_t vbo;

static float projection[16];
static float model[16];
static float translate[16];

static float w = 128.f;
static float h = 128.f;

void init_text ()
{

	//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	FT_Init_FreeType (&ft);
	int ret = FT_New_Face (ft, "assets/Hack-Regular.ttf", 0, &face);

	FT_Set_Pixel_Sizes (face, 0, 48);

	program = shaders_get (PROGRAM_TEXT);

	glUseProgram (program);

	uniform_projection = glGetUniformLocation (program, "projection");
	uniform_color = glGetUniformLocation (program, "textColor");
	uniform_tex = glGetUniformLocation (program, "stext");

	//printf ("error: %d\n", glGetError ());

	
}

struct text *text_build (wchar_t *str, int pixel_size)
{
	struct text *text = malloc (sizeof (struct text));
	text->chars = NULL;
	text->size = 0;

	int len = wcslen (str);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 0; i < len; i++) {
		int ret = FT_Load_Char (face, str[i], FT_LOAD_RENDER);

		uint32_t tex;
		glGenTextures (1, &tex);
		glBindTexture (GL_TEXTURE_2D, tex);

		glTexImage2D (
				GL_TEXTURE_2D,
				0,
				GL_R8,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			     );

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture (GL_TEXTURE_2D, 0);

		struct character *c = malloc (sizeof (struct character));
		c->tex = tex;
		c->bw = face->glyph->bitmap.width;
		c->br = face->glyph->bitmap.rows;
		c->bl = face->glyph->bitmap_left;
		c->bt = face->glyph->bitmap_top;
		c->advance = face->glyph->advance.x;

		int index = text->size++;
		text->chars = realloc (text->chars, text->size * sizeof(struct character));
		text->chars[index] = c;
	}

	return text;
}

void text_render (struct text *text, float x, float y, float scale, float red, float green, float blue)
{
	math_ortho (projection, 0.f, screen_width, 0.f, screen_height, 0.f, 100.f);

	glUseProgram (program);

	glActiveTexture(GL_TEXTURE0);

	glUniform3f (uniform_color, red, green, blue);	
	glUniformMatrix4fv (uniform_projection, 1, GL_FALSE, projection);


	for (int i = 0; i < text->size; i++) {
		struct character *c = text->chars[i];

		float xpos = x + c->bl * scale;
		float ypos = y - (c->br - c->bt) * scale;

		float w = c->bw * scale;
		float h = c->br * scale;

		float vertices[12] = {
			 xpos, ypos + h,
			 xpos, ypos,
			 xpos + w, ypos,
			 xpos, ypos + h ,
			 xpos + w, ypos ,
			xpos + w, ypos + h 
		};

#if 0
	float textures[12] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

#else
		static float textures[12] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};
#endif

		glBindTexture (GL_TEXTURE_2D, c->tex);
		glUniform1i (uniform_tex, 0);


		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, vertices);

		glEnableVertexAttribArray (1);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, textures);

		glDrawArrays (GL_TRIANGLES, 0, 6);

		x += (c->advance >> 6) * scale;

	}

	glBindTexture (GL_TEXTURE_2D, 0);
}

void text_render_center (struct text *text, float w, float h, float scale, float red, float green, float blue)
{
	math_ortho (projection, 0.f, w, 0.f, h, -1.1f, 100.f);

	glUseProgram (program);

	glBindVertexArray (vao);

	glActiveTexture(GL_TEXTURE0);

	glUniform3f (uniform_color, red, green, blue);	
	glUniformMatrix4fv (uniform_projection, 1, GL_FALSE, projection);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	float x = 0.f;
	float y = h / 2.f;
	float tw = 0.f;
	float th = 0.f;

	for (int i = 0; i < text->size; i++) {
		struct character *c = text->chars[i];

		float w = c->bw * scale;
		float h = c->br * scale;

		x += (c->advance >> 6) * scale;

		tw += w;
		th = h;
	}

	y = h / 2.f - th / 2.f;
	x = w / 2.f - tw / 2.f;


	for (int i = 0; i < text->size; i++) {
		struct character *c = text->chars[i];

		float xpos = x + c->bl * scale;
		float ypos = y - (c->br - c->bt) * scale;

		float w = c->bw * scale;
		float h = c->br * scale;

		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos, ypos, 0.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		};

		glBindTexture (GL_TEXTURE_2D, c->tex);

		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (vertices), vertices);
		glBindBuffer (GL_ARRAY_BUFFER, 0);


		x += (c->advance >> 6) * scale;
	}
			glDrawArrays (GL_TRIANGLES, 0, 6);


	glBindVertexArray (0);
	glBindTexture (GL_TEXTURE_2D, 0);
}