/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:05:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

const unsigned int	*clcl_order(void)
{
	static const unsigned int	order[NUM_CODE_LENGTH_CODES] = {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2,
		14, 1, 15
	};

	return (order);
}

unsigned int	deflate_fixed(t_deflate_work *w,
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
	add_bit_to_stream(w->out, w->bp, final);
	add_bit_to_stream(w->out, w->bp, 1);
	add_bit_to_stream(w->out, w->bp, 0);
	if (w->settings->use_lz77)
		error = df_lz77(w, &tree_ll, &tree_d);
	else
		df_literal(w, &tree_ll);
	if (!error)
		add_huffman_symbol(w->bp, w->out,
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

static unsigned int	dv_deflate_blocks(t_deflate_work *w,
		size_t insize, size_t nb, size_t bs)
{
	size_t			i;
	unsigned int	error;

	error = 0;
	i = 0;
	while (i != nb && !error)
	{
		w->datapos = i * bs;
		w->dataend = w->datapos + bs;
		if (w->dataend > insize)
			w->dataend = insize;
		if (w->settings->btype == 1)
			error = deflate_fixed(w, i == nb - 1);
		else
			error = deflate_dynamic(w, i == nb - 1);
		++i;
	}
	return (error);
}

unsigned int	lodepng_deflatev(t_ucvector *out, const unsigned char *in,
		size_t insize, const t_compress_settings *settings)
{
	size_t			nb;
	t_deflate_work	w;
	t_hash			hash;
	size_t			bp;
	size_t			bs;

	bp = 0;
	if (settings->btype > 2)
		return (61);
	if (settings->btype == 0)
		return (deflate_no_compression(out, in, insize));
	bs = dv_blocksize(insize, settings->btype);
	nb = (insize + bs - 1) / bs;
	if (nb == 0)
		nb = 1;
	w = (t_deflate_work){out, &bp, &hash, in, 0, 0, settings};
	if (hash_init(&hash, settings->windowsize))
		return (83);
	bs = dv_deflate_blocks(&w, insize, nb, bs);
	hash_cleanup(&hash);
	return (bs);
}
