/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	unfilter(unsigned char *out, const unsigned char *in,
		unsigned int h, t_filter_dim *d)
{
	unsigned int	y;
	unsigned int	error;
	unsigned char	*prevline;

	prevline = 0;
	y = 0;
	while (y < h)
	{
		d->type = in[(1 + d->len) * y];
		error = unfilter_scanline(&out[d->len * y],
				&in[(1 + d->len) * y + 1], prevline, d);
		if (error)
			return (error);
		prevline = &out[d->len * y];
		++y;
	}
	return (0);
}

float	flog2(float f)
{
	float	result;

	result = 0;
	while (f > 32)
	{
		result += 4;
		f /= 16;
	}
	while (f > 2)
	{
		++result;
		f /= 2;
	}
	return (result + 1.442695f
		* (f * f * f / 3 - 3 * f * f / 2 + 3 * f - 1.83333f));
}

unsigned int	filter_alloc(unsigned char **attempt, size_t lb)
{
	unsigned int	type;

	type = 0;
	while (type != 5)
	{
		attempt[type] = (unsigned char *)lodepng_malloc(lb);
		if (!attempt[type])
			return (83);
		++type;
	}
	return (0);
}

void	filter_free(unsigned char **attempt)
{
	unsigned int	type;

	type = 0;
	while (type != 5)
	{
		lodepng_free(attempt[type]);
		++type;
	}
}

void	filter_copy_best(unsigned char *out_row,
		unsigned char **attempt, unsigned char best_type, size_t lb)
{
	size_t	x;

	out_row[0] = best_type;
	x = 0;
	while (x != lb)
	{
		out_row[1 + x] = attempt[best_type][x];
		++x;
	}
}
