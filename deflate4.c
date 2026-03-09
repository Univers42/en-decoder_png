/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	deflate_dynamic(t_deflate_work *w,
		unsigned int final)
{
	t_dd_ctx		ctx;
	unsigned int	error;

	dd_ctx_init(&ctx);
	error = dd_lz77_encode(&ctx, w);
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
		error = dd_emit(w, &ctx, final);
	dd_ctx_cleanup(&ctx);
	return (error);
}

static void	dnc_write_block(t_ucvector *out, const unsigned char *data,
		unsigned int *datapos, size_t datasize)
{
	unsigned int	len;
	unsigned int	nlen;
	size_t			j;
	unsigned int	bfinal;

	bfinal = ((*datapos) + 65535 >= datasize);
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

unsigned int	deflate_no_compression(t_ucvector *out,
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
		dnc_write_block(out, data, &datapos, datasize);
		++i;
	}
	return (0);
}

unsigned int	df_lz77(t_deflate_work *w,
		t_huffman_tree *tree_ll, t_huffman_tree *tree_d)
{
	t_uivector		lz77_encoded;
	unsigned int	error;

	uivector_init(&lz77_encoded);
	error = encode_lz77(&lz77_encoded, w);
	if (!error)
		write_lz77_data(w, &lz77_encoded, tree_ll, tree_d);
	uivector_cleanup(&lz77_encoded);
	return (error);
}

void	df_literal(t_deflate_work *w, t_huffman_tree *tree_ll)
{
	size_t	i;

	i = w->datapos;
	while (i < w->dataend)
	{
		add_huffman_symbol(w->bp, w->out,
			huffman_tree_get_code(tree_ll, w->data[i]),
			huffman_tree_get_length(tree_ll, w->data[i]));
		++i;
	}
}
