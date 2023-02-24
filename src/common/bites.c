#include <common/bites.h>

uint32_t
bites_swap_little_big_engian_uint (uint32_t num)
{
	return 
		(
		 ((num >> 24) & 0xff) | 
		 ((num << 8) & 0xff0000) | 
		 ((num >> 8) & 0xff00) | 
		 ((num << 24) & 0xff000000)
		);
}
