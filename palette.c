/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   palette.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:05:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:03:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	get_palette_translucency(
				const unsigned char *palette,
				size_t palettesize)
{
	size_t			i;
	unsigned int	key;
	unsigned int	rgb[3];

	key = 0;
	i = 0;
	while (i != palettesize)
	{
		if (!key && palette[4 * i + 3] == 0)
		{
			rgb[0] = palette[4 * i];
			rgb[1] = palette[4 * i + 1];
			rgb[2] = palette[4 * i + 2];
			key = 1;
			i = (size_t)(-1);
		}
		else if (palette[4 * i + 3] != 255)
			return (2);
		else if (key && rgb[0] == palette[i * 4]
			&& rgb[1] == palette[i * 4 + 1]
			&& rgb[2] == palette[i * 4 + 2])
			return (2);
		i++;
	}
	return (key);
}

void	add_length_distance(t_uivector *out,
			unsigned int length,
			unsigned int distance)
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

unsigned int	add_unknown_chunks(t_ucvector *out,
				unsigned char *data, size_t datasize)
{
	unsigned char	*inchunk;
	unsigned int	error;

	inchunk = data;
	while ((size_t)(inchunk - data) < datasize)
	{
		error = lodepng_chunk_append(
				&out->data, &out->size, inchunk);
		if (error)
			return (error);
		out->allocsize = out->size;
		inchunk = lodepng_chunk_next(inchunk);
	}
	return (0);
}

#endif
