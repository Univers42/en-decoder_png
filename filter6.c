/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static size_t	bt_compress(unsigned char *data, size_t lb,
		const t_compress_settings *zs)
{
	t_ucvector	tmp;

	ucvector_init(&tmp);
	zlib_compress(&tmp, data, (unsigned int)lb, zs);
	ucvector_cleanup(&tmp);
	return (tmp.size);
}

static void	bt_scan(unsigned char **attempt, t_filter_ctx *f,
		const unsigned char *prevline, unsigned int y)
{
	t_filter_dim	d;
	unsigned char	type;

	d = (t_filter_dim){f->bw, f->lb, 0};
	type = 0;
	while (type != 5)
	{
		d.type = type;
		filter_scanline(attempt[type], &f->in[y * f->lb],
			prevline, &d);
		++type;
	}
}

static unsigned char	bt_best(unsigned char **attempt, size_t lb,
		const t_compress_settings *zs)
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
		sz = bt_compress(attempt[type], lb, zs);
		if (type == 0 || sz < smallest)
		{
			best_type = type;
			smallest = sz;
		}
		++type;
	}
	return (best_type);
}

unsigned int	filter_brute(t_filter_ctx *f,
		const t_encoder_settings *settings)
{
	unsigned char		*attempt[5];
	const unsigned char	*prevline;
	unsigned int		y;
	t_compress_settings	zs;
	unsigned int		error;

	zs = settings->zlibsettings;
	zs.btype = 1;
	error = filter_alloc(attempt, f->lb);
	if (error)
		return (error);
	prevline = 0;
	y = 0;
	while (y != f->h)
	{
		bt_scan(attempt, f, prevline, y);
		error = bt_best(attempt, f->lb, &zs);
		filter_copy_best(&f->out[y * (f->lb + 1)],
			attempt, error, f->lb);
		prevline = &f->in[y * f->lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}

unsigned int	filter(t_filter_ctx *f, const t_png_color_mode *info,
		const t_encoder_settings *settings)
{
	t_filter_strategy	strat;

	if (f->bw == 0)
		return (31);
	strat = settings->filter_strategy;
	if (settings->filter_palette_zero
		&& (info->colortype == LCT_PALETTE || info->bitdepth < 8))
		strat = LFS_ZERO;
	if (strat == LFS_ZERO)
		return (filter_zero(f));
	if (strat == LFS_MINSUM)
		return (filter_minsum(f));
	if (strat == LFS_ENTROPY)
		return (filter_entropy(f));
	if (strat == LFS_PREDEFINED)
		return (filter_predef(f, settings));
	if (strat == LFS_BRUTE_FORCE)
		return (filter_brute(f, settings));
	return (88);
}
