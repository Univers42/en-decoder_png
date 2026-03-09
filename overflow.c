/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overflow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 23:03:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int	lodepng_mulofl(size_t a, size_t b, size_t *result)
{
	*result = a * b;
	return (a != 0 && *result / a != b);
}

int	lodepng_addofl(size_t a, size_t b, size_t *result)
{
	*result = a + b;
	return (*result < a);
}

static int	pixel_overflow_check(size_t *line, unsigned int w, size_t bpp)
{
	if (lodepng_mulofl((size_t)(w / 8), bpp, line))
		return (1);
	if (lodepng_addofl(*line, ((w & 7) * bpp + 7) / 8, line))
		return (1);
	if (lodepng_addofl(*line, 5, line))
		return (1);
	return (0);
}

int	lodepng_pixel_overflow(unsigned int w, unsigned int h,
		const t_png_color_mode *pngcolor, const t_png_color_mode *rawcolor)
{
	size_t	bpp;
	size_t	numpixels;
	size_t	total;
	size_t	line;

	bpp = lodepng_max_uint(lodepng_get_bpp(pngcolor),
			lodepng_get_bpp(rawcolor));
	if (lodepng_mulofl((size_t)w, (size_t)h, &numpixels))
		return (1);
	if (lodepng_mulofl(numpixels, 8, &total))
		return (1);
	if (pixel_overflow_check(&line, w, bpp))
		return (1);
	if (lodepng_mulofl(line, h, &total))
		return (1);
	return (0);
}
