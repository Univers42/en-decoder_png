/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 02:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

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

static void	dg_zlib_scan(t_png_state *state, t_ucvector *scan,
		t_ucvector *idat, size_t predict)
{
	if (!ucvector_reserve(scan, predict))
		state->error = 83;
	if (!state->error)
	{
		state->error = zlib_decompress(scan,
				idat->data, idat->size,
				&state->decoder.zlibsettings);
		if (!state->error && scan->size != predict)
			state->error = 91;
	}
}

void	dg_decompress(t_png_state *state, unsigned char **out,
		t_ucvector *idat)
{
	t_ucvector		scanlines;
	size_t			predict;
	unsigned int	w;
	unsigned int	h;

	w = state->info_png.width;
	h = state->info_png.height;
	ucvector_init(&scanlines);
	if (state->info_png.interlace_method == 0)
		predict = lodepng_get_raw_size_idat(w, h,
				&state->info_png.color);
	else
		predict = dg_predict_interlaced(w, h,
				&state->info_png.color);
	dg_zlib_scan(state, &scanlines, idat, predict);
	if (!state->error)
		dg_postprocess(state, out, &scanlines);
	ucvector_cleanup(&scanlines);
}
