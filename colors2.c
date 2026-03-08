/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:19:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_color_mode_cleanup(LodePNGColorMode *info)
{
	lodepng_palette_clear(info);
}

unsigned int	lodepng_color_mode_copy(LodePNGColorMode *dest,
		const LodePNGColorMode *source)
{
	size_t	i;

	lodepng_color_mode_cleanup(dest);
	*dest = *source;
	if (source->palette)
	{
		dest->palette = (unsigned char *)lodepng_malloc(1024);
		if (!dest->palette && source->palettesize)
			return (83);
		i = 0;
		while (i != source->palettesize * 4)
		{
			dest->palette[i] = source->palette[i];
			i++;
		}
	}
	return (0);
}

LodePNGColorMode	lodepng_color_mode_make(LodePNGColorType colortype,
		unsigned int bitdepth)
{
	LodePNGColorMode	result;

	lodepng_color_mode_init(&result);
	result.colortype = colortype;
	result.bitdepth = bitdepth;
	return (result);
}

static int	color_mode_keys_equal(const LodePNGColorMode *a,
		const LodePNGColorMode *b)
{
	if (a->key_r != b->key_r)
		return (0);
	if (a->key_g != b->key_g)
		return (0);
	if (a->key_b != b->key_b)
		return (0);
	return (1);
}

int	lodepng_color_mode_equal(const LodePNGColorMode *a,
		const LodePNGColorMode *b)
{
	size_t	i;

	if (a->colortype != b->colortype || a->bitdepth != b->bitdepth)
		return (0);
	if (a->key_defined != b->key_defined)
		return (0);
	if (a->key_defined && !color_mode_keys_equal(a, b))
		return (0);
	if (a->palettesize != b->palettesize)
		return (0);
	i = 0;
	while (i != a->palettesize * 4)
	{
		if (a->palette[i] != b->palette[i])
			return (0);
		i++;
	}
	return (1);
}
