#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stdbool.h>

#include "eh.h"

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
void* console_get_framebuffer();
void console_set_video_mode(VideoMode value);
VideoMode console_get_video_mode();
void console_set_color_mode(ColorMode value);
ColorMode console_get_color_mode();
void console_set_width(int value);
int console_get_width();
void console_set_height(int value);
int console_get_height();
void console_set_char_width(int value);
int console_get_char_width();
void console_set_char_height(int value);
int console_get_char_height();
void console_set_pitch(int value);
int console_get_pitch();
void console_set_depth(int value);
int console_get_depth();
void console_set_bitmap(const uint8_t* value);
const uint8_t* console_get_bitmap();
void console_set_bgcolor(ConsoleColor value);
ConsoleColor console_get_bgcolor();
void console_set_fgcolor(ConsoleColor value);
ConsoleColor console_get_fgcolor();

void console_scroll(int lines);
void console_clear();

void console_write_string(const char* value);
void console_write_char(char value);
void console_write_bool(bool value);
void console_write_int8(int8_t value);
void console_write_int16(int16_t value);
void console_write_int32(int32_t value);
void console_write_int64(int64_t value);
void console_write_uint8(uint8_t value);
void console_write_uint16(uint16_t value);
void console_write_uint32(uint32_t value);
void console_write_uint64(uint64_t value);
void console_write_pointer(void* value);

/*
#define console_write(value) \
	({ \
		if (false); \
		else if (__builtin_types_compatible_p(typeof(value), const char[])) console_write_string((char*)value); \
		else if (__builtin_types_compatible_p(typeof(value), char[])) console_write_string((char*)value); \
		else if (__builtin_types_compatible_p(typeof(value), const char*)) console_write_string((char*)value); \
		else if (__builtin_types_compatible_p(typeof(value), char*)) console_write_string((char*)value); \
		else if (__builtin_types_compatible_p(typeof(value), char)) console_write_char((char)value); \
		else if (__builtin_types_compatible_p(typeof(value), void*)) console_write_pointer((void*)value); \
		else if (__builtin_types_compatible_p(typeof(value), bool)) console_write_bool((bool)value); \
		else if (__builtin_types_compatible_p(typeof(value), int8_t)) console_write_int8((int8_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), int16_t)) console_write_int16((int16_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), int32_t)) console_write_int32((int32_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), int64_t)) console_write_int64((int64_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), uint8_t)) console_write_uint8((uint8_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), uint16_t)) console_write_uint16((uint16_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), uint32_t)) console_write_uint32((uint32_t)value); \
		else if (__builtin_types_compatible_p(typeof(value), uint64_t)) console_write_uint64((uint64_t)value); \
		else throw(ArgumentException, "Unsupported argument type."); \
	})
*/

#define console_write(value) \
	({ \
		_Pragma("GCC diagnostic push"); \
		_Pragma("GCC diagnostic ignored \"-Wpointer-to-int-cast\""); \
		_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\""); \
		_Pragma("GCC diagnostic ignored \"-Waddress\""); \
		_Generic(value, \
			const char*: console_write_string((char*)value), \
			char*: console_write_string((char*)value), \
			char: console_write_char((char)value), \
			void*: console_write_pointer((void*)value), \
			bool: console_write_bool((bool)value), \
			int8_t: console_write_int8((int8_t)value), \
			int16_t: console_write_int16((int16_t)value), \
			int32_t: console_write_int32((int32_t)value), \
			int64_t: console_write_int64((int64_t)value), \
			uint8_t: console_write_uint8((uint8_t)value), \
			uint16_t: console_write_uint16((uint16_t)value), \
			uint32_t: console_write_uint32((uint32_t)value), \
			uint64_t: console_write_uint64((uint64_t)value), \
			default: throw(ArgumentException, "Unsupported argument type.") \
		); \
		_Pragma("GCC diagnostic pop"); \
	})

#define console_writeline(value) ({ console_write(value); console_write_char((char)'\n'); })

#endif
