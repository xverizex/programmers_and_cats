#include <SDL2/SDL.h>
#include <common/file.h>

uint8_t *
file_get_data (const char *filename)
{
	SDL_RWops *io = SDL_RWFromFile (filename, "rb");
	if (io != NULL) {
		uint64_t size = io->size (io);
		uint8_t *data = malloc (size + 1);
		io->read (io, data, size, 1);
		io->close (io);
		data[size] = 0;
		return data;
	}

	return NULL;
}
