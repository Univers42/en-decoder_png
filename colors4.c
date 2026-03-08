/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:20:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_is_palette_type(const t_png_color_mode *info)
{
	return (info->colortype == LCT_PALETTE);
}

unsigned int	lodepng_has_palette_alpha(const t_png_color_mode *info)
{
	size_t	i;

	i = 0;
	while (i != info->palettesize)
	{
		if (info->palette[i * 4 + 3] < 255)
			return (1);
		i++;
	}
	return (0);
}

unsigned int	lodepng_can_have_alpha(const t_png_color_mode *info)
{
	return (info->key_defined || lodepng_is_alpha_type(info)
		|| lodepng_has_palette_alpha(info));
}

size_t	lodepng_get_raw_size_lct(unsigned int w, unsigned int h,
		t_png_color_type colortype, unsigned int bitdepth)
{
	size_t	bpp;
	size_t	n;

	bpp = lodepng_get_bpp_lct(colortype, bitdepth);
	n = (size_t)w * (size_t)h;
	return (((n / 8) * bpp) + ((n & 7) * bpp + 7) / 8);
}

size_t	lodepng_get_raw_size(unsigned int w, unsigned int h,
		const t_png_color_mode *color)
{
	return (lodepng_get_raw_size_lct(w, h,
			color->colortype, color->bitdepth));
}
