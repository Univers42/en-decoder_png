/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:54:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	ent_count_init(unsigned int *count, unsigned char *data,
		size_t lb, unsigned char type)
{
	size_t	x;

	x = 0;
	while (x != 256)
	{
		count[x] = 0;
		++x;
	}
	x = 0;
	while (x != lb)
	{
		++count[data[x]];
		++x;
	}
	++count[type];
}

static float	ent_score(unsigned int *count, size_t lb)
{
	size_t	x;
	float	p;
	float	sum;

	sum = 0;
	x = 0;
	while (x != 256)
	{
		if (count[x] != 0)
		{
			p = count[x] / (float)(lb + 1);
			sum += flog2(1 / p) * p;
		}
		++x;
	}
	return (sum);
}

static void	fer_try(unsigned char **attempt, unsigned int y,
		t_filter_ctx *f, const unsigned char *prev)
{
	t_filter_dim	d;
	unsigned char	type;

	d = (t_filter_dim){f->bw, f->lb, 0};
	type = 0;
	while (type != 5)
	{
		d.type = type;
		filter_scanline(attempt[type], &f->in[y * f->lb],
			prev, &d);
		++type;
	}
}

static void	filter_ent_row(t_filter_ctx *f,
		const unsigned char *prevline,
		unsigned char **attempt, unsigned int y)
{
	unsigned char	type;
	unsigned char	best_type;
	float			smallest;
	unsigned int	count[256];

	fer_try(attempt, y, f, prevline);
	best_type = 0;
	smallest = 0;
	type = 0;
	while (type != 5)
	{
		ent_count_init(count, attempt[type], f->lb, type);
		if (type == 0 || ent_score(count, f->lb) < smallest)
		{
			best_type = type;
			smallest = ent_score(count, f->lb);
		}
		++type;
	}
	filter_copy_best(&f->out[y * (f->lb + 1)],
		attempt, best_type, f->lb);
}

unsigned int	filter_entropy(t_filter_ctx *f)
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
		filter_ent_row(f, prevline, attempt, y);
		prevline = &f->in[y * f->lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}
