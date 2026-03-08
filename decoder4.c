/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	lodepng_decode_memory(unsigned char **out, unsigned *w,
		unsigned *h, const unsigned char *in, size_t insize,
		LodePNGColorType colortype, unsigned bitdepth)
{
	unsigned		error;
	LodePNGState	state;

	lodepng_state_init(&state);
	state.info_raw.colortype = colortype;
	state.info_raw.bitdepth = bitdepth;
	error = lodepng_decode(out, w, h, &state, in, insize);
	lodepng_state_cleanup(&state);
	return (error);
}

unsigned	lodepng_decode32_memory(unsigned char **out, unsigned *w,
		unsigned *h, const unsigned char *in, size_t insize)
{
	return (lodepng_decode_memory(out, w, h, in, insize, LCT_RGBA, 8));
}

unsigned	lodepng_decode32(unsigned char **out, unsigned *w,
		unsigned *h, const unsigned char *in, size_t insize)
{
	return (lodepng_decode_memory(out, w, h, in, insize, LCT_RGBA, 8));
}

unsigned	lodepng_decode24(unsigned char **out, unsigned *w,
		unsigned *h, const unsigned char *in, size_t insize)
{
	return (lodepng_decode_memory(out, w, h, in, insize, LCT_RGB, 8));
}

#ifdef LODEPNG_COMPILE_DISK
unsigned	lodepng_decode_file(unsigned char **out, unsigned *w,
		unsigned *h, const char *filename,
		LodePNGColorType colortype, unsigned bitdepth)
{
	unsigned char	*buffer;
	size_t			buffersize;
	unsigned		error;

	buffer = 0;
	*out = 0;
	*w = 0;
	*h = 0;
	error = lodepng_load_file(&buffer, &buffersize, filename);
	if (!error)
		error = lodepng_decode_memory(out, w, h, buffer,
				buffersize, colortype, bitdepth);
	lodepng_free(buffer);
	return (error);
}
#endif
