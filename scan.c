/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:40 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:56:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	post_process_scanlines(unsigned char *out, unsigned char *in,
			unsigned int w, unsigned int h, const t_png_info *info_png)
{
	unsigned int	bpp;

	if (info_png->interlace_method != 0)
		return (34);
	bpp = lodepng_get_bpp(&info_png->color);
	return (unfilter(out, in, w, h, bpp));
}

unsigned int	read_chunk_plte(t_png_color_mode *color,
			const unsigned char *data, size_t chunk_length)
{
	size_t	i;

	if (chunk_length % 3 != 0)
		return (38);
	color->palettesize = chunk_length / 3;
	color->palette = (unsigned char *)lodepng_malloc(
			4 * color->palettesize);
	if (!color->palette && color->palettesize)
		return (83);
	i = 0;
	while (i < color->palettesize)
	{
		color->palette[4 * i + 0] = data[3 * i + 0];
		color->palette[4 * i + 1] = data[3 * i + 1];
		color->palette[4 * i + 2] = data[3 * i + 2];
		color->palette[4 * i + 3] = 255;
		++i;
	}
	return (0);
}

unsigned int	read_chunk_trns(t_png_color_mode *color,
			const unsigned char *data, size_t chunk_length)
{
	(void)color;
	(void)data;
	(void)chunk_length;
	return (0);
}

size_t	lodepng_get_raw_size_idat(unsigned int w, unsigned int h,
		const t_png_color_mode *color)
{
	size_t	linebytes;

	linebytes = ((size_t)w * lodepng_get_bpp(color) + 7) / 8;
	return (h * (linebytes + 1));
}

void	pre_process_scanlines(unsigned char **out, size_t *outsize,
		const unsigned char *in, unsigned int w, unsigned int h,
		const t_png_info *info, const t_encoder_settings *settings)
{
	(void)settings;
	*outsize = lodepng_get_raw_size(w, h, &info->color);
	*out = (unsigned char *)lodepng_malloc(*outsize);
	if (*out && *outsize)
		memcpy(*out, in, *outsize);
}
