/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:35:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
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
	unsigned int	i;

	add_length_distance(ctx->out, ctx->length, ctx->offset);
	i = 1;
	while (i < ctx->length)
	{
		++ctx->pos;
		ctx->wpos = ctx->pos & (ctx->windowsize - 1);
		ctx->hashval = get_hash(ctx->in, ctx->insize, ctx->pos);
		if (ctx->hashval == 0)
		{
			if (ctx->numzeros == 0)
				ctx->numzeros = count_zeros(ctx->in,
						ctx->insize, ctx->pos);
			else if (ctx->pos + ctx->numzeros > ctx->insize
				|| ctx->in[ctx->pos + ctx->numzeros - 1] != 0)
				--ctx->numzeros;
		}
		else
			ctx->numzeros = 0;
		update_hash_chain(ctx->hash, ctx->wpos, ctx->hashval,
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

unsigned int	encode_lz77(t_uivector *out, t_deflate_work *w)
{
	t_lz77_ctx	ctx;

	ctx.out = out;
	ctx.hash = w->hash;
	ctx.in = w->data;
	ctx.insize = w->dataend;
	ctx.windowsize = w->settings->windowsize;
	ctx.minmatch = w->settings->minmatch;
	ctx.nicematch = w->settings->nicematch;
	ctx.lazymatching = w->settings->lazymatching;
	if (lz77_init(&ctx))
		return (ctx.error);
	ctx.pos = w->datapos;
	while (ctx.pos < ctx.insize && !ctx.error)
	{
		lz77_hash_pos(&ctx);
		lz77_chain_search(&ctx);
		if (!lz77_lazy(&ctx))
			lz77_emit(&ctx);
		++ctx.pos;
	}
	return (ctx.error);
}
