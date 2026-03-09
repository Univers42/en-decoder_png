/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:23:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	gpc8_alpha_key(unsigned char *rgba,
		const t_png_color_mode *mode, unsigned int value)
{
	rgba[3] = 255;
	if (mode->key_defined && value == mode->key_r)
		rgba[3] = 0;
}

static void	gpc8_grey_bits(unsigned char *rgba,
		const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	unsigned int	highest;
	size_t			j;
	unsigned int	value;

	highest = ((1U << mode->bitdepth) - 1U);
	j = i * mode->bitdepth;
	value = read_bits_from_rev_stream(&j, in,
			mode->bitdepth);
	rgba[0] = (value * 255) / highest;
	gpc8_alpha_key(rgba, mode, value);
}

void	gpc8_grey(unsigned char *rgba, const unsigned char *in,
		size_t i, const t_png_color_mode *mode)
{
	if (mode->bitdepth == 8)
	{
		rgba[0] = in[i];
		gpc8_alpha_key(rgba, mode, in[i]);
	}
	else if (mode->bitdepth == 16)
	{
		rgba[0] = in[i * 2];
		gpc8_alpha_key(rgba, mode,
			256U * in[i * 2] + in[i * 2 + 1]);
	}
	else
		gpc8_grey_bits(rgba, in, i, mode);
	rgba[1] = rgba[0];
	rgba[2] = rgba[0];
}

static int	gpc8_rgb_key16(const unsigned char *in, size_t i,
		const t_png_color_mode *mode)
{
	if (!mode->key_defined)
		return (0);
	if (256U * in[i * 6] + in[i * 6 + 1] != mode->key_r)
		return (0);
	if (256U * in[i * 6 + 2] + in[i * 6 + 3] != mode->key_g)
		return (0);
	if (256U * in[i * 6 + 4] + in[i * 6 + 5] != mode->key_b)
		return (0);
	return (1);
}

void	gpc8_rgb(unsigned char *rgba, const unsigned char *in,
		size_t i, const t_png_color_mode *mode)
{
	if (mode->bitdepth == 8)
	{
		rgba[0] = in[i * 3];
		rgba[1] = in[i * 3 + 1];
		rgba[2] = in[i * 3 + 2];
		rgba[3] = 255;
		if (mode->key_defined && rgba[0] == mode->key_r
			&& rgba[1] == mode->key_g
			&& rgba[2] == mode->key_b)
			rgba[3] = 0;
	}
	else
	{
		rgba[0] = in[i * 6];
		rgba[1] = in[i * 6 + 2];
		rgba[2] = in[i * 6 + 4];
		rgba[3] = 255;
		if (gpc8_rgb_key16(in, i, mode))
			rgba[3] = 0;
	}
}
