/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:05 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	remove_padding_bits(unsigned char *out, const unsigned char *in,
		const size_t *linebits, unsigned int h)
{
	unsigned int	y;
	size_t			diff;
	size_t			ibp;
	size_t			obp;
	size_t			x;

	diff = linebits[1] - linebits[0];
	ibp = 0;
	obp = 0;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < linebits[0])
		{
			set_bit_of_rev_stream(&obp, out,
				read_bit_from_rev_stream(&ibp, in));
			++x;
		}
		ibp += diff;
		++y;
	}
}

static void	pad_zeros(size_t *obp, unsigned char *out, size_t diff)
{
	size_t	x;

	x = 0;
	while (x != diff)
	{
		set_bit_of_rev_stream(obp, out, 0);
		++x;
	}
}

void	add_padding_bits(unsigned char *out, const unsigned char *in,
		const size_t *linebits, unsigned int h)
{
	unsigned int	y;
	size_t			obp;
	size_t			ibp;
	size_t			x;

	obp = 0;
	ibp = 0;
	y = 0;
	while (y != h)
	{
		x = 0;
		while (x < linebits[1])
		{
			set_bit_of_rev_stream(&obp, out,
				read_bit_from_rev_stream(&ibp, in));
			++x;
		}
		pad_zeros(&obp, out, linebits[0] - linebits[1]);
		++y;
	}
}
