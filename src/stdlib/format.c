#include "stdlib.h"

typedef void(*CharPutMethod)(char x, char* buffer, size_t offset, size_t size);

size_t tostring_string(char* buffer, size_t n, const char* value);
size_t tostring_char(char* buffer, size_t n, char value);
size_t tostring_bool(char* buffer, size_t n, bool value);
size_t tostring_int8(char* buffer, size_t n, int8_t value, unsigned int base, int precision);
size_t tostring_int16(char* buffer, size_t n, int16_t value, unsigned int base, int precision);
size_t tostring_int32(char* buffer, size_t n, int32_t value, unsigned int base, int precision);
size_t tostring_int64(char* buffer, size_t n, int64_t value, unsigned int base, int precision);
size_t tostring_uint8(char* buffer, size_t n, uint8_t value, unsigned int base, int precision);
size_t tostring_uint16(char* buffer, size_t n, uint16_t value, unsigned int base, int precision);
size_t tostring_uint32(char* buffer, size_t n, uint32_t value, unsigned int base, int precision);
size_t tostring_uint64(char* buffer, size_t n, uint64_t value, unsigned int base, int precision);
size_t tostring_nuint(char* buffer, size_t n, size_t value, unsigned int base, int precision);
size_t tostring_nint(char* buffer, size_t n, ssize_t value, unsigned int base, int precision);
size_t tostring_double(char* buffer, size_t n, double x, int precision);

static bool contains(const char* str, char x) 
{
	if (!str)
		return false;

	for (size_t i = 0; str[i]; i++)
		if (str[i] == x)
			return true;

	return false;
}

static void put_printf(char x, char* buffer, size_t offset, size_t size) 
{
	(void)buffer;
	(void)offset;
	(void)size;

	write(1, &x, 1);
}

static void put_sprintf(char x, char* buffer, size_t offset, size_t size) 
{
	(void)size;

	buffer[offset] = x;
}

static void put_snprintf(char x, char* buffer, size_t offset, size_t size) 
{
	buffer[offset] = (offset + 1 < (size / sizeof(char))) ? x : 0;
}

