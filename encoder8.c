/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder8.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 01:05:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 01:05:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_DISK

unsigned	lodepng_encode_file(const char *filename,
		const unsigned char *image, unsigned w,
		unsigned h, LodePNGColorType colortype, unsigned bitdepth)
{
	unsigned char	*buffer;
	size_t			buffersize;
	unsigned		error;

	error = lodepng_encode_memory(&buffer, &buffersize,
			image, w, h, colortype, bitdepth);
	if (!error)
		error = lodepng_save_file(buffer, buffersize, filename);
	lodepng_free(buffer);
	return (error);
}

unsigned	lodepng_encode32_file(const char *filename,
		const unsigned char *image, unsigned w, unsigned h)
{
	return (lodepng_encode_file(filename, image,
			w, h, LCT_RGBA, 8));
}

unsigned	lodepng_encode24_file(const char *filename,
		const unsigned char *image, unsigned w, unsigned h)
{
	return (lodepng_encode_file(filename, image,
			w, h, LCT_RGB, 8));
}

#endif
