/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned char	*lodepng_chunk_data(unsigned char *chunk)
{
	return (&chunk[8]);
}

const unsigned char	*lodepng_chunk_data_const(const unsigned char *chunk)
{
	return (&chunk[8]);
}

unsigned int	lodepng_chunk_check_crc(const unsigned char *chunk)
{
	unsigned int	length;
	unsigned int	crc;
	unsigned int	checksum;

	length = lodepng_chunk_length(chunk);
	crc = lodepng_read_32bit_int(&chunk[length + 8]);
	checksum = lodepng_crc32(&chunk[4], length + 4);
	if (crc != checksum)
		return (1);
	return (0);
}

void	lodepng_chunk_generate_crc(unsigned char *chunk)
{
	unsigned int	length;
	unsigned int	crc;

	length = lodepng_chunk_length(chunk);
	crc = lodepng_crc32(&chunk[4], length + 4);
	lodepng_set_32bit_int(chunk + 8 + length, crc);
}
