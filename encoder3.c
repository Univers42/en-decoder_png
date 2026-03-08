/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:40:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:48:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	wlz77_emit_extra(size_t *bp, ucvector *out,
		const uivector *lz77, const HuffmanTree *tree_d,
		size_t *i)
{
	unsigned	li;
	unsigned	di;

	li = lz77->data[*i] - FIRST_LENGTH_CODE_INDEX;
	addBitsToStream(bp, out, lz77->data[++(*i)], LENGTHEXTRA[li]);
	di = lz77->data[++(*i)];
	addHuffmanSymbol(bp, out, HuffmanTree_getCode(tree_d, di),
		HuffmanTree_getLength(tree_d, di));
	addBitsToStream(bp, out, lz77->data[++(*i)],
		DISTANCEEXTRA[di]);
}

void	writeLZ77data(size_t *bp, ucvector *out,
		const uivector *lz77_encoded, const HuffmanTree *tree_ll,
		const HuffmanTree *tree_d)
{
	size_t		i;
	unsigned	val;

	i = 0;
	while (i != lz77_encoded->size)
	{
		val = lz77_encoded->data[i];
		addHuffmanSymbol(bp, out, HuffmanTree_getCode(tree_ll, val),
			HuffmanTree_getLength(tree_ll, val));
		if (val > 256)
			wlz77_emit_extra(bp, out, lz77_encoded, tree_d, &i);
		++i;
	}
}

unsigned	enc_validate(t_enc_ctx *ctx)
{
	if ((ctx->state->info_png.color.colortype == LCT_PALETTE
			|| ctx->state->encoder.force_palette)
		&& (ctx->state->info_png.color.palettesize == 0
			|| ctx->state->info_png.color.palettesize > 256))
		return (68);
	if (ctx->state->encoder.zlibsettings.btype > 2)
		return (61);
	if (ctx->state->info_png.interlace_method > 1)
		return (71);
	ctx->state->error = checkColorValidity(
			ctx->state->info_png.color.colortype,
			ctx->state->info_png.color.bitdepth);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = checkColorValidity(
			ctx->state->info_raw.colortype,
			ctx->state->info_raw.bitdepth);
	return (ctx->state->error);
}
