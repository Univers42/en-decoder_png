/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:12:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	filter_zero(t_filter_ctx *f)
{
	unsigned int		y;
	const unsigned char	*prevline;
	t_filter_dim		d;

	d = (t_filter_dim){f->bw, f->lb, 0};
	prevline = 0;
	y = 0;
	while (y != f->h)
	{
		f->out[(1 + f->lb) * y] = 0;
		filter_scanline(&f->out[(1 + f->lb) * y + 1],
			&f->in[f->lb * y], prevline, &d);
		prevline = &f->in[f->lb * y];
		++y;
	}
	return (0);
}

unsigned int	filter_predef(t_filter_ctx *f,
		const t_encoder_settings *settings)
{
	unsigned int		y;
	const unsigned char	*prevline;
	t_filter_dim		d;

	d = (t_filter_dim){f->bw, f->lb, 0};
	prevline = 0;
	y = 0;
	while (y != f->h)
	{
		d.type = settings->predefined_filters[y];
		f->out[(1 + f->lb) * y] = d.type;
		filter_scanline(&f->out[(1 + f->lb) * y + 1],
			&f->in[f->lb * y], prevline, &d);
		prevline = &f->in[f->lb * y];
		++y;
	}
	return (0);
}

static size_t	filter_ms_score(const unsigned char *data,
		size_t lb, unsigned char type)
{
	size_t	sum;
	size_t	x;

	sum = 0;
	x = 0;
	while (x != lb)
	{
		if (type == 0 || data[x] < 128)
			sum += data[x];
		else
			sum += (255U - data[x]);
		++x;
	}
	return (sum);
}

static void	filter_ms_row(t_filter_ctx *f,
		const unsigned char *prevline,
		unsigned char **attempt, unsigned int y)
{
	unsigned char	type;
	unsigned char	best_type;
	size_t			smallest;
	t_filter_dim	d;
	size_t			score;

	d = (t_filter_dim){f->bw, f->lb, 0};
	best_type = 0;
	smallest = 0;
	type = 0;
	while (type != 5)
	{
		d.type = type;
		filter_scanline(attempt[type], &f->in[y * f->lb],
			prevline, &d);
		score = filter_ms_score(attempt[type], f->lb, type);
		if (type == 0 || score < smallest)
		{
			best_type = type;
			smallest = score;
		}
		++type;
	}
	filter_copy_best(&f->out[y * (f->lb + 1)],
		attempt, best_type, f->lb);
}

unsigned int	filter_minsum(t_filter_ctx *f)
{
	unsigned char		*attempt[5];
	const unsigned char	*prevline;
	unsigned int		y;
	unsigned int		error;

	error = filter_alloc(attempt, f->lb);
	if (error)
		return (error);
	prevline = 0;
	y = 0;
	while (y != f->h)
	{
		filter_ms_row(f, prevline, attempt, y);
		prevline = &f->in[y * f->lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}
