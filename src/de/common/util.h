#ifndef __DE_UTIL_H__
#define __DE_UTIL_H__

#include <stddef.h> // size_t

#ifndef QUOTE
#	define DE_INNER_QUOTE(...) #__VA_ARGS__
#	define QUOTE(...) DE_INNER_QUOTE(__VA_ARGS__)
#endif

#define DSWAP(a,b) dswap(&(a),&(b),sizeof(a))

void dswap(void*a, void*b, size_t size);

void ddelay(int milliseconds);

#endif
