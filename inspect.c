/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inspect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:52:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 22:37:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	validate_input(const unsigned char *in,
				size_t insize)
{
	if (insize == 0 || in == 0)
		return (48);
	if (insize < 33)
		return (27);
	return (0);
}

static unsigned int	check_png_header(const unsigned char *in)
{
	if (in[0] != 137 || in[1] != 80
		|| in[2] != 78 || in[3] != 71
		|| in[4] != 13 || in[5] != 10
		|| in[6] != 26 || in[7] != 10)
		return (28);
	if (lodepng_chunk_length(in + 8) != 13)
		return (94);
	if (!lodepng_chunk_type_equals(in + 8, "IHDR"))
		return (29);
	return (0);
}

static unsigned int	parse_ihdr(t_png_info *info,
				const unsigned char *in)
{
	info->width = lodepng_read_32bit_int(&in[16]);
	info->height = lodepng_read_32bit_int(&in[20]);
	info->color.bitdepth = in[24];
	info->color.colortype = (t_png_color_type)in[25];
	info->compression_method = in[26];
	info->filter_method = in[27];
	info->interlace_method = in[28];
	if (info->width == 0 || info->height == 0)
		return (93);
	if (info->compression_method != 0)
		return (32);
	if (info->filter_method != 0)
		return (33);
	if (info->interlace_method > 1)
		return (34);
	return (0);
}

static unsigned int	check_ihdr_crc(const unsigned char *in,
				unsigned int ignore_crc)
{
	unsigned int	crc;
	unsigned int	checksum;

	if (ignore_crc)
		return (0);
	crc = lodepng_read_32bit_int(&in[29]);
	checksum = lodepng_crc32(&in[12], 17);
	if (crc != checksum)
		return (57);
	return (0);
}

unsigned int	lodepng_inspect(t_png_state *state,
				const unsigned char *in, size_t insize)
{
	t_png_info	*info;

	info = &state->info_png;
	state->error = validate_input(in, insize);
	if (state->error)
		return (state->error);
	lodepng_info_cleanup(info);
	lodepng_info_init(info);
	state->error = check_png_header(in);
	if (state->error)
		return (state->error);
	state->error = parse_ihdr(info, in);
	if (state->error)
		return (state->error);
	state->error = check_ihdr_crc(in,
			state->decoder.ignore_crc);
	if (state->error)
		return (state->error);
	state->error = check_color_validity(
			info->color.colortype, info->color.bitdepth);
	return (state->error);
}
