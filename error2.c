/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 03:57:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <stddef.h>

static const char	*err_tbl_5(unsigned int code)
{
	if (code == 59)
		return ("requested color conversion not supported");
	if (code == 60)
		return ("invalid encoder window size (0-32768)");
	if (code == 61)
		return ("invalid encoder BTYPE (only 0, 1, 2)");
	if (code == 62)
		return ("color to grayscale conversion unsupported");
	if (code == 63)
		return ("chunk length exceeds PNG max");
	if (code == 64)
		return ("END symbol 256 has length 0 in huffman");
	if (code == 66)
		return ("text chunk keyword too long (max 79)");
	if (code == 67)
		return ("text chunk keyword too short (min 1)");
	if (code == 68)
		return ("PLTE palette must have 1-256 colors");
	if (code == 69)
		return ("unknown critical chunk type encountered");
	if (code == 71)
		return ("invalid interlace mode (must be 0 or 1)");
	if (code == 72)
		return ("invalid compression in zTXt/iTXt");
	return (NULL);
}

static const char	*err_tbl_6(unsigned int code)
{
	if (code == 73)
		return ("invalid tIME chunk size");
	if (code == 74)
		return ("invalid pHYs chunk size");
	if (code == 75)
		return ("no null termination in text chunk");
	if (code == 76)
		return ("iTXt chunk too short for required bytes");
	if (code == 77)
		return ("integer overflow in buffer size");
	if (code == 78)
		return ("failed to open file for reading");
	if (code == 79)
		return ("failed to open file for writing");
	if (code == 80)
		return ("tried creating a tree of 0 symbols");
	if (code == 81)
		return ("lazy matching at pos 0 is impossible");
	if (code == 82)
		return ("palette color not found or out of bounds");
	if (code == 83)
		return ("memory allocation failed");
	if (code == 84)
		return ("image too small for all pixels to encode");
	return (NULL);
}

static const char	*err_tbl_7(unsigned int code)
{
	if (code == 86)
		return ("impossible offset in lz77 (internal bug)");
	if (code == 87)
		return ("custom zlib function required without ZLIB");
	if (code == 88)
		return ("invalid filter strategy for encoder");
	if (code == 89)
		return ("text keyword must have size 1-79");
	if (code == 90)
		return ("windowsize must be a power of two");
	if (code == 91)
		return ("invalid decompressed idat size");
	if (code == 92)
		return ("integer overflow due to too many pixels");
	if (code == 93)
		return ("zero width or height is invalid");
	if (code == 94)
		return ("header chunk must have size of 13 bytes");
	if (code == 95)
		return ("integer overflow with idat chunk size");
	if (code == 96)
		return ("invalid gAMA chunk size");
	if (code == 97)
		return ("invalid cHRM chunk size");
	return (NULL);
}

static const char	*err_tbl_8(unsigned int code)
{
	if (code == 98)
		return ("invalid sRGB chunk size");
	if (code == 99)
		return ("invalid sRGB rendering intent");
	if (code == 100)
		return ("ICC profile type must be RGB or GRAY");
	if (code == 101)
		return ("ICC profile must match PNG color type");
	if (code == 102)
		return ("gray ICC profile with colored pixels");
	if (code == 103)
		return ("invalid palette index in bKGD chunk");
	if (code == 104)
		return ("invalid bKGD color while encoding");
	return (NULL);
}

const char	*err_ext_lookup(unsigned int code)
{
	const char	*r;

	r = err_tbl_5(code);
	if (!r)
		r = err_tbl_6(code);
	if (!r)
		r = err_tbl_7(code);
	if (!r)
		r = err_tbl_8(code);
	return (r);
}
