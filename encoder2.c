/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:35:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:48:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	lz77_lazy_resolve(t_lz77_ctx *ctx)
{
	if (ctx->length > ctx->lazylength + 1)
	{
		if (!uivector_push_back(ctx->out, ctx->in[ctx->pos - 1]))
			ctx->error = 83;
	}
	else
	{
		ctx->length = ctx->lazylength;
		ctx->offset = ctx->lazyoffset;
		ctx->hash->head[ctx->hashval] = -1;
		ctx->hash->headz[ctx->numzeros] = -1;
		--ctx->pos;
	}
	return (0);
}

static int	lz77_lazy(t_lz77_ctx *ctx)
{
	if (!ctx->lazymatching)
		return (0);
	if (!ctx->lazy && ctx->length >= 3
		&& ctx->length <= ctx->maxlazymatch
		&& ctx->length < MAX_SUPPORTED_DEFLATE_LENGTH)
	{
		ctx->lazy = 1;
		ctx->lazylength = ctx->length;
		ctx->lazyoffset = ctx->offset;
		return (1);
	}
	if (!ctx->lazy)
		return (0);
	ctx->lazy = 0;
	if (ctx->pos == 0)
	{
		ctx->error = 81;
		return (0);
	}
	return (lz77_lazy_resolve(ctx));
}

static void	lz77_advance_match(t_lz77_ctx *ctx)
{
	unsigned	i;

	addLengthDistance(ctx->out, ctx->length, ctx->offset);
	i = 1;
	while (i < ctx->length)
	{
		++ctx->pos;
		ctx->wpos = ctx->pos & (ctx->windowsize - 1);
		ctx->hashval = getHash(ctx->in, ctx->insize, ctx->pos);
		if (ctx->hashval == 0)
		{
			if (ctx->numzeros == 0)
				ctx->numzeros = countZeros(ctx->in,
						ctx->insize, ctx->pos);
			else if (ctx->pos + ctx->numzeros > ctx->insize
				|| ctx->in[ctx->pos + ctx->numzeros - 1] != 0)
				--ctx->numzeros;
		}
		else
			ctx->numzeros = 0;
		updateHashChain(ctx->hash, ctx->wpos, ctx->hashval,
			ctx->numzeros);
		++i;
	}
}

void	lz77_emit(t_lz77_ctx *ctx)
{
	if (ctx->length >= 3 && ctx->offset > ctx->windowsize)
	{
		ctx->error = 86;
		return ;
	}
	if (ctx->length < 3
		|| ctx->length < ctx->minmatch
		|| (ctx->length == 3 && ctx->offset > 4096))
	{
		if (!uivector_push_back(ctx->out, ctx->in[ctx->pos]))
			ctx->error = 83;
		return ;
	}
	lz77_advance_match(ctx);
}

unsigned	encodeLZ77(uivector *out, Hash *hash,
		const unsigned char *in, size_t inpos,
		size_t insize, unsigned windowsize,
		unsigned minmatch, unsigned nicematch,
		unsigned lazymatching)
{
	t_lz77_ctx	ctx;

	ctx.out = out;
	ctx.hash = hash;
	ctx.in = in;
	ctx.insize = insize;
	ctx.windowsize = windowsize;
	ctx.minmatch = minmatch;
	ctx.nicematch = nicematch;
	ctx.lazymatching = lazymatching;
	if (lz77_init(&ctx))
		return (ctx.error);
	ctx.pos = inpos;
	while (ctx.pos < insize && !ctx.error)
	{
		lz77_hash_pos(&ctx);
		lz77_chain_search(&ctx);
		if (!lz77_lazy(&ctx))
			lz77_emit(&ctx);
		++ctx.pos;
	}
	return (ctx.error);
}
