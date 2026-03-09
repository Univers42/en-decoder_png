/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:30:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:12:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	lz77_init(t_lz77_ctx *ctx)
{
	if (ctx->windowsize == 0 || ctx->windowsize > 32768)
		return (60);
	if ((ctx->windowsize & (ctx->windowsize - 1)) != 0)
		return (90);
	if (ctx->nicematch > MAX_SUPPORTED_DEFLATE_LENGTH)
		ctx->nicematch = MAX_SUPPORTED_DEFLATE_LENGTH;
	if (ctx->windowsize >= 8192)
		ctx->maxchainlength = ctx->windowsize;
	else
		ctx->maxchainlength = ctx->windowsize / 8;
	if (ctx->windowsize >= 8192)
		ctx->maxlazymatch = MAX_SUPPORTED_DEFLATE_LENGTH;
	else
		ctx->maxlazymatch = 64;
	ctx->numzeros = 0;
	ctx->lazy = 0;
	ctx->lazylength = 0;
	ctx->lazyoffset = 0;
	ctx->error = 0;
	return (0);
}

void	lz77_hash_pos(t_lz77_ctx *ctx)
{
	ctx->wpos = ctx->pos & (ctx->windowsize - 1);
	ctx->hashval = get_hash(ctx->in, ctx->insize, ctx->pos);
	if (ctx->hashval == 0)
	{
		if (ctx->numzeros == 0)
			ctx->numzeros = count_zeros(ctx->in, ctx->insize,
					ctx->pos);
		else if (ctx->pos + ctx->numzeros > ctx->insize
			|| ctx->in[ctx->pos + ctx->numzeros - 1] != 0)
			--ctx->numzeros;
	}
	else
		ctx->numzeros = 0;
	update_hash_chain(ctx->hash, ctx->wpos, ctx->hashval,
		ctx->numzeros);
}

void	lz77_try_match(t_lz77_ctx *ctx, unsigned int cur_off)
{
	const unsigned char	*foreptr;
	const unsigned char	*backptr;
	unsigned int		skip;
	unsigned int		cur_len;

	foreptr = &ctx->in[ctx->pos];
	backptr = &ctx->in[ctx->pos - cur_off];
	if (ctx->numzeros >= 3)
	{
		skip = lodepng_min_uint(
				ctx->hash->zeros[ctx->hashpos], ctx->numzeros);
		backptr += skip;
		foreptr += skip;
	}
	while (foreptr != ctx->lastptr && *backptr == *foreptr)
	{
		++backptr;
		++foreptr;
	}
	cur_len = (unsigned int)(foreptr - &ctx->in[ctx->pos]);
	if (cur_len > ctx->length)
	{
		ctx->length = cur_len;
		ctx->offset = cur_off;
	}
}
