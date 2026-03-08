/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   palette.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:05:04 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 00:05:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int get_palette_translucency(const unsigned char *palette, size_t palettesize)
{
	size_t i;
	unsigned int key = 0;
	unsigned int r = 0, g = 0, b = 0;
	for (i = 0; i != palettesize; ++i)
	{
		if (!key && palette[4 * i + 3] == 0)
		{
			r = palette[4 * i + 0];
			g = palette[4 * i + 1];
			b = palette[4 * i + 2];
			key = 1;
			i = (size_t)(-1);
		}
		else if (palette[4 * i + 3] != 255)
			return 2;
		else if (key && r == palette[i * 4 + 0] && g == palette[i * 4 + 1] && b == palette[i * 4 + 2])
			return 2;
	}
	return key;
}

void	add_length_distance(uivector *out, unsigned int length, unsigned int distance)
{
	unsigned int	length_code;
	unsigned int	distance_code;
	unsigned int	len_sym;
	unsigned int	len_val;

	length_code = length - 3;
	distance_code = distance - 1;
	if (length_code > 279)
		len_sym = length_code;
	else
		len_sym = length_code + 257;
	if (length > 258)
		len_val = 258;
	else
		len_val = length;
	uivector_push_back(out, len_sym);
	uivector_push_back(out, len_val);
	uivector_push_back(out, distance_code);
}

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
unsigned int	add_unknown_chunks(ucvector *out, unsigned char *data,
		size_t datasize)
{
	unsigned char *inchunk = data;
	while ((size_t)(inchunk - data) < datasize)
	{
		CERROR_TRY_RETURN(lodepng_chunk_append(&out->data, &out->size, inchunk));
		out->allocsize = out->size;
		inchunk = lodepng_chunk_next(inchunk);
	}
	return 0;
}
#endif