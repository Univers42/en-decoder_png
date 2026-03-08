/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:05 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:51:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	remove_padding_bits(unsigned char *out, const unsigned char *in,
		size_t olinebits, size_t ilinebits, unsigned int h)
{
	unsigned int	y;
	size_t		diff;
	size_t		ibp;
	size_t		obp;
	size_t		x;

	diff = ilinebits - olinebits;
	ibp = 0;
	obp = 0;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < olinebits)
		{
			set_bit_of_rev_stream(&obp, out,
				read_bit_from_rev_stream(&ibp, in));
			++x;
		}
		ibp += diff;
		++y;
	}
}

void	add_padding_bits(unsigned char *out, const unsigned char *in,
		size_t olinebits, size_t ilinebits, unsigned int h)
{
	unsigned int	y;
	size_t		diff;
	size_t		obp;
	size_t		ibp;
	size_t		x;

	diff = olinebits - ilinebits;
	obp = 0;
	ibp = 0;
	y = 0;
	while (y != h)
	{
		x = 0;
		while (x < ilinebits)
		{
			set_bit_of_rev_stream(&obp, out,
				read_bit_from_rev_stream(&ibp, in));
			++x;
		}
		x = 0;
		while (x != diff)
		{
			set_bit_of_rev_stream(&obp, out, 0);
			++x;
		}
		++y;
	}
}
