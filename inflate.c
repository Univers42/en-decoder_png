/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:15:46 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:51:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	inflateNoCompression(ucvector *out, const unsigned char *in,
			size_t *bp, size_t *pos, size_t inlength)
{
	size_t		p;
	unsigned	len;
	unsigned	nlen;
	unsigned	n;

	while (((*bp) & 0x7) != 0)
		++(*bp);
	p = (*bp) / 8;
	if (p + 4 >= inlength)
		return (52);
	len = in[p] + 256u * in[p + 1];
	p += 2;
	nlen = in[p] + 256u * in[p + 1];
	p += 2;
	if (len + nlen != 65535)
		return (21);
	if (!ucvector_resize(out, (*pos) + len))
		return (83);
	if (p + len > inlength)
		return (23);
	n = 0;
	while (n < len)
	{
		out->data[(*pos)++] = in[p++];
		++n;
	}
	(*bp) = p * 8;
	return (0);
}

unsigned	lodepng_inflatev(ucvector *out, const unsigned char *in,
			size_t insize, const LodePNGDecompressSettings *settings)
{
	size_t		bp;
	unsigned	bfinal;
	size_t		pos;
	unsigned	error;
	unsigned	btype;

	(void)settings;
	bp = 0;
	bfinal = 0;
	pos = 0;
	error = 0;
	while (!bfinal)
	{
		if (bp + 2 >= insize * 8)
			return (52);
		bfinal = readBitFromStream(&bp, in);
		btype = 1u * readBitFromStream(&bp, in);
		btype += 2u * readBitFromStream(&bp, in);
		if (btype == 3)
			return (20);
		else if (btype == 0)
			error = inflateNoCompression(out, in, &bp, &pos, insize);
		else
			error = inflateHuffmanBlock(out, in, &bp, &pos,
					insize, btype);
		if (error)
			return (error);
	}
	return (error);
}

unsigned	lodepng_inflate(unsigned char **out, size_t *outsize,
			const unsigned char *in, size_t insize,
			const LodePNGDecompressSettings *settings)
{
	unsigned	error;
	ucvector	v;

	ucvector_init_buffer(&v, *out, *outsize);
	error = lodepng_inflatev(&v, in, insize, settings);
	*out = v.data;
	*outsize = v.size;
	return (error);
}

unsigned	inflate(unsigned char **out, size_t *outsize,
			const unsigned char *in, size_t insize,
			const LodePNGDecompressSettings *settings)
{
	if (settings->custom_inflate)
		return (settings->custom_inflate(out, outsize,
				in, insize, settings));
	return (lodepng_inflate(out, outsize, in, insize, settings));
}
