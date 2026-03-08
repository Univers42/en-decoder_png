/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:17:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	gethash_small(const unsigned char *data,
		size_t size, size_t pos)
{
	unsigned int	result;
	size_t			amount;
	size_t			i;

	result = 0;
	if (pos >= size)
		return (0);
	amount = size - pos;
	i = 0;
	while (i != amount)
	{
		result ^= (unsigned int)(data[pos + i] << (i * 8u));
		i++;
	}
	return (result);
}

unsigned int	getHash(const unsigned char *data, size_t size, size_t pos)
{
	unsigned int	result;

	result = 0;
	if (pos + 2 < size)
	{
		result ^= (unsigned int)(data[pos + 0] << 0u);
		result ^= (unsigned int)(data[pos + 1] << 4u);
		result ^= (unsigned int)(data[pos + 2] << 8u);
	}
	else
		result = gethash_small(data, size, pos);
	return (result & HASH_BIT_MASK);
}

unsigned int	countZeros(const unsigned char *data, size_t size,
		size_t pos)
{
	const unsigned char	*start;
	const unsigned char	*end;

	start = data + pos;
	end = start + MAX_SUPPORTED_DEFLATE_LENGTH;
	if (end > data + size)
		end = data + size;
	data = start;
	while (data != end && *data == 0)
		++data;
	return ((unsigned int)(data - start));
}
