#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <GLES3/gl3.h>
#include <common/file.h>
#include <shaders/list.h>


uint32_t programs[N_PROGRAMS];

static uint32_t
init_shader (int type, const char *filename)
{
	uint32_t shader = glCreateShader (type);

	uint8_t *data = file_get_data (filename);

	glShaderSource (shader, 1, (const char *const *) &data, NULL);

	free (data);

	glCompileShader (shader);

	int value;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &value);

	if (value == GL_FALSE) {
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &value);
		char *info = malloc (value + 1);
		glGetShaderInfoLog (shader, value, &value, info);
		info[value] = 0;
		fprintf (stderr, "compile shader: [%s]\n", info);
		free (info);
		exit (-1);
	}

	return shader;
}

static uint32_t
init_program (const char *program_str)
{
	char v_shader_str[255];
	char f_shader_str[255];
	snprintf (v_shader_str, 255, "%s.vert", program_str);
	snprintf (f_shader_str, 255, "%s.frag", program_str);

	uint32_t v_shader = init_shader (GL_VERTEX_SHADER, v_shader_str);
	uint32_t f_shader = init_shader (GL_FRAGMENT_SHADER, f_shader_str);

	uint32_t program = glCreateProgram ();

	glAttachShader (program, v_shader);
	glAttachShader (program, f_shader);
	glLinkProgram (program);

	int value;
	glGetProgramiv (program, GL_LINK_STATUS, &value);
	if (value == GL_FALSE) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &value);
		char *info = malloc (value + 1);
		glGetProgramInfoLog (program, value, &value, info);
		info[value] = 0;
		fprintf (stderr, "link program: [%s]\n", info);
		free (info);
		exit (-1);
	}

	return program;
}

void
shaders_init ()
{
	/* init shaders */
	programs[PROGRAM_2D_SIMPLE] = init_program ("assets/program_2d_simple");
	programs[PROGRAM_2D_SIMPLE_COLOR] = init_program ("assets/program_2d_simple_color");
	programs[PROGRAM_TEXT] = init_program ("assets/shader_text_simple");

	/* init uniforms */
	simple_2d_shader_init ();
	simple_color_2d_shader_init ();
}

uint32_t shaders_get (int index)
{
	return programs[index];
}
