#include <stdbool.h>

#include "stdlib.h"

#include "console.h"
#include "fonts.h"
#include "ioport.h"
#include "eh.h"

typedef void(*CharPutMethod)(char c, ConsoleColor color);

static int x = 0;
static int y = 0;

static void* framebuffer;
static VideoMode videoMode = CONSOLE_VIDEO_MODE_SERIAL;
static ColorMode colorMode;
static int width, height, charWidth, charHeight, pitch, depth;
static const uint8_t* bitmap;
static uint8_t consoleColor = 0xf;

void console_set_framebuffer(void* value) { framebuffer = value; }
void* console_get_framebuffer() { return framebuffer; }

void console_set_video_mode(VideoMode value) { videoMode = value; }
VideoMode console_get_video_mode() { return videoMode; }

void console_set_color_mode(ColorMode value) { colorMode = value; }
ColorMode console_get_color_mode() { return colorMode; }

void console_set_width(int value) { width = value; }
int console_get_width() { return width; }

void console_set_height(int value) { height = value; }
int console_get_height() { return height; }

void console_set_char_width(int value) { charWidth = value; }
int console_get_char_width() { return charWidth; }

void console_set_char_height(int value) { charHeight = value; }
int console_get_char_height() { return charHeight; }

void console_set_pitch(int value) { pitch = value; }
int console_get_pitch() { return pitch; }

void console_set_depth(int value) { depth = value; }
int console_get_depth() { return depth; }

void console_set_bitmap(const uint8_t* value) { bitmap = value; }
const uint8_t* console_get_bitmap() { return bitmap; }

void console_set_bgcolor(ConsoleColor value) { consoleColor = ((value & 0xf) << 4) | console_get_fgcolor(); }
ConsoleColor console_get_bgcolor() { return (consoleColor >> 4) & 0xf; }

void console_set_fgcolor(ConsoleColor value) { consoleColor = (console_get_bgcolor() << 4) | (value & 0xf); }
ConsoleColor console_get_fgcolor() { return consoleColor & 0xf; }

static void put__VGA_TEXT__B4F4(char c, ConsoleColor color) 
{
	((uint16_t*)framebuffer)[y * width + x] = (((uint16_t)color << 8) | ((uint16_t)c & 0xff));
}

static void put__VGA_GRAPHICS__R5G6B5(char c, ConsoleColor color) 
{
	int pixelw = width * charWidth;

	const uint8_t* map = bitmap + (((charWidth * charHeight) / 8) * (int)(uint32_t)c);

	uint16_t colors[16] = { 0x1084, 0x1230, 0x250c, 0x2cf5, 0xb8c4, 0x9a36, 0x9b89, 0xce78, 0x5acc, 0x2bda, 0x366e, 0x363a, 0xeae9, 0xbaf8, 0xe541, 0xffff };
	uint16_t bgfg[2] = { colors[(color >> 4) & 0xf], colors[(color >> 0) & 0xf] };

	int i = 0;
	for (int dy = 0; dy < charHeight; dy++) 
	{
		for (int dx = 0; dx < charWidth; dx++) 
		{
			((uint16_t*)framebuffer)[(y * charHeight * pixelw) + (dy * pixelw) + (x * charWidth) + dx] = bgfg[(map[i / charWidth] >> (i % charWidth)) & 1]; // charWidth must be == sizeof(uint8_t) * 8
			i++;
		}
	}
}

