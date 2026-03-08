/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	lodepng_chunk_append(unsigned char **out,
			size_t *outlength, const unsigned char *chunk)
{
	unsigned		i;
	unsigned		total_chunk_length;
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

unsigned	lodepng_chunk_create(unsigned char **out,
			size_t *outlength, unsigned length,
			const char *type, const unsigned char *data)
{
	unsigned		i;
	unsigned char	*chunk;
	unsigned char	*new_buffer;
	size_t			new_length;

	new_length = (*outlength) + length + 12;
	if (new_length < length + 12 || new_length < (*outlength))
		return (77);
	new_buffer = (unsigned char *)lodepng_realloc(*out, new_length);
	if (!new_buffer)
		return (83);
	(*out) = new_buffer;
	(*outlength) = new_length;
	chunk = &(*out)[(*outlength) - length - 12];
	lodepng_set32bitInt(chunk, (unsigned)length);
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

unsigned	addChunk(ucvector *out, const char *chunkName,
			const unsigned char *data, size_t length)
{
	CERROR_TRY_RETURN(lodepng_chunk_create(&out->data,
			&out->size, (unsigned)length, chunkName, data));
	out->allocsize = out->size;
	return (0);
}

unsigned	addChunk_IEND(ucvector *out)
{
	return (addChunk(out, "IEND", 0, 0));
}

unsigned	addChunk_IHDR(ucvector *out, unsigned w, unsigned h,
			LodePNGColorType colortype, unsigned bitdepth,
			unsigned interlace_method)
{
	unsigned	error;
	ucvector	header;

	ucvector_init(&header);
	lodepng_add32bitInt(&header, w);
	lodepng_add32bitInt(&header, h);
	ucvector_push_back(&header, (unsigned char)bitdepth);
	ucvector_push_back(&header, (unsigned char)colortype);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, interlace_method);
	error = addChunk(out, "IHDR", header.data, header.size);
	ucvector_cleanup(&header);
	return (error);
}
