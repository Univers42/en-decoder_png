/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:12:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	filter_zero(unsigned char *out, const unsigned char *in,
		unsigned int h, size_t lb, size_t bw)
{
	unsigned int		y;
	const unsigned char	*prevline;
	size_t				outindex;
	size_t				inindex;

	prevline = 0;
	y = 0;
	while (y != h)
	{
		outindex = (1 + lb) * y;
		inindex = lb * y;
		out[outindex] = 0;
		filter_scanline(&out[outindex + 1], &in[inindex],
			prevline, lb, bw, 0);
		prevline = &in[inindex];
		++y;
	}
	return (0);
}

unsigned int	filter_predef(unsigned char *out, const unsigned char *in,
		unsigned int h, size_t lb, size_t bw,
		const t_encoder_settings *settings)
{
	unsigned int		y;
	const unsigned char	*prevline;
	size_t				outindex;
	size_t				inindex;
	unsigned char		type;

	prevline = 0;
	y = 0;
	while (y != h)
	{
		outindex = (1 + lb) * y;
		inindex = lb * y;
		type = settings->predefined_filters[y];
		out[outindex] = type;
		filter_scanline(&out[outindex + 1], &in[inindex],
			prevline, lb, bw, type);
		prevline = &in[inindex];
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

static void	filter_ms_row(unsigned char *out, const unsigned char *in,
		const unsigned char *prevline, unsigned char **attempt,
		size_t lb, size_t bw, unsigned int y)
{
	unsigned char	type;
	unsigned char	best_type;
	size_t			smallest;
	size_t			sum;

	best_type = 0;
	smallest = 0;
	type = 0;
	while (type != 5)
	{
		filter_scanline(attempt[type], &in[y * lb],
			prevline, lb, bw, type);
		sum = filter_ms_score(attempt[type], lb, type);
		if (type == 0 || sum < smallest)
		{
			best_type = type;
			smallest = sum;
		}
		++type;
	}
	filter_copy_best(out, attempt, best_type, lb, y);
}

unsigned int	filter_minsum(unsigned char *out, const unsigned char *in,
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
		filter_ms_row(out, in, prevline, attempt, lb, bw, y);
		prevline = &in[y * lb];
		++y;
	}
	filter_free(attempt);
	return (0);
}
