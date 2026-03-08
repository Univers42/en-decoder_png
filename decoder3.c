/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	dg_postprocess(LodePNGState *state, unsigned char **out,
		ucvector *scanlines, unsigned w, unsigned h)
{
	size_t	outsize;
	size_t	i;

	outsize = lodepng_get_raw_size(w, h, &state->info_png.color);
	*out = (unsigned char *)lodepng_malloc(outsize);
	if (!*out)
	{
		state->error = 83;
		return ;
	}
	i = 0;
	while (i < outsize)
	{
		(*out)[i] = 0;
		++i;
	}
	state->error = postProcessScanlines(*out, scanlines->data,
			w, h, &state->info_png);
}

static void	dg_convert_color(LodePNGState *state,
		unsigned char **out, unsigned w, unsigned h)
{
	unsigned char	*data;
	size_t			outsize;

	data = *out;
	if (!(state->info_raw.colortype == LCT_RGB
			|| state->info_raw.colortype == LCT_RGBA)
		&& !(state->info_raw.bitdepth == 8))
	{
		state->error = 56;
		return ;
	}
	outsize = lodepng_get_raw_size(w, h, &state->info_raw);
	*out = (unsigned char *)lodepng_malloc(outsize);
	if (!(*out))
		state->error = 83;
	else
		state->error = lodepng_convert(*out, data,
				&state->info_raw, &state->info_png.color, w, h);
	lodepng_free(data);
}

void	decodeGeneric(unsigned char **out, unsigned *w, unsigned *h,
		LodePNGState *state, const unsigned char *in, size_t insize)
{
	ucvector	idat;

	*out = 0;
	*w = 0;
	*h = 0;
	state->error = lodepng_inspect(w, h, state, in, insize);
	if (state->error)
		return ;
	if (lodepng_pixel_overflow(*w, *h,
			&state->info_png.color, &state->info_raw))
	{
		state->error = 92;
		return ;
	}
	ucvector_init(&idat);
	dg_chunk_loop(state, &idat, in, insize);
	if (!state->error)
		dg_decompress(state, out, &idat, *w, *h);
	ucvector_cleanup(&idat);
}

unsigned	lodepng_decode(unsigned char **out, unsigned *w,
		unsigned *h, LodePNGState *state,
		const unsigned char *in, size_t insize)
{
	*out = 0;
	decodeGeneric(out, w, h, state, in, insize);
	if (state->error)
		return (state->error);
	if (!state->decoder.color_convert
		|| lodepng_color_mode_equal(&state->info_raw,
			&state->info_png.color))
	{
		if (!state->decoder.color_convert)
		{
			state->error = lodepng_color_mode_copy(
					&state->info_raw, &state->info_png.color);
			if (state->error)
				return (state->error);
		}
	}
	else
		dg_convert_color(state, out, *w, *h);
	return (state->error);
}
