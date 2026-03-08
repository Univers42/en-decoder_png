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

unsigned int	dd_build_cl(t_dd_ctx *ctx)
{
	unsigned int	error;
	size_t		i;

	error = huffman_tree_make_from_freq(&ctx->tree_cl,
			ctx->freq_cl.data, ctx->freq_cl.size,
			ctx->freq_cl.size, 7);
	if (error)
		return (error);
	if (!uivector_resize(&ctx->bitlen_cl, ctx->tree_cl.numcodes))
		return (83);
	i = 0;
	while (i != ctx->tree_cl.numcodes)
	{
		ctx->bitlen_cl.data[i] = huffman_tree_get_length(
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
		t_dd_ctx *ctx, unsigned int bfinal)
{
	unsigned int	hlit;
	unsigned int	hdist;
	unsigned int	hclen;
	size_t		i;

	hlit = (unsigned int)(ctx->numcodes_ll - 257);
	hdist = (unsigned int)(ctx->numcodes_d - 1);
	hclen = (unsigned int)ctx->bitlen_cl.size - 4;
	while (!ctx->bitlen_cl.data[hclen + 4 - 1] && hclen > 0)
		--hclen;
	add_bit_to_stream(out, bp, bfinal);
	add_bit_to_stream(out, bp, 0);
	add_bit_to_stream(out, bp, 1);
	add_bits_to_stream(bp, out, hlit, 5);
	add_bits_to_stream(bp, out, hdist, 5);
	add_bits_to_stream(bp, out, hclen, 4);
	i = 0;
	while (i != hclen + 4)
	{
		add_bits_to_stream(bp, out, ctx->bitlen_cl.data[i], 3);
		++i;
	}
}

void	dd_write_cls(ucvector *out, size_t *bp, t_dd_ctx *ctx)
{
	size_t	i;

	i = 0;
	while (i != ctx->bitlen_lld_e.size)
	{
		add_huffman_symbol(bp, out,
			huffman_tree_get_code(&ctx->tree_cl,
				ctx->bitlen_lld_e.data[i]),
			huffman_tree_get_length(&ctx->tree_cl,
				ctx->bitlen_lld_e.data[i]));
		if (ctx->bitlen_lld_e.data[i] == 16)
			add_bits_to_stream(bp, out,
				ctx->bitlen_lld_e.data[++i], 2);
		else if (ctx->bitlen_lld_e.data[i] == 17)
			add_bits_to_stream(bp, out,
				ctx->bitlen_lld_e.data[++i], 3);
		else if (ctx->bitlen_lld_e.data[i] == 18)
			add_bits_to_stream(bp, out,
				ctx->bitlen_lld_e.data[++i], 7);
		++i;
	}
}

unsigned int	dd_write_data(ucvector *out, size_t *bp, t_dd_ctx *ctx)
{
	write_lz77_data(bp, out, &ctx->lz77_encoded,
		&ctx->tree_ll, &ctx->tree_d);
	if (huffman_tree_get_length(&ctx->tree_ll, 256) == 0)
		return (64);
	add_huffman_symbol(bp, out,
		huffman_tree_get_code(&ctx->tree_ll, 256),
		huffman_tree_get_length(&ctx->tree_ll, 256));
	return (0);
}

unsigned int	dd_emit(ucvector *out, size_t *bp, t_dd_ctx *ctx,
		unsigned int bfinal)
{
	dd_write_header(out, bp, ctx, bfinal);
	dd_write_cls(out, bp, ctx);
	return (dd_write_data(out, bp, ctx));
}
