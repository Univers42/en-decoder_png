/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:19:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_palette_add(t_png_color_mode *info,
		unsigned char r, unsigned char g,
		unsigned char b, unsigned char a)
{
	unsigned char	*data;

	if (!info->palette)
	{
		data = (unsigned char *)lodepng_realloc(info->palette, 1024);
		if (!data)
			return (83);
		info->palette = data;
	}
	info->palette[4 * info->palettesize + 0] = r;
	info->palette[4 * info->palettesize + 1] = g;
	info->palette[4 * info->palettesize + 2] = b;
	info->palette[4 * info->palettesize + 3] = a;
	++info->palettesize;
	return (0);
}

unsigned int	lodepng_get_bpp(const t_png_color_mode *info)
{
	return (lodepng_get_bpp_lct(info->colortype, info->bitdepth));
}

unsigned int	lodepng_get_channels(const t_png_color_mode *info)
{
	return (get_num_color_channels(info->colortype));
}

unsigned int	lodepng_is_greyscale_type(const t_png_color_mode *info)
{
	return (info->colortype == LCT_GREY
		|| info->colortype == LCT_GREY_ALPHA);
}

unsigned int	lodepng_is_alpha_type(const t_png_color_mode *info)
{
	return ((info->colortype & 4) != 0);
}
