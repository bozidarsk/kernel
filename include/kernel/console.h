#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#include "kernel/eh.h"

typedef enum VideMode 
{
	CONSOLE_VIDEO_MODE_SERIAL,
	CONSOLE_VIDEO_MODE_VGA_TEXT,
	CONSOLE_VIDEO_MODE_VGA_GRAPHICS,
} VideoMode;

typedef enum 
{
	CONSOLE_COLOR_MODE_B4F4,
	CONSOLE_COLOR_MODE_R5G6B5,
	CONSOLE_COLOR_MODE_RGB565 = CONSOLE_COLOR_MODE_R5G6B5,
	CONSOLE_COLOR_MODE_R8G8B8,
	CONSOLE_COLOR_MODE_RGB = CONSOLE_COLOR_MODE_R8G8B8,
} ColorMode;

typedef enum : uint8_t
{
	CONSOLE_COLOR_BLACK = 0x0,
	CONSOLE_COLOR_DARKBLUE = 0x1,
	CONSOLE_COLOR_DARKGREEN = 0x2,
	CONSOLE_COLOR_DARKCYAN = 0x3,
	CONSOLE_COLOR_DARKRED = 0x4,
	CONSOLE_COLOR_DARKMAGENTA = 0x5,
	CONSOLE_COLOR_DARKYELLOW = 0x6,
	CONSOLE_COLOR_GRAY = 0x7,
	CONSOLE_COLOR_DARKGRAY = 0x8,
	CONSOLE_COLOR_BLUE = 0x9,
	CONSOLE_COLOR_GREEN = 0xa,
	CONSOLE_COLOR_CYAN = 0xb,
	CONSOLE_COLOR_RED = 0xc,
	CONSOLE_COLOR_MAGENTA = 0xd,
	CONSOLE_COLOR_YELLOW = 0xe,
	CONSOLE_COLOR_WHITE = 0xf,
} ConsoleColor;

void console_set_framebuffer(void* value);
void* console_get_framebuffer(void);
void console_set_video_mode(VideoMode value);
VideoMode console_get_video_mode(void);
void console_set_color_mode(ColorMode value);
ColorMode console_get_color_mode(void);
void console_set_width(int value);
int console_get_width(void);
void console_set_height(int value);
int console_get_height(void);
void console_set_char_width(int value);
int console_get_char_width(void);
void console_set_char_height(int value);
int console_get_char_height(void);
void console_set_pitch(int value);
int console_get_pitch(void);
void console_set_depth(int value);
int console_get_depth(void);
void console_set_bitmap(const uint8_t* value);
const uint8_t* console_get_bitmap(void);
void console_set_bgcolor(ConsoleColor value);
ConsoleColor console_get_bgcolor(void);
void console_set_fgcolor(ConsoleColor value);
ConsoleColor console_get_fgcolor(void);

void console_scroll(int lines);
void console_clear(void);

#endif
