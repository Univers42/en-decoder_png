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
		const uivector *lz77, const t_huffman_tree *tree_d,
		size_t *i)
{
	unsigned int	li;
	unsigned int	di;

	li = lz77->data[*i] - FIRST_LENGTH_CODE_INDEX;
	add_bits_to_stream(bp, out, lz77->data[++(*i)], g_lengthextra[li]);
	di = lz77->data[++(*i)];
	add_huffman_symbol(bp, out, huffman_tree_get_code(tree_d, di),
		huffman_tree_get_length(tree_d, di));
	add_bits_to_stream(bp, out, lz77->data[++(*i)],
		g_distanceextra[di]);
}

void	write_lz77_data(size_t *bp, ucvector *out,
		const uivector *lz77_encoded, const t_huffman_tree *tree_ll,
		const t_huffman_tree *tree_d)
{
	size_t		i;
	unsigned int	val;

	i = 0;
	while (i != lz77_encoded->size)
	{
		val = lz77_encoded->data[i];
		add_huffman_symbol(bp, out, huffman_tree_get_code(tree_ll, val),
			huffman_tree_get_length(tree_ll, val));
		if (val > 256)
			wlz77_emit_extra(bp, out, lz77_encoded, tree_d, &i);
		++i;
	}
}

unsigned int	enc_validate(t_enc_ctx *ctx)
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
	ctx->state->error = check_color_validity(
			ctx->state->info_png.color.colortype,
			ctx->state->info_png.color.bitdepth);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = check_color_validity(
			ctx->state->info_raw.colortype,
			ctx->state->info_raw.bitdepth);
	return (ctx->state->error);
}
