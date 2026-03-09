/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:17:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	r8_to_grey(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned char *rgba)
{
	unsigned char	grey;

	grey = rgba[0];
	if (mode->bitdepth == 8)
		out[i] = grey;
	else if (mode->bitdepth == 16)
	{
		out[i * 2] = grey;
		out[i * 2 + 1] = grey;
	}
	else
	{
		grey = (grey >> (8 - mode->bitdepth))
			& ((1 << mode->bitdepth) - 1);
		add_color_bits(out, i, mode->bitdepth, grey);
	}
}

static void	r8_to_rgb(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned char *rgba)
{
	if (mode->bitdepth == 8)
	{
		out[i * 3] = rgba[0];
		out[i * 3 + 1] = rgba[1];
		out[i * 3 + 2] = rgba[2];
	}
	else
	{
		out[i * 6] = rgba[0];
		out[i * 6 + 1] = rgba[0];
		out[i * 6 + 2] = rgba[1];
		out[i * 6 + 3] = rgba[1];
		out[i * 6 + 4] = rgba[2];
		out[i * 6 + 5] = rgba[2];
	}
}

static void	r8_to_ga(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned char *rgba)
{
	if (mode->bitdepth == 8)
	{
		out[i * 2] = rgba[0];
		out[i * 2 + 1] = rgba[3];
	}
	else if (mode->bitdepth == 16)
	{
		out[i * 4] = rgba[0];
		out[i * 4 + 1] = rgba[0];
		out[i * 4 + 2] = rgba[3];
		out[i * 4 + 3] = rgba[3];
	}
}

static void	r8_to_rgba(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned char *rgba)
{
	if (mode->bitdepth == 8)
	{
		out[i * 4] = rgba[0];
		out[i * 4 + 1] = rgba[1];
		out[i * 4 + 2] = rgba[2];
		out[i * 4 + 3] = rgba[3];
	}
	else
	{
		out[i * 8] = rgba[0];
		out[i * 8 + 1] = rgba[0];
		out[i * 8 + 2] = rgba[1];
		out[i * 8 + 3] = rgba[1];
		out[i * 8 + 4] = rgba[2];
		out[i * 8 + 5] = rgba[2];
		out[i * 8 + 6] = rgba[3];
		out[i * 8 + 7] = rgba[3];
	}
}

unsigned int	rgba8_to_pixel(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned char *rgba)
{
	if (mode->colortype == LCT_GREY)
		r8_to_grey(out, i, mode, rgba);
	else if (mode->colortype == LCT_RGB)
		r8_to_rgb(out, i, mode, rgba);
	else if (mode->colortype == LCT_GREY_ALPHA)
		r8_to_ga(out, i, mode, rgba);
	else if (mode->colortype == LCT_RGBA)
		r8_to_rgba(out, i, mode, rgba);
	else
		return (31);
	return (0);
}
