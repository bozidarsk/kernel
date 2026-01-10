#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

typedef enum 
{
	FONT_NAME_W8H16,
} FontName;

const uint8_t* fonts_get_bitmap(FontName name);

#endif
