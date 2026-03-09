/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_palette_add(t_png_color_mode *info,
		const unsigned char *rgba)
{
	unsigned char	*data;

	if (!info->palette)
	{
		data = (unsigned char *)lodepng_realloc(info->palette, 1024);
		if (!data)
			return (83);
		info->palette = data;
	}
	info->palette[4 * info->palettesize + 0] = rgba[0];
	info->palette[4 * info->palettesize + 1] = rgba[1];
	info->palette[4 * info->palettesize + 2] = rgba[2];
	info->palette[4 * info->palettesize + 3] = rgba[3];
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
