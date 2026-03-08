/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	dd_build_bitlen(t_dd_ctx *ctx)
{
	size_t	i;

	ctx->numcodes_ll = ctx->tree_ll.numcodes;
	if (ctx->numcodes_ll > 286)
		ctx->numcodes_ll = 286;
	ctx->numcodes_d = ctx->tree_d.numcodes;
	if (ctx->numcodes_d > 30)
		ctx->numcodes_d = 30;
	i = 0;
	while (i != ctx->numcodes_ll)
	{
		uivector_push_back(&ctx->bitlen_lld,
			huffman_tree_get_length(&ctx->tree_ll, (unsigned int)i));
		++i;
	}
	i = 0;
	while (i != ctx->numcodes_d)
	{
		uivector_push_back(&ctx->bitlen_lld,
			huffman_tree_get_length(&ctx->tree_d, (unsigned int)i));
		++i;
	}
}

static size_t	dd_rle_zeros(t_dd_ctx *ctx, size_t i, unsigned int j)
{
	++j;
	if (j <= 10)
	{
		uivector_push_back(&ctx->bitlen_lld_e, 17);
		uivector_push_back(&ctx->bitlen_lld_e, j - 3);
	}
	else
	{
		if (j > 138)
			j = 138;
		uivector_push_back(&ctx->bitlen_lld_e, 18);
		uivector_push_back(&ctx->bitlen_lld_e, j - 11);
	}
	return (i + j - 1);
}

static size_t	dd_rle_repeats(t_dd_ctx *ctx, size_t i, unsigned int j)
{
	unsigned int	num;
	unsigned int	rest;
	unsigned int	k;

	num = j / 6;
	rest = j % 6;
	uivector_push_back(&ctx->bitlen_lld_e, ctx->bitlen_lld.data[i]);
	k = 0;
	while (k < num)
	{
		uivector_push_back(&ctx->bitlen_lld_e, 16);
		uivector_push_back(&ctx->bitlen_lld_e, 6 - 3);
		++k;
	}
	if (rest >= 3)
	{
		uivector_push_back(&ctx->bitlen_lld_e, 16);
		uivector_push_back(&ctx->bitlen_lld_e, rest - 3);
	}
	else
		j -= rest;
	return (i + j);
}

void	dd_rle_encode(t_dd_ctx *ctx)
{
	size_t		i;
	unsigned int	j;

	i = 0;
	while (i != (unsigned int)ctx->bitlen_lld.size)
	{
		j = 0;
		while (i + j + 1 < (unsigned int)ctx->bitlen_lld.size
			&& ctx->bitlen_lld.data[i + j + 1]
				== ctx->bitlen_lld.data[i])
			++j;
		if (ctx->bitlen_lld.data[i] == 0 && j >= 2)
			i = dd_rle_zeros(ctx, i, j);
		else if (j >= 3)
			i = dd_rle_repeats(ctx, i, j);
		else
			uivector_push_back(&ctx->bitlen_lld_e,
				ctx->bitlen_lld.data[i]);
		++i;
	}
}

unsigned int	dd_freq_cl(t_dd_ctx *ctx)
{
	size_t	i;

	if (!uivector_resizev(&ctx->freq_cl, NUM_CODE_LENGTH_CODES, 0))
		return (83);
	i = 0;
	while (i != ctx->bitlen_lld_e.size)
	{
		++ctx->freq_cl.data[ctx->bitlen_lld_e.data[i]];
		if (ctx->bitlen_lld_e.data[i] >= 16)
			++i;
		++i;
	}
	return (0);
}
