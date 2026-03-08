/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:13:00 by dlesieur         ###   ########.fr       */
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

static void	ent_compute_sum(float *sum, unsigned int *count, size_t lb)
{
	size_t	x;
	float	p;

	*sum = 0;
	x = 0;
	while (x != 256)
	{
		if (count[x] != 0)
		{
			p = count[x] / (float)(lb + 1);
			*sum += flog2(1 / p) * p;
		}
		++x;
	}
}

static void	filter_ent_row(unsigned char *out, const unsigned char *in,
		const unsigned char *prevline, unsigned char **attempt,
		size_t lb, size_t bw, unsigned int y)
{
	unsigned char	type;
	unsigned char	best_type;
	float			smallest;
	float			sum;
	unsigned int	count[256];

	best_type = 0;
	smallest = 0;
	type = 0;
	while (type != 5)
	{
		filter_scanline(attempt[type], &in[y * lb],
			prevline, lb, bw, type);
		ent_count_init(count, attempt[type], lb, type);
		ent_compute_sum(&sum, count, lb);
		if (type == 0 || sum < smallest)
		{
			best_type = type;
			smallest = sum;
		}
		++type;
	}
	filter_copy_best(out, attempt, best_type, lb, y);
}

unsigned int	filter_entropy(unsigned char *out, const unsigned char *in,
		unsigned int h, size_t lb, size_t bw)
{
	unsigned char		*attempt[5];
	const unsigned char	*prevline;
	unsigned int		y;
	unsigned int		error;

	error = filter_alloc(attempt, lb);
	if (error)
		return (error);
	prevline = 0;
	y = 0;
	while (y != h)
	{
		filter_ent_row(out, in, prevline, attempt, lb, bw, y);
		prevline = &in[y * lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}
