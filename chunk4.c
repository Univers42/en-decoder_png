/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 20:04:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lodepng_chunk_append(unsigned char **out,
			size_t *outlength, const unsigned char *chunk)
{
	unsigned int		i;
	unsigned int		total_chunk_length;
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

unsigned int	lodepng_chunk_create(unsigned char **out,
			size_t *outlength, unsigned int length,
			const char *type, const unsigned char *data)
{
	unsigned int		i;
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
	lodepng_set_32bit_int(chunk, (unsigned int)length);
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

unsigned int	add_chunk(ucvector *out, const char *chunk_name,
			const unsigned char *data, size_t length)
{
	CERROR_TRY_RETURN(lodepng_chunk_create(&out->data,
			&out->size, (unsigned int)length, chunk_name, data));
	out->allocsize = out->size;
	return (0);
}

unsigned int	add_chunk_iend(ucvector *out)
{
	return (add_chunk(out, "IEND", 0, 0));
}

unsigned int	add_chunk_ihdr(ucvector *out, unsigned int w, unsigned int h,
			t_png_color_type colortype, unsigned int bitdepth,
			unsigned interlace_method)
{
	unsigned int	error;
	ucvector	header;

	ucvector_init(&header);
	lodepng_add_32bit_int(&header, w);
	lodepng_add_32bit_int(&header, h);
	ucvector_push_back(&header, (unsigned char)bitdepth);
	ucvector_push_back(&header, (unsigned char)colortype);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, interlace_method);
	error = add_chunk(out, "IHDR", header.data, header.size);
	ucvector_cleanup(&header);
	return (error);
}
