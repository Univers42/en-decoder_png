/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 03:57:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <stddef.h>

static const char	*err_tbl_1(unsigned int code)
{
	if (code == 0)
		return ("no error, everything went ok");
	if (code == 1)
		return ("nothing done yet");
	if (code == 10)
		return ("end of input without huffman end code");
	if (code == 11)
		return ("code tree jump outside of huffman tree");
	if (code == 13)
		return ("problem processing dynamic deflate block");
	if (code == 14)
		return ("problem processing dynamic deflate block");
	if (code == 15)
		return ("problem processing dynamic deflate block");
	if (code == 16)
		return ("unexisting code in dynamic deflate block");
	if (code == 17)
		return ("end of buffer reached while inflating");
	if (code == 18)
		return ("invalid distance code while inflating");
	if (code == 19)
		return ("end of buffer reached while inflating");
	if (code == 20)
		return ("invalid deflate block BTYPE while decoding");
	return (NULL);
}

static const char	*err_tbl_2(unsigned int code)
{
	if (code == 21)
		return ("NLEN is not ones complement of LEN");
	if (code == 22)
		return ("end of buffer reached while inflating");
	if (code == 23)
		return ("end of in buffer reached while inflating");
	if (code == 24)
		return ("invalid FCHECK in zlib header");
	if (code == 25)
		return ("invalid compression method in zlib header");
	if (code == 26)
		return ("FDICT in zlib header not used for PNG");
	if (code == 27)
		return ("PNG file is smaller than a PNG header");
	if (code == 28)
		return ("incorrect PNG signature or corrupted");
	if (code == 29)
		return ("first chunk is not the header chunk");
	if (code == 30)
		return ("chunk length too large, broken at end");
	if (code == 31)
		return ("illegal PNG color type or bpp");
	if (code == 32)
		return ("illegal PNG compression method");
	return (NULL);
}

static const char	*err_tbl_3(unsigned int code)
{
	if (code == 33)
		return ("illegal PNG filter method");
	if (code == 34)
		return ("illegal PNG interlace method");
	if (code == 35)
		return ("chunk length too large or chunk too small");
	if (code == 36)
		return ("illegal PNG filter type encountered");
	if (code == 37)
		return ("illegal bit depth for this color type");
	if (code == 38)
		return ("the palette is too big");
	if (code == 39)
		return ("tRNS before PLTE or too many entries");
	if (code == 40)
		return ("tRNS wrong size for grayscale image");
	if (code == 41)
		return ("tRNS chunk has wrong size for RGB image");
	if (code == 42)
		return ("tRNS not allowed for this color type");
	if (code == 43)
		return ("bKGD wrong size for palette image");
	if (code == 44)
		return ("bKGD wrong size for grayscale image");
	return (NULL);
}

static const char	*err_tbl_4(unsigned int code)
{
	if (code == 45)
		return ("bKGD chunk has wrong size for RGB image");
	if (code == 48)
		return ("empty input or non-existing file");
	if (code == 49)
		return ("jumped past memory generating huffman tree");
	if (code == 50)
		return ("jumped past memory generating huffman tree");
	if (code == 51)
		return ("jumped past memory inflating huffman");
	if (code == 52)
		return ("jumped past memory while inflating");
	if (code == 53)
		return ("size of zlib data too small");
	if (code == 54)
		return ("repeat symbol without value symbol yet");
	if (code == 55)
		return ("jumped past tree generating huffman tree");
	if (code == 56)
		return ("unsupported colortype or bitdepth");
	if (code == 57)
		return ("invalid CRC (checking can be disabled)");
	if (code == 58)
		return ("invalid ADLER32 (can be disabled)");
	return (NULL);
}

const char	*lodepng_error_text(unsigned int code)
{
	const char	*r;

	r = err_tbl_1(code);
	if (!r)
		r = err_tbl_2(code);
	if (!r)
		r = err_tbl_3(code);
	if (!r)
		r = err_tbl_4(code);
	if (!r)
		r = err_ext_lookup(code);
	if (!r)
		return ("unknown error code");
	return (r);
}
