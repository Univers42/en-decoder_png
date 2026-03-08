/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:16:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_read32bitInt(const unsigned char *buffer)
{
	return ((unsigned int)((buffer[0] << 24) | (buffer[1] << 16)
			| (buffer[2] << 8) | buffer[3]));
}

void	lodepng_set32bitInt(unsigned char *buffer, unsigned int value)
{
	buffer[0] = (unsigned char)((value >> 24) & 0xff);
	buffer[1] = (unsigned char)((value >> 16) & 0xff);
	buffer[2] = (unsigned char)((value >> 8) & 0xff);
	buffer[3] = (unsigned char)((value) & 0xff);
}

void	lodepng_add32bitInt(ucvector *buffer, unsigned int value)
{
	ucvector_resize(buffer, buffer->size + 4);
	lodepng_set32bitInt(&buffer->data[buffer->size - 4], value);
}

void	addBitToStream(ucvector *bitstream, size_t *bitpointer,
		unsigned int bit)
{
	if (((*bitpointer) & 7) == 0)
		ucvector_push_back(bitstream, (unsigned char)0);
	bitstream->data[bitstream->size - 1] |= (bit << ((*bitpointer) & 0x7));
	++(*bitpointer);
}

void	addBitsToStream(size_t *bitpointer, ucvector *bitstream,
		unsigned int value, size_t nbits)
{
	size_t	i;

	i = 0;
	while (i != nbits)
	{
		addBitToStream(bitstream, bitpointer,
			(unsigned char)((value >> i) & 1));
		i++;
	}
}
