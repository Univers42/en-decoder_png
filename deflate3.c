/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	dd_build_cl(t_dd_ctx *ctx)
{
	unsigned	error;
	size_t		i;

	error = HuffmanTree_makeFromFrequencies(&ctx->tree_cl,
			ctx->freq_cl.data, ctx->freq_cl.size,
			ctx->freq_cl.size, 7);
	if (error)
		return (error);
	if (!uivector_resize(&ctx->bitlen_cl, ctx->tree_cl.numcodes))
		return (83);
	i = 0;
	while (i != ctx->tree_cl.numcodes)
	{
		ctx->bitlen_cl.data[i] = HuffmanTree_getLength(
				&ctx->tree_cl, g_clcl_order[i]);
		++i;
	}
	while (ctx->bitlen_cl.data[ctx->bitlen_cl.size - 1] == 0
		&& ctx->bitlen_cl.size > 4)
	{
		if (!uivector_resize(&ctx->bitlen_cl,
				ctx->bitlen_cl.size - 1))
			return (83);
	}
	return (0);
}

void	dd_write_header(ucvector *out, size_t *bp,
		t_dd_ctx *ctx, unsigned bfinal)
{
	unsigned	hlit;
	unsigned	hdist;
	unsigned	hclen;
	size_t		i;

	hlit = (unsigned)(ctx->numcodes_ll - 257);
	hdist = (unsigned)(ctx->numcodes_d - 1);
	hclen = (unsigned)ctx->bitlen_cl.size - 4;
	while (!ctx->bitlen_cl.data[hclen + 4 - 1] && hclen > 0)
		--hclen;
	addBitToStream(out, bp, bfinal);
	addBitToStream(out, bp, 0);
	addBitToStream(out, bp, 1);
	addBitsToStream(bp, out, hlit, 5);
	addBitsToStream(bp, out, hdist, 5);
	addBitsToStream(bp, out, hclen, 4);
	i = 0;
	while (i != hclen + 4)
	{
		addBitsToStream(bp, out, ctx->bitlen_cl.data[i], 3);
		++i;
	}
}

void	dd_write_cls(ucvector *out, size_t *bp, t_dd_ctx *ctx)
{
	size_t	i;

	i = 0;
	while (i != ctx->bitlen_lld_e.size)
	{
		addHuffmanSymbol(bp, out,
			HuffmanTree_getCode(&ctx->tree_cl,
				ctx->bitlen_lld_e.data[i]),
			HuffmanTree_getLength(&ctx->tree_cl,
				ctx->bitlen_lld_e.data[i]));
		if (ctx->bitlen_lld_e.data[i] == 16)
			addBitsToStream(bp, out,
				ctx->bitlen_lld_e.data[++i], 2);
		else if (ctx->bitlen_lld_e.data[i] == 17)
			addBitsToStream(bp, out,
				ctx->bitlen_lld_e.data[++i], 3);
		else if (ctx->bitlen_lld_e.data[i] == 18)
			addBitsToStream(bp, out,
				ctx->bitlen_lld_e.data[++i], 7);
		++i;
	}
}

unsigned	dd_write_data(ucvector *out, size_t *bp, t_dd_ctx *ctx)
{
	writeLZ77data(bp, out, &ctx->lz77_encoded,
		&ctx->tree_ll, &ctx->tree_d);
	if (HuffmanTree_getLength(&ctx->tree_ll, 256) == 0)
		return (64);
	addHuffmanSymbol(bp, out,
		HuffmanTree_getCode(&ctx->tree_ll, 256),
		HuffmanTree_getLength(&ctx->tree_ll, 256));
	return (0);
}

unsigned	dd_emit(ucvector *out, size_t *bp, t_dd_ctx *ctx,
		unsigned bfinal)
{
	dd_write_header(out, bp, ctx, bfinal);
	dd_write_cls(out, bp, ctx);
	return (dd_write_data(out, bp, ctx));
}
