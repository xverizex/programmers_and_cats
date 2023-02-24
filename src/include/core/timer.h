#ifndef OFFICE_TIMER_H
#define OFFICE_TIMER_H

#include <stdint.h>

struct times {
	uint64_t cur;
	uint64_t turn;
};

struct times *timer_init(const uint32_t size);
uint32_t timer_run(struct times *t, const uint32_t index, const uint32_t milliseconds);
void timer_clear (struct times *t);
void timer_stop (struct times *t, const uint32_t index);
#endif
