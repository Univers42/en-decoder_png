/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:17:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	r16_write_rgba(unsigned char *out, size_t i,
		const unsigned short *rgba)
{
	out[i * 8] = (rgba[0] >> 8) & 255;
	out[i * 8 + 1] = rgba[0] & 255;
	out[i * 8 + 2] = (rgba[1] >> 8) & 255;
	out[i * 8 + 3] = rgba[1] & 255;
	out[i * 8 + 4] = (rgba[2] >> 8) & 255;
	out[i * 8 + 5] = rgba[2] & 255;
	out[i * 8 + 6] = (rgba[3] >> 8) & 255;
	out[i * 8 + 7] = rgba[3] & 255;
}

void	rgba16_to_pixel(unsigned char *out, size_t i,
		const t_png_color_mode *mode, const unsigned short *rgba)
{
	if (mode->colortype == LCT_GREY)
	{
		out[i * 2] = (rgba[0] >> 8) & 255;
		out[i * 2 + 1] = rgba[0] & 255;
	}
	else if (mode->colortype == LCT_RGB)
	{
		out[i * 6] = (rgba[0] >> 8) & 255;
		out[i * 6 + 1] = rgba[0] & 255;
		out[i * 6 + 2] = (rgba[1] >> 8) & 255;
		out[i * 6 + 3] = rgba[1] & 255;
		out[i * 6 + 4] = (rgba[2] >> 8) & 255;
		out[i * 6 + 5] = rgba[2] & 255;
	}
	else if (mode->colortype == LCT_GREY_ALPHA)
	{
		out[i * 4] = (rgba[0] >> 8) & 255;
		out[i * 4 + 1] = rgba[0] & 255;
		out[i * 4 + 2] = (rgba[3] >> 8) & 255;
		out[i * 4 + 3] = rgba[3] & 255;
	}
	else if (mode->colortype == LCT_RGBA)
		r16_write_rgba(out, i, rgba);
}

static void	gpc16_grey(unsigned short *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	rgba[0] = 256u * in[i * 2] + in[i * 2 + 1];
	rgba[1] = rgba[0];
	rgba[2] = rgba[0];
	if (mode->key_defined && rgba[0] == mode->key_r)
		rgba[3] = 0;
	else
		rgba[3] = 65535;
}

static void	gpc16_rgb(unsigned short *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	rgba[0] = 256u * in[i * 6] + in[i * 6 + 1];
	rgba[1] = 256u * in[i * 6 + 2] + in[i * 6 + 3];
	rgba[2] = 256u * in[i * 6 + 4] + in[i * 6 + 5];
	if (mode->key_defined && rgba[0] == mode->key_r
		&& rgba[1] == mode->key_g && rgba[2] == mode->key_b)
		rgba[3] = 0;
	else
		rgba[3] = 65535;
}

void	get_pixel_color_rgba16(unsigned short *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	if (mode->colortype == LCT_GREY)
		gpc16_grey(rgba, in, i, mode);
	else if (mode->colortype == LCT_RGB)
		gpc16_rgb(rgba, in, i, mode);
	else if (mode->colortype == LCT_GREY_ALPHA)
	{
		rgba[0] = 256u * in[i * 4] + in[i * 4 + 1];
		rgba[1] = rgba[0];
		rgba[2] = rgba[0];
		rgba[3] = 256u * in[i * 4 + 2] + in[i * 4 + 3];
	}
	else
	{
		rgba[0] = 256u * in[i * 8] + in[i * 8 + 1];
		rgba[1] = 256u * in[i * 8 + 2] + in[i * 8 + 3];
		rgba[2] = 256u * in[i * 8 + 4] + in[i * 8 + 5];
		rgba[3] = 256u * in[i * 8 + 6] + in[i * 8 + 7];
	}
}
