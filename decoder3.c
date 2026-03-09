/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:25:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	dg_postprocess(t_png_state *state, unsigned char **out,
		t_ucvector *scanlines)
{
	size_t			outsize;
	unsigned int	w;
	unsigned int	h;
	t_filter_dim	d;

	w = state->info_png.width;
	h = state->info_png.height;
	if (state->info_png.interlace_method != 0)
	{
		state->error = 34;
		return ;
	}
	outsize = lodepng_get_raw_size(w, h, &state->info_png.color);
	*out = (unsigned char *)lodepng_malloc(outsize);
	if (!*out)
	{
		state->error = 83;
		return ;
	}
	memset(*out, 0, outsize);
	d.bw = lodepng_get_bpp(&state->info_png.color);
	d.len = (w * d.bw + 7) / 8;
	d.bw = (d.bw + 7) / 8;
	state->error = unfilter(*out, scanlines->data, h, &d);
}

static void	dg_cv_io(t_conv_io *io, t_png_state *state,
		unsigned char *out, const unsigned char *data)
{
	io->out = out;
	io->in = data;
	io->numpixels = (size_t)state->info_png.width
		* state->info_png.height;
}

static void	dg_convert_color(t_png_state *state, unsigned char **out)
{
	unsigned char	*data;
	size_t			outsize;
	t_conv_io		io;

	data = *out;
	if (!(state->info_raw.colortype == LCT_RGB
			|| state->info_raw.colortype == LCT_RGBA)
		&& !(state->info_raw.bitdepth == 8))
	{
		state->error = 56;
		return ;
	}
	outsize = lodepng_get_raw_size(state->info_png.width,
			state->info_png.height, &state->info_raw);
	*out = (unsigned char *)lodepng_malloc(outsize);
	if (!(*out))
		state->error = 83;
	else
	{
		dg_cv_io(&io, state, *out, data);
		state->error = lodepng_convert(&io, &state->info_raw,
				&state->info_png.color);
	}
	lodepng_free(data);
}

void	decode_generic(unsigned char **out, t_png_state *state,
		const unsigned char *in, size_t insize)
{
	t_ucvector		idat;
	unsigned int	w;
	unsigned int	h;

	*out = 0;
	state->error = lodepng_inspect(state, in, insize);
	if (state->error)
		return ;
	w = state->info_png.width;
	h = state->info_png.height;
	if (lodepng_pixel_overflow(w, h,
			&state->info_png.color, &state->info_raw))
	{
		state->error = 92;
		return ;
	}
	ucvector_init(&idat);
	dg_chunk_loop(state, &idat, in, insize);
	if (!state->error)
		dg_decompress(state, out, &idat);
	ucvector_cleanup(&idat);
}

unsigned int	lodepng_decode(unsigned char **out, t_png_state *state,
		const unsigned char *in, size_t insize)
{
	*out = 0;
	decode_generic(out, state, in, insize);
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
		dg_convert_color(state, out);
	return (state->error);
}
