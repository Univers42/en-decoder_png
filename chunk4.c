/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_chunk_append(unsigned char **out,
			size_t *outlength, const unsigned char *chunk)
{
	unsigned int	i;
	unsigned int	total_chunk_length;
	unsigned char	*chunk_start;
	unsigned char	*new_buffer;
	size_t			new_length;

	total_chunk_length = lodepng_chunk_length(chunk) + 12;
	new_length = (*outlength) + total_chunk_length;
	if (new_length < total_chunk_length || new_length < (*outlength))
		return (77);
	new_buffer = (unsigned char *)lodepng_realloc(*out, new_length);
	if (!new_buffer)
		return (83);
	(*out) = new_buffer;
	(*outlength) = new_length;
	chunk_start = &(*out)[new_length - total_chunk_length];
	i = 0;
	while (i != total_chunk_length)
	{
		chunk_start[i] = chunk[i];
		++i;
	}
	return (0);
}

static unsigned int	cc_realloc(t_ucvector *out, unsigned int length)
{
	unsigned char	*buf;
	size_t			new_len;

	new_len = out->size + length + 12;
	if (new_len < length + 12 || new_len < out->size)
		return (77);
	buf = (unsigned char *)lodepng_realloc(out->data, new_len);
	if (!buf)
		return (83);
	out->data = buf;
	out->size = new_len;
	out->allocsize = new_len;
	return (0);
}

unsigned int	lodepng_chunk_create(t_ucvector *out,
			unsigned int length, const char *type,
			const unsigned char *data)
{
	unsigned int	error;
	unsigned int	i;
	unsigned char	*chunk;

	error = cc_realloc(out, length);
	if (error)
		return (error);
	chunk = &out->data[out->size - length - 12];
	lodepng_set_32bit_int(chunk, length);
	chunk[4] = (unsigned char)type[0];
	chunk[5] = (unsigned char)type[1];
	chunk[6] = (unsigned char)type[2];
	chunk[7] = (unsigned char)type[3];
	i = 0;
	while (i != length)
	{
		chunk[8 + i] = data[i];
		++i;
	}
	lodepng_chunk_generate_crc(chunk);
	return (0);
}

unsigned int	add_chunk(t_ucvector *out, const char *chunk_name,
			const unsigned char *data, size_t length)
{
	return (lodepng_chunk_create(out,
			(unsigned int)length, chunk_name, data));
}

unsigned int	add_chunk_iend(t_ucvector *out)
{
	return (add_chunk(out, "IEND", 0, 0));
}
