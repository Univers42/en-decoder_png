/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	is_png_signature(const unsigned char *chunk)
{
	return (chunk[0] == 0x89 && chunk[1] == 0x50
		&& chunk[2] == 0x4e && chunk[3] == 0x47
		&& chunk[4] == 0x0d && chunk[5] == 0x0a
		&& chunk[6] == 0x1a && chunk[7] == 0x0a);
}

unsigned char	*lodepng_chunk_next(unsigned char *chunk)
{
	unsigned	total_chunk_length;

	if (is_png_signature(chunk))
		return (chunk + 8);
	total_chunk_length = lodepng_chunk_length(chunk) + 12;
	return (chunk + total_chunk_length);
}

const unsigned char	*lodepng_chunk_next_const(const unsigned char *chunk)
{
	unsigned	total_chunk_length;

	if (is_png_signature(chunk))
		return (chunk + 8);
	total_chunk_length = lodepng_chunk_length(chunk) + 12;
	return (chunk + total_chunk_length);
}

unsigned char	*lodepng_chunk_find(unsigned char *chunk,
				const unsigned char *end, const char type[5])
{
	while (1)
	{
		if (chunk + 12 >= end)
			return (0);
		if (lodepng_chunk_type_equals(chunk, type))
			return (chunk);
		chunk = lodepng_chunk_next(chunk);
	}
}

const unsigned char	*lodepng_chunk_find_const(const unsigned char *chunk,
				const unsigned char *end, const char type[5])
{
	while (1)
	{
		if (chunk + 12 >= end)
			return (0);
		if (lodepng_chunk_type_equals(chunk, type))
			return (chunk);
		chunk = lodepng_chunk_next_const(chunk);
	}
}
