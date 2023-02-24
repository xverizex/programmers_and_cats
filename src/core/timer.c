#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <core/timer.h>

struct times *
timer_init (const uint32_t size)
{
	struct times *t = malloc (sizeof (struct times) * size);
	for (int i = 0; i < size; i++) {
		t[i].cur = 0L;
		t[i].turn = 0L;
	}
	return t;
}

uint32_t
timer_run (struct times *t, const uint32_t index, const uint32_t milliseconds)
{
	struct timeval tv;
	gettimeofday (&tv, NULL);

	uint64_t ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	if (t[index].cur == 0L) {
		t[index].cur = ms;
		return 0;
	}

	uint64_t diff_time = ms - t[index].cur;

	t[index].turn += diff_time;
	if (t[index].turn >= milliseconds) {
		t[index].turn = 0L;
		t[index].cur = ms;
		return 1;
	}

	t[index].cur = ms;
	return 0;
}

void
timer_clear (struct times *t)
{
	free (t);
}

void 
timer_stop (struct times *t, const uint32_t index)
{
	t[index].cur = 0L;
	t[index].turn = 0L;
}