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

unsigned int	deflate_fixed(ucvector *out, size_t *bp, t_hash *hash,
		const unsigned char *data, size_t datapos,
		size_t dataend, const t_compress_settings *s,
		unsigned int final)
{
	t_huffman_tree	tree_ll;
	t_huffman_tree	tree_d;
	unsigned int	error;

	error = 0;
	huffman_tree_init(&tree_ll);
	huffman_tree_init(&tree_d);
	gen_fixed_litlen_tree(&tree_ll);
	gen_fixed_dist_tree(&tree_d);
	add_bit_to_stream(out, bp, final);
	add_bit_to_stream(out, bp, 1);
	add_bit_to_stream(out, bp, 0);
	if (s->use_lz77)
		error = df_lz77(out, bp, hash, data, datapos,
				dataend, s, &tree_ll, &tree_d);
	else
		df_literal(out, bp, data, datapos, dataend, &tree_ll);
	if (!error)
		add_huffman_symbol(bp, out,
			huffman_tree_get_code(&tree_ll, 256),
			huffman_tree_get_length(&tree_ll, 256));
	huffman_tree_cleanup(&tree_ll);
	huffman_tree_cleanup(&tree_d);
	return (error);
}

static size_t	dv_blocksize(size_t insize, unsigned int btype)
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

static unsigned int	dv_deflate_blocks(ucvector *out, size_t *bp,
		t_hash *hash, const unsigned char *in, size_t insize,
		size_t nb, size_t bs,
		const t_compress_settings *settings)
{
	size_t		i;
	size_t		start;
	size_t		end;
	unsigned int	error;

	error = 0;
	i = 0;
	while (i != nb && !error)
	{
		start = i * bs;
		end = start + bs;
		if (end > insize)
			end = insize;
		if (settings->btype == 1)
			error = deflate_fixed(out, bp, hash, in,
					start, end, settings, i == nb - 1);
		else
			error = deflate_dynamic(out, bp, hash, in,
					start, end, settings, i == nb - 1);
		++i;
	}
	return (error);
}

unsigned int	lodepng_deflatev(ucvector *out, const unsigned char *in,
		size_t insize, const t_compress_settings *settings)
{
	size_t		bs;
	size_t		nb;
	size_t		bp;
	t_hash		hash;
	unsigned int	error;

	bp = 0;
	if (settings->btype > 2)
		return (61);
	if (settings->btype == 0)
		return (deflate_no_compression(out, in, insize));
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

unsigned int	lodepng_deflate(unsigned char **out, size_t *outsize,
		const unsigned char *in, size_t insize,
		const t_compress_settings *settings)
{
	unsigned int	error;
	ucvector	v;

	ucvector_init_buffer(&v, *out, *outsize);
	error = lodepng_deflatev(&v, in, insize, settings);
	*out = v.data;
	*outsize = v.size;
	return (error);
}
