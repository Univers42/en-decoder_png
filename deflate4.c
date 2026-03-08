/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	deflate_dynamic(ucvector *out, size_t *bp, t_hash *hash,
		const unsigned char *data, size_t datapos,
		size_t dataend, const t_compress_settings *s,
		unsigned int final)
{
	t_dd_ctx		ctx;
	unsigned int	error;

	dd_ctx_init(&ctx);
	error = dd_lz77_encode(&ctx, hash, data, datapos, dataend, s);
	if (!error)
		error = dd_build_freq(&ctx);
	if (!error)
		error = dd_build_trees(&ctx);
	if (!error)
		dd_build_bitlen(&ctx);
	if (!error)
		dd_rle_encode(&ctx);
	if (!error)
		error = dd_freq_cl(&ctx);
	if (!error)
		error = dd_build_cl(&ctx);
	if (!error)
		error = dd_emit(out, bp, &ctx, final);
	dd_ctx_cleanup(&ctx);
	return (error);
}

static void	dnc_write_block(ucvector *out, const unsigned char *data,
		unsigned int *datapos, size_t datasize, unsigned int bfinal)
{
	unsigned int	len;
	unsigned int	nlen;
	size_t			j;

	ucvector_push_back(out, (unsigned char)(bfinal));
	len = 65535;
	if (datasize - *datapos < 65535)
		len = (unsigned int)datasize - *datapos;
	nlen = 65535 - len;
	ucvector_push_back(out, (unsigned char)(len & 255));
	ucvector_push_back(out, (unsigned char)(len >> 8));
	ucvector_push_back(out, (unsigned char)(nlen & 255));
	ucvector_push_back(out, (unsigned char)(nlen >> 8));
	j = 0;
	while (j < 65535 && *datapos < datasize)
	{
		ucvector_push_back(out, data[(*datapos)++]);
		++j;
	}
}

unsigned int	deflate_no_compression(ucvector *out,
		const unsigned char *data, size_t datasize)
{
	size_t			i;
	size_t			numblocks;
	unsigned int	datapos;

	numblocks = (datasize + 65534) / 65535;
	datapos = 0;
	i = 0;
	while (i != numblocks)
	{
		dnc_write_block(out, data, &datapos, datasize,
			i == numblocks - 1);
		++i;
	}
	return (0);
}

unsigned int	df_lz77(ucvector *out, size_t *bp, t_hash *hash,
		const unsigned char *data, size_t datapos,
		size_t dataend, const t_compress_settings *s,
		t_huffman_tree *tree_ll, t_huffman_tree *tree_d)
{
	uivector		lz77_encoded;
	unsigned int	error;

	uivector_init(&lz77_encoded);
	error = encode_lz77(&lz77_encoded, hash, data, datapos,
			dataend, s->windowsize, s->minmatch,
			s->nicematch, s->lazymatching);
	if (!error)
		write_lz77_data(bp, out, &lz77_encoded, tree_ll, tree_d);
	uivector_cleanup(&lz77_encoded);
	return (error);
}

void	df_literal(ucvector *out, size_t *bp,
		const unsigned char *data, size_t datapos,
		size_t dataend, t_huffman_tree *tree_ll)
{
	size_t	i;

	i = datapos;
	while (i < dataend)
	{
		add_huffman_symbol(bp, out,
			huffman_tree_get_code(tree_ll, data[i]),
			huffman_tree_get_length(tree_ll, data[i]));
		++i;
	}
}
