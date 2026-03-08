/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	dg_validate_chunk(t_png_state *state,
		const unsigned char *chunk, const unsigned char *in,
		size_t insize)
{
	unsigned int	len;

	if ((size_t)((chunk - in) + 12) > insize || chunk < in)
	{
		if (state->decoder.ignore_end)
			return (0);
		state->error = 30;
		return (0);
	}
	len = lodepng_chunk_length(chunk);
	if (len > 2147483647)
	{
		if (state->decoder.ignore_end)
			return (0);
		state->error = 63;
		return (0);
	}
	if ((size_t)((chunk - in) + len + 12) > insize
		|| (chunk + len + 12) < in)
	{
		state->error = 64;
		return (0);
	}
	return (1);
}

static void	dg_post_chunk(t_png_state *state,
		const unsigned char *chunk, unsigned int unknown, unsigned int cpos)
{
	if (!state->decoder.ignore_crc && !unknown)
	{
		if (lodepng_chunk_check_crc(chunk))
			state->error = 57;
	}
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	if (unknown && state->decoder.remember_unknown_chunks)
	{
		state->error = lodepng_chunk_append(
				&state->info_png.unknown_chunks_data[cpos - 1],
				&state->info_png.unknown_chunks_size[cpos - 1],
				chunk);
	}
#else
	(void)cpos;
#endif
}

static size_t	dg_predict_interlaced(unsigned int w, unsigned int h,
		const t_png_color_mode *color)
{
	size_t	predict;

	predict = 0;
	predict += lodepng_get_raw_size_idat((w + 7) >> 3,
			(h + 7) >> 3, color);
	if (w > 4)
		predict += lodepng_get_raw_size_idat((w + 3) >> 3,
				(h + 7) >> 3, color);
	predict += lodepng_get_raw_size_idat((w + 3) >> 2,
			(h + 3) >> 3, color);
	if (w > 2)
		predict += lodepng_get_raw_size_idat((w + 1) >> 2,
				(h + 3) >> 2, color);
	predict += lodepng_get_raw_size_idat((w + 1) >> 1,
			(h + 1) >> 2, color);
	if (w > 1)
		predict += lodepng_get_raw_size_idat(w >> 1,
				(h + 1) >> 1, color);
	predict += lodepng_get_raw_size_idat(w, h >> 1, color);
	return (predict);
}

void	dg_chunk_loop(t_png_state *state, ucvector *idat,
		const unsigned char *in, size_t insize)
{
	unsigned int			iend;
	unsigned int			unknown;
	unsigned int			cpos;
	const unsigned char	*chunk;

	iend = 0;
	cpos = 1;
	chunk = &in[33];
	while (!iend && !state->error)
	{
		if (!dg_validate_chunk(state, chunk, in, insize))
			break ;
		dg_handle_chunk(state, idat, chunk,
			&iend, &unknown, &cpos);
		if (state->error)
			break ;
		dg_post_chunk(state, chunk, unknown, cpos);
		if (!iend)
			chunk = lodepng_chunk_next_const(chunk);
	}
}

void	dg_decompress(t_png_state *state, unsigned char **out,
		ucvector *idat, unsigned int w, unsigned int h)
{
	ucvector	scanlines;
	size_t		predict;

	ucvector_init(&scanlines);
	if (state->info_png.interlace_method == 0)
		predict = lodepng_get_raw_size_idat(w, h,
				&state->info_png.color);
	else
		predict = dg_predict_interlaced(w, h,
				&state->info_png.color);
	if (!ucvector_reserve(&scanlines, predict))
		state->error = 83;
	if (!state->error)
	{
		state->error = zlib_decompress(&scanlines.data,
				&scanlines.size, idat->data, idat->size,
				&state->decoder.zlibsettings);
		if (!state->error && scanlines.size != predict)
			state->error = 91;
	}
	if (!state->error)
		dg_postprocess(state, out, &scanlines, w, h);
	ucvector_cleanup(&scanlines);
}
