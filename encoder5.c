/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:50:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	enc_write_palette(t_enc_ctx *ctx)
{
	if (ctx->info.color.colortype == LCT_PALETTE)
		add_chunk_plte(&ctx->outv, &ctx->info.color);
	if (ctx->state->encoder.force_palette
		&& (ctx->info.color.colortype == LCT_RGB
			|| ctx->info.color.colortype == LCT_RGBA))
		add_chunk_plte(&ctx->outv, &ctx->info.color);
	if (ctx->info.color.colortype == LCT_PALETTE
		&& get_palette_translucency(ctx->info.color.palette,
			ctx->info.color.palettesize) != 0)
		add_chunk_trns(&ctx->outv, &ctx->info.color);
	if ((ctx->info.color.colortype == LCT_GREY
			|| ctx->info.color.colortype == LCT_RGB)
		&& ctx->info.color.key_defined)
		add_chunk_trns(&ctx->outv, &ctx->info.color);
}

unsigned int	enc_write_pre_idat(t_enc_ctx *ctx)
{
	write_signature(&ctx->outv);
	add_chunk_ihdr(&ctx->outv, &ctx->info, ctx->w, ctx->h);
	if (ctx->info.unknown_chunks_data[0])
	{
		ctx->state->error = add_unknown_chunks(&ctx->outv,
				ctx->info.unknown_chunks_data[0],
				ctx->info.unknown_chunks_size[0]);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	if (ctx->info.iccp_defined)
		add_chunk_iccp(&ctx->outv, &ctx->info,
			&ctx->state->encoder.zlibsettings);
	if (ctx->info.srgb_defined)
		add_chunk_srgb(&ctx->outv, &ctx->info);
	if (ctx->info.gama_defined)
		add_chunk_gama(&ctx->outv, &ctx->info);
	if (ctx->info.chrm_defined)
		add_chunk_chrm(&ctx->outv, &ctx->info);
	enc_write_palette(ctx);
	return (0);
}

unsigned int	enc_write_idat_anc(t_enc_ctx *ctx)
{
	if (ctx->info.background_defined)
	{
		ctx->state->error = add_chunk_bkgd(&ctx->outv, &ctx->info);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	if (ctx->info.phys_defined)
		add_chunk_phys(&ctx->outv, &ctx->info);
	if (ctx->info.unknown_chunks_data[1])
	{
		ctx->state->error = add_unknown_chunks(&ctx->outv,
				ctx->info.unknown_chunks_data[1],
				ctx->info.unknown_chunks_size[1]);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	ctx->state->error = add_chunk_idat(&ctx->outv, ctx->data,
			ctx->datasize, &ctx->state->encoder.zlibsettings);
	return (ctx->state->error);
}

unsigned int	enc_write_post(t_enc_ctx *ctx)
{
	ctx->state->error = enc_write_text(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_write_itext_end(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	add_chunk_iend(&ctx->outv);
	return (0);
}
