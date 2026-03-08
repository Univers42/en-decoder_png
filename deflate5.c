/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	deflateFixed(ucvector *out, size_t *bp, Hash *hash,
		const unsigned char *data, size_t datapos,
		size_t dataend, const LodePNGCompressSettings *s,
		unsigned final)
{
	HuffmanTree	tree_ll;
	HuffmanTree	tree_d;
	unsigned	error;

	error = 0;
	HuffmanTree_init(&tree_ll);
	HuffmanTree_init(&tree_d);
	generateFixedLitLenTree(&tree_ll);
	generateFixedDistanceTree(&tree_d);
	addBitToStream(out, bp, final);
	addBitToStream(out, bp, 1);
	addBitToStream(out, bp, 0);
	if (s->use_lz77)
		error = df_lz77(out, bp, hash, data, datapos,
				dataend, s, &tree_ll, &tree_d);
	else
		df_literal(out, bp, data, datapos, dataend, &tree_ll);
	if (!error)
		addHuffmanSymbol(bp, out,
			HuffmanTree_getCode(&tree_ll, 256),
			HuffmanTree_getLength(&tree_ll, 256));
	HuffmanTree_cleanup(&tree_ll);
	HuffmanTree_cleanup(&tree_d);
	return (error);
}

static size_t	dv_blocksize(size_t insize, unsigned btype)
{
	size_t	blocksize;

	if (btype == 1)
		return (insize);
	blocksize = insize / 8 + 8;
	if (blocksize < 65536)
		blocksize = 65536;
	if (blocksize > 262144)
		blocksize = 262144;
	return (blocksize);
}

static unsigned	dv_deflate_blocks(ucvector *out, size_t *bp,
		Hash *hash, const unsigned char *in, size_t insize,
		size_t nb, size_t bs,
		const LodePNGCompressSettings *settings)
{
	size_t		i;
	size_t		start;
	size_t		end;
	unsigned	error;

	error = 0;
	i = 0;
	while (i != nb && !error)
	{
		start = i * bs;
		end = start + bs;
		if (end > insize)
			end = insize;
		if (settings->btype == 1)
			error = deflateFixed(out, bp, hash, in,
					start, end, settings, i == nb - 1);
		else
			error = deflateDynamic(out, bp, hash, in,
					start, end, settings, i == nb - 1);
		++i;
	}
	return (error);
}

unsigned	lodepng_deflatev(ucvector *out, const unsigned char *in,
		size_t insize, const LodePNGCompressSettings *settings)
{
	size_t		bs;
	size_t		nb;
	size_t		bp;
	Hash		hash;
	unsigned	error;

	bp = 0;
	if (settings->btype > 2)
		return (61);
	if (settings->btype == 0)
		return (deflateNoCompression(out, in, insize));
	bs = dv_blocksize(insize, settings->btype);
	nb = (insize + bs - 1) / bs;
	if (nb == 0)
		nb = 1;
	error = hash_init(&hash, settings->windowsize);
	if (error)
		return (error);
	error = dv_deflate_blocks(out, &bp, &hash, in,
			insize, nb, bs, settings);
	hash_cleanup(&hash);
	return (error);
}

unsigned	lodepng_deflate(unsigned char **out, size_t *outsize,
		const unsigned char *in, size_t insize,
		const LodePNGCompressSettings *settings)
{
	unsigned	error;
	ucvector	v;

	ucvector_init_buffer(&v, *out, *outsize);
	error = lodepng_deflatev(&v, in, insize, settings);
	*out = v.data;
	*outsize = v.size;
	return (error);
}
