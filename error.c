/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:12:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

typedef struct s_err_entry
{
	unsigned int	code;
	const char		*msg;
}	t_err_entry;

static const t_err_entry	g_err_table[] = {
{0, "no error, everything went ok"},
{1, "nothing done yet"},
{10, "end of input memory reached without huffman end code"},
{11, "error in code tree made it jump outside of huffman tree"},
{13, "problem while processing dynamic deflate block"},
{14, "problem while processing dynamic deflate block"},
{15, "problem while processing dynamic deflate block"},
{16, "unexisting code while processing dynamic deflate block"},
{17, "end of out buffer memory reached while inflating"},
{18, "invalid distance code while inflating"},
{19, "end of out buffer memory reached while inflating"},
{20, "invalid deflate block BTYPE encountered while decoding"},
{21, "NLEN is not ones complement of LEN in a deflate block"},
{22, "end of out buffer memory reached while inflating"},
{23, "end of in buffer memory reached while inflating"},
{24, "invalid FCHECK in zlib header"},
{25, "invalid compression method in zlib header"},
{26, "FDICT encountered in zlib header while it's not used for PNG"},
{27, "PNG file is smaller than a PNG header"},
{28, "incorrect PNG signature, it's no PNG or corrupted"},
{29, "first chunk is not the header chunk"},
{30, "chunk length too large, chunk broken off at end of file"},
{31, "illegal PNG color type or bpp"},
{32, "illegal PNG compression method"},
{33, "illegal PNG filter method"},
{34, "illegal PNG interlace method"},
{35, "chunk length of a chunk is too large or the chunk too small"},
{36, "illegal PNG filter type encountered"},
{37, "illegal bit depth for this color type given"},
{38, "the palette is too big"},
{39, "tRNS chunk before PLTE or has more entries than palette size"},
{40, "tRNS chunk has wrong size for grayscale image"},
{41, "tRNS chunk has wrong size for RGB image"},
{42, "tRNS chunk appeared while it was not allowed for this color type"},
{43, "bKGD chunk has wrong size for palette image"},
{44, "bKGD chunk has wrong size for grayscale image"},
{45, "bKGD chunk has wrong size for RGB image"},
{48, "empty input buffer given to decoder. Maybe caused by non-existing file?"},
{49, "jumped past memory while generating dynamic huffman tree"},
{50, "jumped past memory while generating dynamic huffman tree"},
{51, "jumped past memory while inflating huffman block"},
{52, "jumped past memory while inflating"},
{53, "size of zlib data too small"},
{54, "repeat symbol in tree while there was no value symbol yet"},
{55, "jumped past tree while generating huffman tree"},
{56, "given output image colortype or bitdepth not supported for color conversion"},
{57, "invalid CRC encountered (checking CRC can be disabled)"},
{58, "invalid ADLER32 encountered (checking ADLER32 can be disabled)"},
{59, "requested color conversion not supported"},
{60, "invalid window size given in the settings of the encoder (must be 0-32768)"},
{61, "invalid BTYPE given in the settings of the encoder (only 0, 1 and 2 are allowed)"},
{62, "conversion from color to grayscale not supported"},
{63, "length of a chunk too long, max allowed for PNG is 2147483647 bytes per chunk"},
{64, "the length of the END symbol 256 in the Huffman tree is 0"},
{66, "the length of a text chunk keyword given to the encoder is longer than the maximum of 79 bytes"},
{67, "the length of a text chunk keyword given to the encoder is smaller than the minimum of 1 byte"},
{68, "tried to encode a PLTE chunk with a palette that has less than 1 or more than 256 colors"},
{69, "unknown chunk type with 'critical' flag encountered by the decoder"},
{71, "unexisting interlace mode given to encoder (must be 0 or 1)"},
{72, "while decoding, unexisting compression method encountering in zTXt or iTXt chunk (it must be 0)"},
{73, "invalid tIME chunk size"},
{74, "invalid pHYs chunk size"},
{75, "no null termination char found while decoding text chunk"},
{76, "iTXt chunk too short to contain required bytes"},
{77, "integer overflow in buffer size"},
{78, "failed to open file for reading"},
{79, "failed to open file for writing"},
{80, "tried creating a tree of 0 symbols"},
{81, "lazy matching at pos 0 is impossible"},
{82, "color conversion to palette requested while a color isn't in palette, or index out of bounds"},
{83, "memory allocation failed"},
{84, "given image too small to contain all pixels to be encoded"},
{86, "impossible offset in lz77 encoding (internal bug)"},
{87, "must provide custom zlib function pointer if LODEPNG_COMPILE_ZLIB is not defined"},
{88, "invalid filter strategy given for LodePNGEncoderSettings.filter_strategy"},
{89, "text chunk keyword too short or long: must have size 1-79"},
{90, "windowsize must be a power of two"},
{91, "invalid decompressed idat size"},
{92, "integer overflow due to too many pixels"},
{93, "zero width or height is invalid"},
{94, "header chunk must have a size of 13 bytes"},
{95, "integer overflow with combined idat chunk size"},
{96, "invalid gAMA chunk size"},
{97, "invalid cHRM chunk size"},
{98, "invalid sRGB chunk size"},
{99, "invalid sRGB rendering intent"},
{100, "invalid ICC profile color type, the PNG specification only allows RGB or GRAY"},
{101, "PNG specification does not allow RGB ICC profile on gray color types and vice versa"},
{102, "not allowed to set grayscale ICC profile with colored pixels by PNG specification"},
{103, "invalid palette index in bKGD chunk. Maybe it came before PLTE chunk?"},
{104, "invalid bKGD color while encoding (e.g. palette index out of range)"},
};

static const unsigned int	g_err_count = sizeof(g_err_table)
	/ sizeof(g_err_table[0]);

const char	*lodepng_error_text(unsigned int code)
{
	unsigned int	i;

	i = 0;
	while (i < g_err_count)
	{
		if (g_err_table[i].code == code)
			return (g_err_table[i].msg);
		i++;
	}
	return ("unknown error code");
}
