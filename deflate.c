/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	dd_ctx_init(t_dd_ctx *ctx)
{
	uivector_init(&ctx->lz77_encoded);
	huffman_tree_init(&ctx->tree_ll);
	huffman_tree_init(&ctx->tree_d);
	huffman_tree_init(&ctx->tree_cl);
	uivector_init(&ctx->freq_ll);
	uivector_init(&ctx->freq_d);
	uivector_init(&ctx->freq_cl);
	uivector_init(&ctx->bitlen_lld);
	uivector_init(&ctx->bitlen_lld_e);
	uivector_init(&ctx->bitlen_cl);
}

void	dd_ctx_cleanup(t_dd_ctx *ctx)
{
	uivector_cleanup(&ctx->lz77_encoded);
	huffman_tree_cleanup(&ctx->tree_ll);
	huffman_tree_cleanup(&ctx->tree_d);
	huffman_tree_cleanup(&ctx->tree_cl);
	uivector_cleanup(&ctx->freq_ll);
	uivector_cleanup(&ctx->freq_d);
	uivector_cleanup(&ctx->freq_cl);
	uivector_cleanup(&ctx->bitlen_lld_e);
	uivector_cleanup(&ctx->bitlen_lld);
	uivector_cleanup(&ctx->bitlen_cl);
}

unsigned int	dd_lz77_encode(t_dd_ctx *ctx, t_deflate_work *w)
{
	size_t	datasize;
	size_t	i;

	datasize = w->dataend - w->datapos;
	if (w->settings->use_lz77)
		return (encode_lz77(&ctx->lz77_encoded, w));
	if (!uivector_resize(&ctx->lz77_encoded, datasize))
		return (83);
	i = w->datapos;
	while (i < w->dataend)
	{
		ctx->lz77_encoded.data[i - w->datapos] = w->data[i];
		++i;
	}
	return (0);
}

unsigned int	dd_build_freq(t_dd_ctx *ctx)
{
	size_t			i;
	unsigned int	symbol;

	if (!uivector_resizev(&ctx->freq_ll, 286, 0))
		return (83);
	if (!uivector_resizev(&ctx->freq_d, 30, 0))
		return (83);
	i = 0;
	while (i != ctx->lz77_encoded.size)
	{
		symbol = ctx->lz77_encoded.data[i];
		++ctx->freq_ll.data[symbol];
		if (symbol > 256)
		{
			++ctx->freq_d.data[ctx->lz77_encoded.data[i + 2]];
			i += 3;
		}
		++i;
	}
	ctx->freq_ll.data[256] = 1;
	return (0);
}

unsigned int	dd_build_trees(t_dd_ctx *ctx)
{
	unsigned int	error;

	ctx->tree_ll.max_bit_len = 15;
	error = huffman_tree_make_from_freq(&ctx->tree_ll,
			ctx->freq_ll.data, 257, ctx->freq_ll.size);
	if (error)
		return (error);
	ctx->tree_d.max_bit_len = 15;
	return (huffman_tree_make_from_freq(&ctx->tree_d,
			ctx->freq_d.data, 2, ctx->freq_d.size));
}