/*
%[argument$][flags][width][.precision][length modifier]conversion

flags:
#      The  value should be converted to an "alternate form".  For o conversions, the first character of the output string is made zero (by prefixing a 0 if it was not zero already).  For x and X conversions, a nonzero result has
      the string "0x" (or "0X" for X conversions) prepended to it.  For a, A, e, E, f, F, g, and G conversions, the result will always contain a decimal point, even if no digits follow it (normally, a decimal  point  appears  in
      the  results  of  those conversions only if a digit follows).  For g and G conversions, trailing zeros are not removed from the result as they would otherwise be.  For m, if errno contains a valid error code, the output of
      strerrorname_np(errno) is printed; otherwise, the value stored in errno is printed as a decimal number.  For other conversions, the result is undefined.
0      The value should be zero padded.  For d, i, o, u, x, X, a, A, e, E, f, F, g, and G conversions, the converted value is padded on the left with zeros rather than blanks.  If the 0 and - flags both appear, the 0 flag is  ig‐
      nored.  If a precision is given with an integer conversion (d, i, o, u, x, and X), the 0 flag is ignored.  For other conversions, the behavior is undefined.
-      The converted value is to be left adjusted on the field boundary.  (The default is right justification.)  The converted value is padded on the right with blanks, rather than on the left with blanks or zeros.  A - overrides
      a 0 if both are given.
' '    (a space) A blank should be left before a positive number (or empty string) produced by a signed conversion.
+      A sign (+ or -) should always be placed before a number produced by a signed conversion.  By default, a sign is used only for negative numbers.  A + overrides a space if both are used.
The five flag characters above are defined in the C99 standard.  POSIX specifies one further flag character.
'      For  decimal  conversion (i, d, u, f, F, g, G) the output is to be grouped with thousands' grouping characters as a non-monetary quantity.  Misleadingly, this isn't necessarily every thousand: for example Karbi ("mjw_IN"),
      groups its digits into 3 once, then 2 repeatedly.  Compare locale(7) grouping and thousands_sep, contrast with mon_grouping/mon_thousands_sep and strfmon(3).  This is a no-op in the default "C" locale.
glibc 2.2 adds one further flag character.
I      For decimal integer conversion (i, d, u) the output uses the locale's alternative output digits, if any.  For example, since glibc 2.2.3 this will give Arabic-Indic digits in the Persian ("fa_IR") locale.


length modifier:
hh     A following integer conversion corresponds to a signed char or unsigned char argument, or a following n conversion corresponds to a pointer to a signed char argument.
h      A following integer conversion corresponds to a short or unsigned short argument, or a following n conversion corresponds to a pointer to a short argument.
l      (ell)  A  following integer conversion corresponds to a long or unsigned long argument, or a following n conversion corresponds to a pointer to a long argument, or a following c conversion corresponds to a wint_t argument,
      or a following s conversion corresponds to a pointer to wchar_t argument.  On a following a, A, e, E, f, F, g, or G conversion, this length modifier is ignored (C99; not in SUSv2).
ll     (ell-ell).  A following integer conversion corresponds to a long long or unsigned long long argument, or a following n conversion corresponds to a pointer to a long long argument.
q      A synonym for ll.  This is a nonstandard extension, derived from BSD; avoid its use in new code.
L      A following a, A, e, E, f, F, g, or G conversion corresponds to a long double argument.  (C99 allows %LF, but SUSv2 does not.)
j      A following integer conversion corresponds to an intmax_t or uintmax_t argument, or a following n conversion corresponds to a pointer to an intmax_t argument.
z      A following integer conversion corresponds to a size_t or ssize_t argument, or a following n conversion corresponds to a pointer to a size_t argument.
Z      A nonstandard synonym for z that predates the appearance of z.  Do not use in new code.
t      A following integer conversion corresponds to a ptrdiff_t argument, or a following n conversion corresponds to a pointer to a ptrdiff_t argument.

conversion:
d, i   The int argument is converted to signed decimal notation.  The precision, if any, gives the minimum number of digits that must appear; if the converted value requires fewer digits, it is padded on the left with zeros.  The
              default precision is 1.  When 0 is printed with an explicit precision 0, the output is empty.
o, u, x, X
      The unsigned int argument is converted to unsigned octal (o), unsigned decimal (u), or unsigned hexadecimal (x and X) notation.  The letters abcdef are used for x conversions; the letters ABCDEF are used for X conversions.
      The  precision,  if  any,  gives the minimum number of digits that must appear; if the converted value requires fewer digits, it is padded on the left with zeros.  The default precision is 1.  When 0 is printed with an ex‐
      plicit precision 0, the output is empty.
e, E   The double argument is rounded and converted in the style [-]d.ddde±dd where there is one digit (which is nonzero if the argument is nonzero) before the decimal-point character and the number of digits after it is equal to
      the precision; if the precision is missing, it is taken as 6; if the precision is zero, no decimal-point character appears.  An E conversion uses the letter E (rather than e) to introduce the exponent.  The exponent always
      contains at least two digits; if the value is zero, the exponent is 00.
f, F   The double argument is rounded and converted to decimal notation in the style [-]ddd.ddd, where the number of digits after the decimal-point character is equal to the precision specification.  If the precision is  missing,
      it is taken as 6; if the precision is explicitly zero, no decimal-point character appears.  If a decimal point appears, at least one digit appears before it.
      (SUSv2 does not know about F and says that character string representations for infinity and NaN may be made available.  SUSv3 adds a specification for F.  The C99 standard specifies "[-]inf" or "[-]infinity" for infinity,
      and a string starting with "nan" for NaN, in the case of f conversion, and "[-]INF" or "[-]INFINITY" or "NAN" in the case of F conversion.)
g, G   The  double  argument is converted in style f or e (or F or E for G conversions).  The precision specifies the number of significant digits.  If the precision is missing, 6 digits are given; if the precision is zero, it is
      treated as 1.  Style e is used if the exponent from its conversion is less than -4 or greater than or equal to the precision.  Trailing zeros are removed from the fractional part of the result; a decimal point appears only
      if it is followed by at least one digit.
a, A   (C99; not in SUSv2, but added in SUSv3) For a conversion, the double argument is converted to hexadecimal notation (using the letters abcdef) in the style [-]0xh.hhhhp±d; for A conversion the prefix 0X, the letters ABCDEF,
      and the exponent separator P is used.  There is one hexadecimal digit before the radix point, and the number of digits after it is equal to the precision.  The default precision suffices for an exact representation of  the
      value if an exact representation in base 2 exists and otherwise is sufficiently large to distinguish values of type double.  The digit before the radix point is unspecified for nonnormalized numbers, and nonzero but other‐
      wise unspecified for normalized numbers.  The exponent, d, is the appropriate exponent of 2 expressed as a decimal integer; it always contains at least one digit; if the value is zero, the exponent is 0.
c      If  no  l  modifier  is present, the int argument is converted to an unsigned char, and the resulting character is written.  If an l modifier is present, the wint_t (wide character) argument is converted to a multibyte se‐
      quence by a call to the wcrtomb(3) function, with a conversion state starting in the initial state, and the resulting multibyte string is written.
s      If no l modifier is present: the const char * argument is expected to be a pointer to an array of character type (pointer to a string).  Characters from the array are written up to (but not including)  a  terminating  null
      byte  ('\0'); if a precision is specified, no more than the number specified are written.  If a precision is given, no null byte need be present; if the precision is not specified, or is greater than the size of the array,
      the array must contain a terminating null byte.
      If an l modifier is present: the const wchar_t * argument is expected to be a pointer to an array of wide characters.  Wide characters from the array are converted to multibyte characters (each by a call to the  wcrtomb(3)
      function,  with  a  conversion state starting in the initial state before the first wide character), up to and including a terminating null wide character.  The resulting multibyte characters are written up to (but not in‐
      cluding) the terminating null byte.  If a precision is specified, no more bytes than the number specified are written, but no partial multibyte characters are written.  Note that the  precision  determines  the  number  of
      bytes  written,  not  the number of wide characters or screen positions.  The array must contain a terminating null wide character, unless a precision is given and it is so small that the number of bytes written exceeds it
      before the end of the array is reached.
C      (Not in C99 or C11, but in SUSv2, SUSv3, and SUSv4.)  Synonym for lc.  Don't use.
S      (Not in C99 or C11, but in SUSv2, SUSv3, and SUSv4.)  Synonym for ls.  Don't use.
p      The void * pointer argument is printed in hexadecimal (as if by %#x or %#lx).
n      The number of characters written so far is stored into the integer pointed to by the corresponding argument.  That argument shall be an int *, or variant whose size matches the (optionally) supplied  integer  length  modi‐
      fier.  No argument is converted.  (This specifier is not supported by the bionic C library.)  The behavior is undefined if the conversion specification includes any flags, a field width, or a precision.
m      (glibc extension; supported by uClibc and musl, and on Android from API level 29.)  Print output of strerror(errno) (or strerrorname_np(errno) in the alternate form).  No argument is required.
%      A '%' is written.  No argument is converted.  The complete conversion specification is '%%'.
*/
static void put_common(CharPutMethod put, char* buffer, size_t size, const char* format, va_list args) 
{
	size_t offset = 0;

	ssize_t value__nint;
	size_t value__nuint;
	void* value__ptr;
	char value__char;
	int8_t value__int8_t;
	int16_t value__int16_t;
	int32_t value__int32_t;
	int64_t value__int64_t;
	uint8_t value__uint8_t;
	uint16_t value__uint16_t;
	uint32_t value__uint32_t;
	uint64_t value__uint64_t;
	double value__double;
	long double value__long_double;

	// %[argument$][flags][width][.precision][length modifier]conversion
	const char* flags = "#0- +'I";
	const char* numbers = "0123456789";
	const char* modifiers = "hlqLjzZt";

	for (size_t i = 0; format[i] != 0; i++) 
	{
		if (format[i] != '%') 
		{
			put(format[i], buffer, offset++, size);
			continue;
		}

		if (format[i + 1] == '%') 
		{
			put('%', buffer, offset++, size);
			i++;
			continue;
		}

		i++;

		// [argument$] - not implemented

		const char* flag = format + i;
		size_t flagLength = 0;

		while (contains(flags, format[i])) 
		{
			flagLength++;
			i++;
		}

		const char* width = format + i;
		size_t widthLength = 0;

		while (contains(numbers, format[i])) 
		{
			widthLength++;
			i++;
		}

		if (format[i] == '.') i++;

		const char* precision = format + i;
		size_t precisionLength = 0;

		while (contains(numbers, format[i])) 
		{
			precisionLength++;
			i++;
		}

		if (format[i] == '.' && precisionLength == 0) 
		{
			precision = "0";
			precisionLength = 1;
		}

		const char* modifier = format + i;
		size_t modifierLength = 0;

		while (contains(modifiers, format[i])) 
		{
			modifierLength++;
			i++;
		}

		const char* conversion = format + i++;

		char f[flagLength + 1];
		f[flagLength] = 0;
		memcpy(f, flag, flagLength * sizeof(char));

		char m[modifierLength + 1];
		m[modifierLength] = 0;
		memcpy(m, modifier, modifierLength * sizeof(char));

		int w = -1, p = -1;

		if (widthLength != 0) 
		{
			char widthBuffer[widthLength + 1];
			widthBuffer[widthLength] = 0;
			w = atoi((const char*)memcpy(widthBuffer, width, widthLength * sizeof(char)));
		}

		if (precisionLength != 0) 
		{
			char precisionBuffer[precisionLength + 1];
			precisionBuffer[precisionLength] = 0;
			p = atoi((const char*)memcpy(precisionBuffer, precision, precisionLength * sizeof(char)));
		}

		size_t n = 1024; // should be enough (even with precision-added zeroes)
		if (p != -1 && (size_t)p > n - 1) n = p + 1;
		char array[n];
		char* string = array;

		switch (*conversion) 
		{
			case 's':
				string = va_arg(args, char*);
				n = strlen(string) + 1;
				if (p != -1 && (size_t)p < n - 1) 
				{
					n = p;
					string[n] = 0;
				}
				break;
			case 'c':
				value__char = (char)va_arg(args, int);
				n = tostring_char(string, n, value__char);
				break;
			case 'p':
				value__ptr = va_arg(args, void*);
				string[0] = '0';
				string[1] = 'x';
				n = 2 + tostring_uint64(string + 2, n + 2, (uint64_t)value__ptr, 16, p);
				break;
			case 'x':
			case 'X':
				if (false);
				else if (!strcmp(m, "hh")) 
				{
					value__uint8_t = (uint8_t)va_arg(args, int);
					n = tostring_uint8(string, n, value__uint8_t, 16, p);
				}
				else if (!strcmp(m, "h")) 
				{
					value__uint16_t = (uint16_t)va_arg(args, int);
					n = tostring_uint16(string, n, value__uint16_t, 16, p);
				}
				else if (!strcmp(m, "l")) 
				{
					value__uint64_t = va_arg(args, unsigned long);
					n = tostring_uint64(string, n, value__uint64_t, 16, p);
				}
				else if (!strcmp(m, "ll")) 
				{
					value__uint64_t = va_arg(args, unsigned long long);
					n = tostring_uint64(string, n, value__uint64_t, 16, p);
				}
				else if (!strcmp(m, "z") || !strcmp(m, "Z")) 
				{
					value__nuint = va_arg(args, size_t);
					n = tostring_nuint(string, n, value__nuint, 16, p);
				}
				else 
				{
					value__uint32_t = va_arg(args, unsigned int);
					n = tostring_uint32(string, n, value__uint32_t, 16, p);
				}
				break;
			case 'o':
				if (false);
				else if (!strcmp(m, "hh")) 
				{
					value__uint8_t = (uint8_t)va_arg(args, int);
					n = tostring_uint8(string, n, value__uint8_t, 8, p);
				}
				else if (!strcmp(m, "h")) 
				{
					value__uint16_t = (uint16_t)va_arg(args, int);
					n = tostring_uint16(string, n, value__uint16_t, 8, p);
				}
				else if (!strcmp(m, "l")) 
				{
					value__uint64_t = va_arg(args, unsigned long);
					n = tostring_uint64(string, n, value__uint64_t, 8, p);
				}
				else if (!strcmp(m, "ll")) 
				{
					value__uint64_t = va_arg(args, unsigned long long);
					n = tostring_uint64(string, n, value__uint64_t, 8, p);
				}
				else if (!strcmp(m, "z") || !strcmp(m, "Z")) 
				{
					value__nuint = va_arg(args, size_t);
					n = tostring_nuint(string, n, value__nuint, 8, p);
				}
				else 
				{
					value__uint32_t = va_arg(args, unsigned int);
					n = tostring_uint32(string, n, value__uint32_t, 8, p);
				}
				break;
			case 'i':
			case 'd':
				if (false);
				else if (!strcmp(m, "hh")) 
				{
					value__int8_t = (int8_t)va_arg(args, int);
					n = tostring_int8(string, n, value__int8_t, 10, p);
				}
				else if (!strcmp(m, "h")) 
				{
					value__int16_t = (int16_t)va_arg(args, int);
					n = tostring_int16(string, n, value__int16_t, 10, p);
				}
				else if (!strcmp(m, "l")) 
				{
					value__int64_t = va_arg(args, long);
					n = tostring_int64(string, n, value__int64_t, 10, p);
				}
				else if (!strcmp(m, "ll")) 
				{
					value__int64_t = va_arg(args, long long);
					n = tostring_int64(string, n, value__int64_t, 10, p);
				}
				else if (!strcmp(m, "z") || !strcmp(m, "Z")) 
				{
					value__nint = va_arg(args, ssize_t);
					n = tostring_nint(string, n, value__nint, 10, p);
				}
				else 
				{
					value__int32_t = va_arg(args, int);
					n = tostring_int32(string, n, value__int32_t, 10, p);
				}
				break;
			case 'u':
				if (false);
				else if (!strcmp(m, "hh")) 
				{
					value__uint8_t = (uint8_t)va_arg(args, int);
					n = tostring_uint8(string, n, value__uint8_t, 10, p);
				}
				else if (!strcmp(m, "h")) 
				{
					value__uint16_t = (uint16_t)va_arg(args, int);
					n = tostring_uint16(string, n, value__uint16_t, 10, p);
				}
				else if (!strcmp(m, "l")) 
				{
					value__uint64_t = va_arg(args, unsigned long);
					n = tostring_uint64(string, n, value__uint64_t, 10, p);
				}
				else if (!strcmp(m, "ll")) 
				{
					value__uint64_t = va_arg(args, unsigned long long);
					n = tostring_uint64(string, n, value__uint64_t, 10, p);
				}
				else if (!strcmp(m, "z") || !strcmp(m, "Z")) 
				{
					value__nuint = va_arg(args, size_t);
					n = tostring_nuint(string, n, value__nuint, 10, p);
				}
				else 
				{
					value__uint32_t = va_arg(args, unsigned int);
					n = tostring_uint32(string, n, value__uint32_t, 10, p);
				}
				break;
			case 'e':
			case 'E':
			case 'g':
			case 'G':
			case 'f':
			case 'F':
				if (*m == 'L') 
				{
					value__long_double = va_arg(args, long double);
					n = tostring_double(string, n, (double)value__long_double, p);
				}
				else 
				{
					value__double = va_arg(args, double);
					n = tostring_double(string, n, value__double, p);
				}
				break;
			default:
				i--;
				continue;
		}

		char padding = (contains("diouxXaAeEfFgG", *conversion) && contains(f, '0')) ? '0' : ' ';

		if (!contains(f, '-')) 
		{
			if (contains("idfFeEgG", *conversion) && contains(f, ' ') && string[0] != '-')
				put(' ', buffer, offset++, size);

			if (contains("idfFeEgG", *conversion) && contains(f, '+') && string[0] != '-')
				put('+', buffer, offset++, size);

			if (w != -1)
				for (int t = (n == 0) ? 0 : n - 1; t < w; t++)
					put(padding, buffer, offset++, size);

			if (n != 0)
				for (size_t t = 0; t < n - 1; t++)
					put(string[t], buffer, offset++, size);
		}
		else 
		{
			if (contains("idfFeEgG", *conversion) && contains(f, ' ') && string[0] != '-')
				put(' ', buffer, offset++, size);

			if (contains("idfFeEgG", *conversion) && contains(f, '+') && string[0] != '-')
				put('+', buffer, offset++, size);

			if (n != 0)
				for (size_t t = 0; t < n - 1; t++)
					put(string[t], buffer, offset++, size);

			if (w != -1)
				for (int t = (n == 0) ? 0 : n - 1; t < w; t++)
					put(padding, buffer, offset++, size);
		}

		i--;
	}
}

void printf(const char* format, ...) 
{
	va_list args;
	va_start(args, format);
	put_common(&put_printf, NULL, 0, format, args);
	va_end(args);
}

void sprintf(char* buffer, const char* format, ...) 
{
	va_list args;
	va_start(args, format);
	put_common(&put_sprintf, buffer, 0, format, args);
	va_end(args);
}

void snprintf(char* buffer, size_t size, const char* format, ...) 
{
	va_list args;
	va_start(args, format);
	put_common(&put_snprintf, buffer, size, format, args);
	va_end(args);
}
