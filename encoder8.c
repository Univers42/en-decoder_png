/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder8.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 01:05:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:58:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_DISK

unsigned int	lodepng_encode_file(const char *filename,
		const unsigned char *image, unsigned int w,
		unsigned int h, t_png_color_type colortype, unsigned int bitdepth)
{
	unsigned char	*buffer;
	size_t			buffersize;
	unsigned int	error;

	error = lodepng_encode_memory(&buffer, &buffersize,
			image, w, h, colortype, bitdepth);
	if (!error)
		error = lodepng_save_file(buffer, buffersize, filename);
	lodepng_free(buffer);
	return (error);
}

unsigned int	lodepng_encode32_file(const char *filename,
		const unsigned char *image, unsigned int w, unsigned int h)
{
	return (lodepng_encode_file(filename, image,
			w, h, LCT_RGBA, 8));
}

unsigned int	lodepng_encode24_file(const char *filename,
		const unsigned char *image, unsigned int w, unsigned int h)
{
	return (lodepng_encode_file(filename, image,
			w, h, LCT_RGB, 8));
}

#endif
