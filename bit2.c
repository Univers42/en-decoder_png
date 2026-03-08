/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:16:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	addBitsToStreamReversed(size_t *bitpointer, ucvector *bitstream,
		unsigned int value, size_t nbits)
{
	size_t	i;

	i = 0;
	while (i != nbits)
	{
		addBitToStream(bitstream, bitpointer,
			(unsigned char)((value >> (nbits - 1 - i)) & 1));
		i++;
	}
}

unsigned char	readBit(size_t bitpointer, const unsigned char *bitstream)
{
	return ((bitstream[bitpointer >> 3] >> (bitpointer & 0x7))
		& (unsigned char)1);
}

unsigned char	readBitFromStream(size_t *bitpointer,
		const unsigned char *bitstream)
{
	unsigned char	result;

	result = (unsigned char)(READBIT(*bitpointer, bitstream));
	++(*bitpointer);
	return (result);
}

unsigned int	readBitsFromStream(size_t *bitpointer,
		const unsigned char *bitstream, size_t nbits)
{
	unsigned int	result;
	unsigned int	i;

	result = 0;
	i = 0;
	while (i != nbits)
	{
		result += ((unsigned int)READBIT(*bitpointer, bitstream)) << i;
		++(*bitpointer);
		i++;
	}
	return (result);
}

unsigned char	readBitFromReversedStream(size_t *bitpointer,
		const unsigned char *bitstream)
{
	unsigned char	result;

	result = (unsigned char)((bitstream[(*bitpointer) >> 3]
				>> (7 - ((*bitpointer) & 0x7))) & 1);
	++(*bitpointer);
	return (result);
}
