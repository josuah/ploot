#include <string.h>

#include "def.h"

size_t
font_width(struct font *ft, int c)
{
	return strlen(ft->glyph[c]) / ft->height;
}

size_t
font_strlen(struct font *ft, char *s)
{
	size_t		len;

	len = 0;
	for (; *s != '\0'; s++)
		len += font_width(ft, *s);
	return len;
}
