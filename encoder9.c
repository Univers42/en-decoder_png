/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder9.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 12:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:12:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	lz77_chain_advance(t_lz77_ctx *ctx)
{
	if (ctx->hashpos == ctx->hash->chain[ctx->hashpos])
		return (0);
	if (ctx->numzeros >= 3 && ctx->length > ctx->numzeros)
	{
		ctx->hashpos = ctx->hash->chainz[ctx->hashpos];
		if (ctx->hash->zeros[ctx->hashpos] != ctx->numzeros)
			return (0);
	}
	else
	{
		ctx->hashpos = ctx->hash->chain[ctx->hashpos];
		if (ctx->hash->val[ctx->hashpos] != (int)ctx->hashval)
			return (0);
	}
	return (1);
}

static void	chain_search_init(t_lz77_ctx *ctx)
{
	ctx->length = 0;
	ctx->offset = 0;
	ctx->hashpos = ctx->hash->chain[ctx->wpos];
	if (ctx->insize < ctx->pos + MAX_SUPPORTED_DEFLATE_LENGTH)
		ctx->lastptr = &ctx->in[ctx->insize];
	else
		ctx->lastptr = &ctx->in[ctx->pos
			+ MAX_SUPPORTED_DEFLATE_LENGTH];
}

static int	chain_search_step(t_lz77_ctx *ctx,
				unsigned int *prev_off)
{
	unsigned int	cur_off;

	if (ctx->hashpos <= ctx->wpos)
		cur_off = (unsigned int)(ctx->wpos - ctx->hashpos);
	else
		cur_off = (unsigned int)(ctx->wpos - ctx->hashpos
				+ ctx->windowsize);
	if (cur_off < *prev_off)
		return (0);
	*prev_off = cur_off;
	if (cur_off > 0)
		lz77_try_match(ctx, cur_off);
	if (ctx->length >= ctx->nicematch)
		return (0);
	if (!lz77_chain_advance(ctx))
		return (0);
	return (1);
}

void	lz77_chain_search(t_lz77_ctx *ctx)
{
	unsigned int	chainlength;
	unsigned int	prev_off;

	chain_search_init(ctx);
	prev_off = 0;
	chainlength = 0;
	while (chainlength < ctx->maxchainlength)
	{
		++chainlength;
		if (!chain_search_step(ctx, &prev_off))
			break ;
	}
}
