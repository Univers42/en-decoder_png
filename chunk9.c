/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

static unsigned	itxt_fill_header(ucvector *data, const char *keyword,
				unsigned compressed, const char *langtag,
				const char *transkey)
{
	size_t	i;

	i = 0;
	while (keyword[i] != 0)
		ucvector_push_back(data, (unsigned char)keyword[i++]);
	if (i < 1 || i > 79)
		return (89);
	ucvector_push_back(data, 0);
	if (compressed)
		ucvector_push_back(data, 1);
	else
		ucvector_push_back(data, 0);
	ucvector_push_back(data, 0);
	i = 0;
	while (langtag[i] != 0)
		ucvector_push_back(data, (unsigned char)langtag[i++]);
	ucvector_push_back(data, 0);
	i = 0;
	while (transkey[i] != 0)
		ucvector_push_back(data, (unsigned char)transkey[i++]);
	ucvector_push_back(data, 0);
	return (0);
}

static unsigned	itxt_write_body(ucvector *data, unsigned compressed,
				const char *textstring,
				LodePNGCompressSettings *zlibsettings)
{
	ucvector	comp;
	unsigned	error;
	size_t		i;

	if (!compressed)
	{
		i = 0;
		while (textstring[i] != 0)
			ucvector_push_back(data, (unsigned char)textstring[i++]);
		return (0);
	}
	ucvector_init(&comp);
	error = zlib_compress(&comp.data, &comp.size,
			(unsigned char *)textstring, strlen(textstring), zlibsettings);
	if (!error)
	{
		i = 0;
		while (i != comp.size)
			ucvector_push_back(data, comp.data[i++]);
	}
	ucvector_cleanup(&comp);
	return (error);
}

unsigned	addChunk_iTXt(ucvector *out, unsigned compressed,
			const char *keyword, const char *langtag,
			const char *transkey, const char *textstring,
			LodePNGCompressSettings *zlibsettings)
{
	unsigned	error;
	ucvector	data;

	ucvector_init(&data);
	error = itxt_fill_header(&data, keyword, compressed, langtag, transkey);
	if (error)
		return (error);
	error = itxt_write_body(&data, compressed, textstring, zlibsettings);
	if (!error)
		error = addChunk(out, "iTXt", data.data, data.size);
	ucvector_cleanup(&data);
	return (error);
}

#endif
