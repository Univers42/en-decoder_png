/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:15:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static size_t	brute_try(unsigned char **attempt,
		const unsigned char *in, const unsigned char *prevline,
		size_t lb, size_t bw, unsigned int y, unsigned char type,
		const t_compress_settings *zs)
{
	unsigned char	*dummy;
	size_t			sz;

	filter_scanline(attempt[type], &in[y * lb],
		prevline, lb, bw, type);
	sz = 0;
	dummy = 0;
	zlib_compress(&dummy, &sz, attempt[type],
		(unsigned int)lb, zs);
	lodepng_free(dummy);
	return (sz);
}

static void	filter_brute_row(unsigned char *out,
		const unsigned char *in, const unsigned char *prevline,
		unsigned char **attempt, size_t lb, size_t bw,
		unsigned int y, const t_compress_settings *zs)
{
	unsigned char	type;
	unsigned char	best_type;
	size_t			smallest;
	size_t			sz;

	best_type = 0;
	smallest = 0;
	type = 0;
	while (type != 5)
	{
		sz = brute_try(attempt, in, prevline,
			lb, bw, y, type, zs);
		if (type == 0 || sz < smallest)
		{
			best_type = type;
			smallest = sz;
		}
		++type;
	}
	filter_copy_best(out, attempt, best_type, lb, y);
}

static unsigned int	filter_brute(unsigned char *out, const unsigned char *in,
		unsigned int h, size_t lb, size_t bw,
		const t_encoder_settings *settings)
{
	unsigned char			*attempt[5];
	const unsigned char		*prevline;
	unsigned int				y;
	t_compress_settings	zs;
	unsigned int				error;

	zs = settings->zlibsettings;
	zs.btype = 1;
	zs.custom_zlib = 0;
	zs.custom_deflate = 0;
	error = filter_alloc(attempt, lb);
	if (error)
		return (error);
	prevline = 0;
	y = 0;
	while (y != h)
	{
		filter_brute_row(out, in, prevline, attempt,
			lb, bw, y, &zs);
		prevline = &in[y * lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}

unsigned int	filter(unsigned char *out, const unsigned char *in,
		unsigned int w, unsigned int h, const t_png_color_mode *info,
		const t_encoder_settings *settings)
{
	unsigned int				bpp;
	size_t					lb;
	size_t					bw;
	t_filter_strategy	strat;

	bpp = lodepng_get_bpp(info);
	if (bpp == 0)
		return (31);
	lb = (w * bpp + 7) / 8;
	bw = (bpp + 7) / 8;
	strat = settings->filter_strategy;
	if (settings->filter_palette_zero
		&& (info->colortype == LCT_PALETTE || info->bitdepth < 8))
		strat = LFS_ZERO;
	if (strat == LFS_ZERO)
		return (filter_zero(out, in, h, lb, bw));
	if (strat == LFS_MINSUM)
		return (filter_minsum(out, in, h, lb, bw));
	if (strat == LFS_ENTROPY)
		return (filter_entropy(out, in, h, lb, bw));
	if (strat == LFS_PREDEFINED)
		return (filter_predef(out, in, h, lb, bw, settings));
	if (strat == LFS_BRUTE_FORCE)
		return (filter_brute(out, in, h, lb, bw, settings));
	return (88);
}
