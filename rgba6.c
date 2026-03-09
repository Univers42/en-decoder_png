/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:25:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	lcr_from_palette(unsigned int *rgb,
		const unsigned int *in, const t_png_color_mode *mode)
{
	if (in[0] >= mode->palettesize)
		return (82);
	rgb[0] = mode->palette[in[0] * 4] * 257u;
	rgb[1] = mode->palette[in[0] * 4 + 1] * 257u;
	rgb[2] = mode->palette[in[0] * 4 + 2] * 257u;
	return (0);
}

static unsigned int	lcr_from_input(unsigned int *rgb,
		const unsigned int *in, const t_png_color_mode *mode)
{
	unsigned int	mul;

	mul = 65535 / ((1u << mode->bitdepth) - 1u);
	if (mode->colortype == LCT_GREY
		|| mode->colortype == LCT_GREY_ALPHA)
	{
		rgb[0] = in[0] * mul;
		rgb[1] = rgb[0];
		rgb[2] = rgb[0];
	}
	else if (mode->colortype == LCT_RGB
		|| mode->colortype == LCT_RGBA)
	{
		rgb[0] = in[0] * mul;
		rgb[1] = in[1] * mul;
		rgb[2] = in[2] * mul;
	}
	else if (mode->colortype == LCT_PALETTE)
		return (lcr_from_palette(rgb, in, mode));
	else
		return (31);
	return (0);
}

static unsigned int	lcr_to_grey_rgb(unsigned int *out,
		const unsigned int *rgb, const t_png_color_mode *mode)
{
	unsigned int	shift;

	shift = 16 - mode->bitdepth;
	if (mode->colortype == LCT_GREY
		|| mode->colortype == LCT_GREY_ALPHA)
		out[0] = rgb[0] >> shift;
	else if (mode->colortype == LCT_RGB
		|| mode->colortype == LCT_RGBA)
	{
		out[0] = rgb[0] >> shift;
		out[1] = rgb[1] >> shift;
		out[2] = rgb[2] >> shift;
	}
	else
		return (31);
	return (0);
}

static unsigned int	lcr_to_palette(unsigned int *out,
		const unsigned int *rgb, const t_png_color_mode *mode)
{
	unsigned int	i;
	unsigned int	j;

	if ((rgb[0] >> 8) != (rgb[0] & 255)
		|| (rgb[1] >> 8) != (rgb[1] & 255)
		|| (rgb[2] >> 8) != (rgb[2] & 255))
		return (82);
	i = 0;
	while (i < mode->palettesize)
	{
		j = i * 4;
		if ((rgb[0] >> 8) == mode->palette[j]
			&& (rgb[1] >> 8) == mode->palette[j + 1]
			&& (rgb[2] >> 8) == mode->palette[j + 2])
		{
			out[0] = i;
			return (0);
		}
		++i;
	}
	return (82);
}

unsigned int	lodepng_convert_rgb(unsigned int *out,
		const unsigned int *in,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in)
{
	unsigned int	rgb[3];
	unsigned int	error;

	error = lcr_from_input(rgb, in, mode_in);
	if (error)
		return (error);
	if (mode_out->colortype == LCT_PALETTE)
		return (lcr_to_palette(out, rgb, mode_out));
	return (lcr_to_grey_rgb(out, rgb, mode_out));
}
