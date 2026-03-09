/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:23:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	gpc8_palette(unsigned char *rgba, const unsigned char *in,
		size_t i, const t_png_color_mode *mode)
{
	unsigned int	index;
	size_t			j;

	if (mode->bitdepth == 8)
		index = in[i];
	else
	{
		j = i * mode->bitdepth;
		index = read_bits_from_rev_stream(&j, in,
				mode->bitdepth);
	}
	if (index >= mode->palettesize)
	{
		rgba[0] = 0;
		rgba[1] = 0;
		rgba[2] = 0;
		rgba[3] = 255;
		return ;
	}
	rgba[0] = mode->palette[index * 4];
	rgba[1] = mode->palette[index * 4 + 1];
	rgba[2] = mode->palette[index * 4 + 2];
	rgba[3] = mode->palette[index * 4 + 3];
}

static void	gpc8_grey_alpha(unsigned char *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	if (mode->bitdepth == 8)
	{
		rgba[0] = in[i * 2];
		rgba[3] = in[i * 2 + 1];
	}
	else
	{
		rgba[0] = in[i * 4];
		rgba[3] = in[i * 4 + 2];
	}
	rgba[1] = rgba[0];
	rgba[2] = rgba[0];
}

static void	gpc8_rgba(unsigned char *rgba, const unsigned char *in,
		size_t i, const t_png_color_mode *mode)
{
	if (mode->bitdepth == 8)
	{
		rgba[0] = in[i * 4];
		rgba[1] = in[i * 4 + 1];
		rgba[2] = in[i * 4 + 2];
		rgba[3] = in[i * 4 + 3];
	}
	else
	{
		rgba[0] = in[i * 8];
		rgba[1] = in[i * 8 + 2];
		rgba[2] = in[i * 8 + 4];
		rgba[3] = in[i * 8 + 6];
	}
}

void	get_pixel_color_rgba8(unsigned char *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	if (mode->colortype == LCT_GREY)
		gpc8_grey(rgba, in, i, mode);
	else if (mode->colortype == LCT_RGB)
		gpc8_rgb(rgba, in, i, mode);
	else if (mode->colortype == LCT_PALETTE)
		gpc8_palette(rgba, in, i, mode);
	else if (mode->colortype == LCT_GREY_ALPHA)
		gpc8_grey_alpha(rgba, in, i, mode);
	else if (mode->colortype == LCT_RGBA)
		gpc8_rgba(rgba, in, i, mode);
}
