#include <stdint.h>
#include <GLES3/gl3.h>
#include <core/shaders.h>
#include <core/rect.h>
#include <core/cmath.h>

static uint32_t program;
static uint32_t ortho_loc;
static uint32_t pos_loc;
static uint32_t color_loc;

void
simple_color_2d_shader_init ()
{
	program = shaders_get (PROGRAM_2D_SIMPLE_COLOR);

	glUseProgram (program);

	ortho_loc = glGetUniformLocation (program, "ortho");
	pos_loc = glGetUniformLocation (program, "pos");
	color_loc = glGetUniformLocation (program, "s_color");
}

void
simple_color_2d_shader_render (struct rect *rt)
{
	glUseProgram (program);

	glUniformMatrix4fv (ortho_loc, 1, GL_FALSE, rt->ortho);
	glUniformMatrix4fv (pos_loc, 1, GL_FALSE, rt->pos);
	glUniform4fv (color_loc, 1, rt->color);

	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, rt->v_data);

	glDrawArrays (GL_TRIANGLES, 0, 6);
}