static void put__VGA_GRAPHICS__R8G8B8(char c, ConsoleColor color) 
{
	const uint8_t* map = bitmap + (((charWidth * charHeight) / 8) * (int)(uint32_t)c);

	uint32_t colors[16] = { 0x000000, 0x00008b, 0x008b00, 0x008b8b, 0x8b0000, 0x8b008b, 0x8b8b00, 0x8b8b8b, 0x5a5a5a, 0x0000ff, 0x00ff00, 0x00ffff, 0xff0000, 0xff00ff, 0xffff00, 0xffffff };
	uint32_t bgfg[2] = { colors[(color >> 4) & 0xf], colors[(color >> 0) & 0xf] };

	int i = 0;
	for (int dy = 0; dy < charHeight; dy++) 
	{
		for (int dx = 0; dx < charWidth; dx++) 
		{
			int bpp = depth / 8; // bytes per pixel not bits

			uint8_t* address = framebuffer + ((y * charHeight) * pitch) + ((x * charWidth) * bpp) + (dy * pitch) + (dx * bpp);

			// charWidth must be == sizeof(uint8_t) * 8
			uint32_t cc = bgfg[(map[i / charWidth] >> (i % charWidth)) & 1];

			// buffer is in BGR format ???
			((uint8_t*)address)[0] = (uint8_t)((cc >> 0) & 0xff);
			((uint8_t*)address)[1] = (uint8_t)((cc >> 8) & 0xff);
			((uint8_t*)address)[2] = (uint8_t)((cc >> 16) & 0xff);

			i++;
		}
	}
}

static void put__SERIAL__(char c, ConsoleColor color) 
{
	(void)color; // -Wunused-parameter

	while ((inb(0x3f8 + 5) & 0x20) == 0);
	outb(0x3f8, (uint8_t)c);
}

static CharPutMethod getCharPutMethod() 
{
	switch (videoMode) 
	{
		case CONSOLE_VIDEO_MODE_VGA_TEXT:
			switch (colorMode) 
			{
				case CONSOLE_COLOR_MODE_B4F4:
					return &put__VGA_TEXT__B4F4;
				default:
					break;
			}
			break;
		case CONSOLE_VIDEO_MODE_VGA_GRAPHICS:
			switch (colorMode) 
			{
				case CONSOLE_COLOR_MODE_R5G6B5:
					return &put__VGA_GRAPHICS__R5G6B5;
				case CONSOLE_COLOR_MODE_R8G8B8:
					return &put__VGA_GRAPHICS__R8G8B8;
				default:
					break;
			}
			break;
		case CONSOLE_VIDEO_MODE_SERIAL:
			return &put__SERIAL__;
		default:
			break;
	}

	throw(NotImplementedException, "Cannot putc with current video and color modes.");
}

void console_scroll(int lines) 
{
	if (videoMode == CONSOLE_VIDEO_MODE_SERIAL)
		return;

	if (lines == 0)
		return;
	else if (lines > 0) 
	{
		throw(NotImplementedException, "Cannot scroll up.");
	}
	else if (lines < 0) 
	{
		int pitchb = pitch * charHeight;
		lines = -lines;

		memcpy(framebuffer, framebuffer + (pitchb * lines), (size_t)(pitchb * (height - lines)));
		memset(framebuffer + (pitchb * (height - lines)), 0, (size_t)(pitchb * lines));
	}
}

void console_clear() 
{
	if (videoMode == CONSOLE_VIDEO_MODE_SERIAL)
		return;

	CharPutMethod put = getCharPutMethod();

	if (!put)
		return;

	int max = width * height;

	x = 0;
	y = 0;

	for (int i = 0; i < max; i++) 
	{
		if (x >= width) 
		{
			x = 0;
			y++;
		}

		put(' ', 0);
		x++;
	}

	x = 0;
	y = 0;
}

static void writeChars(const char* chars, ConsoleColor color) 
{
	if (!chars)
		return;

	CharPutMethod put = getCharPutMethod();

	if (!put)
		return;

	for (int i = 0; chars[i] != '\0'; i++) 
	{
		if (x >= width) 
		{
			x = 0;
			y++;
		}

		if (videoMode != CONSOLE_VIDEO_MODE_SERIAL && y >= height) 
		{
			console_scroll(-1);
			y--;
		}

		switch (chars[i]) 
		{
			case '\r':
				x = 0;
				break;
			case '\n':
				if (videoMode == CONSOLE_VIDEO_MODE_SERIAL)
					put(chars[i], color);

				x = 0;
				y++;
				break;
			default:
				put(chars[i], color);
				x++;
				break;
		}
	}
}

ssize_t write(int fd, const void* buffer, size_t size) 
{
	(void)fd;

	if (!buffer)
		return -1;

	char chars[size + 1];

	memcpy(chars, buffer, size);
	chars[size] = 0;

	writeChars(chars, consoleColor);

	return size;
}
