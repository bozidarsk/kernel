#include "stdlib.h"

static const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const unsigned int baseMax = (sizeof(chars) / sizeof(char)) - 1;

int atoi(const char* value) 
{
	if (!value)
		return 0;

	bool isNegative = value[0] == '-';
	int result = 0;

	for (int i = (isNegative || value[0] == '+') ? 1 : 0; value[i]; i++) 
	{
		if (value[i] < '0' || value[i] > '9')
			return 0;

		result *= 10;
		result += (int)value[i] - 0x30;
	}

	return isNegative ? -result : result;
}

long atol(const char* value) 
{
	if (!value)
		return 0;

	bool isNegative = value[0] == '-';
	long result = 0;

	for (int i = (isNegative || value[0] == '+') ? 1 : 0; value[i]; i++) 
	{
		if (value[i] < '0' || value[i] > '9')
			return 0;

		result *= 10;
		result += (long)value[i] - 0x30l;
	}

	return isNegative ? -result : result;
}

long long atoll(const char* value) 
{
	if (!value)
		return 0;

	bool isNegative = value[0] == '-';
	long long result = 0;

	for (int i = (isNegative || value[0] == '+') ? 1 : 0; value[i]; i++) 
	{
		if (value[i] < '0' || value[i] > '9')
			return 0;

		result *= 10;
		result += (long long)value[i] - 0x30ll;
	}

	return isNegative ? -result : result;
}

size_t tostring_string(char* buffer, size_t n, const char* value) 
{
	if (!buffer || n == 0)
		return 0;

	if (!value)
		return 0;

	size_t i = 0;

	for (i = 0; value[i]; i++) 
	{
		if (i + 1 >= n)
			break;

		buffer[i] = value[i];
	}

	buffer[i] = 0;
	return i + 1;
}

size_t tostring_char(char* buffer, size_t n, char value) 
{
	char string[] = { value, 0 };
	return tostring_string(buffer, n, string);
}

size_t tostring_bool(char* buffer, size_t n, bool value) 
{
	char* string = value ? "True" : "False";
	return tostring_string(buffer, n, string);
}

size_t tostring_int8(char* buffer, size_t n, int8_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	int8_t x = (value < 0) ? -value : value;

	do 
	{
		string--;
		*string = chars[x % (int8_t)base];
		x /= (int8_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	if (value < 0) 
	{
		string--;
		*string = '-';
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_int16(char* buffer, size_t n, int16_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	int16_t x = (value < 0) ? -value : value;

	do 
	{
		string--;
		*string = chars[x % (int16_t)base];
		x /= (int16_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	if (value < 0) 
	{
		string--;
		*string = '-';
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_int32(char* buffer, size_t n, int32_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	int32_t x = (value < 0) ? -value : value;

	do 
	{
		string--;
		*string = chars[x % (int32_t)base];
		x /= (int32_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	if (value < 0) 
	{
		string--;
		*string = '-';
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_int64(char* buffer, size_t n, int64_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	int64_t x = (value < 0) ? -value : value;

	do 
	{
		string--;
		*string = chars[x % (int64_t)base];
		x /= (int64_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	if (value < 0) 
	{
		string--;
		*string = '-';
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_uint8(char* buffer, size_t n, uint8_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	uint8_t x = value;

	do 
	{
		string--;
		*string = chars[x % (uint8_t)base];
		x /= (uint8_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_uint16(char* buffer, size_t n, uint16_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	uint16_t x = value;

	do 
	{
		string--;
		*string = chars[x % (uint16_t)base];
		x /= (uint16_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_uint32(char* buffer, size_t n, uint32_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	uint32_t x = value;

	do 
	{
		string--;
		*string = chars[x % (uint32_t)base];
		x /= (uint32_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_uint64(char* buffer, size_t n, uint64_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	uint64_t x = value;

	do 
	{
		string--;
		*string = chars[x % (uint64_t)base];
		x /= (uint64_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_nuint(char* buffer, size_t n, size_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	size_t x = value;

	do 
	{
		string--;
		*string = chars[x % (size_t)base];
		x /= (size_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_nint(char* buffer, size_t n, ssize_t value, unsigned int base, int precision) 
{
	if (base == 0 || base > baseMax)
		return 0;

	char array[256 + ((precision > 0) ? precision : 0)];
	char* string = array;
	int length = 0;

	string += (sizeof(array) / sizeof(char)) - 1;
	*string = '\0';

	ssize_t x = (value < 0) ? -value : value;

	do 
	{
		string--;
		*string = chars[x % (ssize_t)base];
		x /= (ssize_t)base;
		length++;
	} while (x != 0);

	while (length < precision) 
	{
		string--;
		*string = '0';
		length++;
	}

	if (value < 0) 
	{
		string--;
		*string = '-';
	}

	return tostring_string(buffer, n, string);
}

size_t tostring_double(char* buffer, size_t n, double x, int precision) 
{
	if (!buffer || n == 0)
		return 0;

	if (precision == -1)
		precision = 6;

	size_t offset = 0, total = 0;

	uint64_t memory = *(uint64_t*)&x;
	int64_t exponent = (memory >> 52) & ((1l << 11) - 1);
	int sign = (memory >> 63) & 1;

	if (sign) 
	{
		offset = tostring_string(buffer, n, "-");
		if (offset == 0) return total;
		offset--;
		total += offset;
		buffer += offset;
		n -= offset;
	}

	if (x != x) 
	{
		offset = tostring_string(buffer, n, "nan");
		if (offset == 0) return total;
		offset--;
		total += offset;
		return total;
	}

	if (exponent == 0x7ff) 
	{
		offset = tostring_string(buffer, n, "inf");
		if (offset == 0) return total;
		offset--;
		total += offset;
		return total;
	}

	x = fabs(x);

	offset = tostring_uint64(buffer, n, (uint64_t)floor(x), 10, -1);
	if (offset == 0) return total;
	offset--;
	total += offset;
	if (n < offset) return total;
	buffer += offset;
	n -= offset;

	offset = tostring_string(buffer, n, ".");
	if (offset == 0) return total;
	offset--;
	total += offset;
	if (n < offset) return total;
	buffer += offset;
	n -= offset;

	x = x - floor(x);
	int d, p = 0;

	do 
	{
		x *= 10.0;
		d = (uint64_t)x % 10;

		offset = tostring_uint64(buffer, n, d, 10, -1);
		if (offset == 0) return total;
		offset--;
		total += offset;
		buffer += offset;
		n -= offset;
	}
	while (p++ < precision);

	return total;
}

size_t tostring_float(char* buffer, size_t n, float value, int precision) 
{
	return tostring_double(buffer, n, (double)value, precision);
}
