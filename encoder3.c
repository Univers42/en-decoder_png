/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:40:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	wlz77_emit_extra(t_deflate_work *w,
		const unsigned int *d, const t_huffman_tree *tree_d)
{
	unsigned int	li;
	unsigned int	di;

	li = d[0] - FIRST_LENGTH_CODE_INDEX;
	add_bits_to_stream(w->bp, w->out, d[1], hft()->lengthextra[li]);
	di = d[2];
	add_huffman_symbol(w->bp, w->out, huffman_tree_get_code(tree_d, di),
		huffman_tree_get_length(tree_d, di));
	add_bits_to_stream(w->bp, w->out, d[3],
		hft()->distanceextra[di]);
}

void	write_lz77_data(t_deflate_work *w,
		const t_uivector *lz77, const t_huffman_tree *tree_ll,
		const t_huffman_tree *tree_d)
{
	size_t			i;
	unsigned int	val;

	i = 0;
	while (i != lz77->size)
	{
		val = lz77->data[i];
		add_huffman_symbol(w->bp, w->out,
			huffman_tree_get_code(tree_ll, val),
			huffman_tree_get_length(tree_ll, val));
		if (val > 256)
		{
			wlz77_emit_extra(w, &lz77->data[i], tree_d);
			i += 3;
		}
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
