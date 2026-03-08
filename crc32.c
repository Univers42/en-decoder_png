/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crc32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:15:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	crc32_process_byte(unsigned int *crc, unsigned char byte)
{
	int	k;

	*crc ^= byte;
	k = 0;
	while (k < 8)
	{
		*crc = (*crc >> 1) ^ (0xedb88320u & (-(int)(*crc & 1)));
		k++;
	}
}

unsigned int	lodepng_crc32(const unsigned char *data, size_t length)
{
	unsigned int	crc;
	size_t			i;

	crc = 0xffffffffu;
	i = 0;
	while (i < length)
	{
		crc32_process_byte(&crc, data[i]);
		i++;
	}
	return (crc ^ 0xffffffffu);
}
