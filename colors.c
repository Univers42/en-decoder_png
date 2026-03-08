/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:19:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	check_grey_palette(unsigned int bd)
{
	if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16))
		return (37);
	return (0);
}

unsigned int	checkColorValidity(LodePNGColorType colortype,
		unsigned int bd)
{
	if (colortype == 0)
		return (check_grey_palette(bd));
	if (colortype == 2 || colortype == 4 || colortype == 6)
	{
		if (!(bd == 8 || bd == 16))
			return (37);
		return (0);
	}
	if (colortype == 3)
	{
		if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8))
			return (37);
		return (0);
	}
	return (31);
}

unsigned int	getNumColorChannels(LodePNGColorType colortype)
{
	if (colortype == 0 || colortype == 3)
		return (1);
	if (colortype == 2)
		return (3);
	if (colortype == 4)
		return (2);
	if (colortype == 6)
		return (4);
	return (0);
}

unsigned int	lodepng_get_bpp_lct(LodePNGColorType colortype,
		unsigned int bitdepth)
{
	return (getNumColorChannels(colortype) * bitdepth);
}

void	lodepng_palette_clear(LodePNGColorMode *info)
{
	if (info->palette)
		lodepng_free(info->palette);
	info->palette = 0;
	info->palettesize = 0;
}

void	lodepng_color_mode_init(LodePNGColorMode *info)
{
	info->key_defined = 0;
	info->key_r = 0;
	info->key_g = 0;
	info->colortype = LCT_RGBA;
	info->bitdepth = 8;
	info->palette = 0;
	info->palettesize = 0;
}
