/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:16:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	read_bits_from_rev_stream(size_t *bitpointer,
		const unsigned char *bitstream, size_t nbits)
{
	unsigned int	result;
	size_t			i;

	result = 0;
	i = 0;
	while (i < nbits)
	{
		result <<= 1;
		result |= (unsigned int)read_bit_from_rev_stream(bitpointer,
				bitstream);
		i++;
	}
	return (result);
}

void	set_bit_of_rev_stream0(size_t *bitpointer,
		unsigned char *bitstream, unsigned char bit)
{
	if (bit)
		bitstream[(*bitpointer) >> 3] |= (bit
				<< (7 - ((*bitpointer) & 0x7)));
	++(*bitpointer);
}

void	set_bit_of_rev_stream(size_t *bitpointer,
		unsigned char *bitstream, unsigned char bit)
{
	if (bit == 0)
		bitstream[(*bitpointer) >> 3] &= (unsigned char)(~(1
					<< (7 - ((*bitpointer) & 0x7))));
	else
		bitstream[(*bitpointer) >> 3] |= (1
				<< (7 - ((*bitpointer) & 0x7)));
	++(*bitpointer);
}

unsigned int	lodepng_chunk_length(const unsigned char *chunk)
{
	return (lodepng_read_32bit_int(&chunk[0]));
}

unsigned int	get_value_required_bits(unsigned char value)
{
	if (value == 0 || value == 255)
		return (1);
	if (value % 17 == 0)
	{
		if (value % 85 == 0)
			return (2);
		return (4);
	}
	return (8);
}
