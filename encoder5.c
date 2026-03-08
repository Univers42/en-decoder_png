/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:50:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 00:50:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	enc_write_palette(t_enc_ctx *ctx)
{
	if (ctx->info.color.colortype == LCT_PALETTE)
		addChunk_PLTE(&ctx->outv, &ctx->info.color);
	if (ctx->state->encoder.force_palette
		&& (ctx->info.color.colortype == LCT_RGB
			|| ctx->info.color.colortype == LCT_RGBA))
		addChunk_PLTE(&ctx->outv, &ctx->info.color);
	if (ctx->info.color.colortype == LCT_PALETTE
		&& getPaletteTranslucency(ctx->info.color.palette,
			ctx->info.color.palettesize) != 0)
		addChunk_tRNS(&ctx->outv, &ctx->info.color);
	if ((ctx->info.color.colortype == LCT_GREY
			|| ctx->info.color.colortype == LCT_RGB)
		&& ctx->info.color.key_defined)
		addChunk_tRNS(&ctx->outv, &ctx->info.color);
}

unsigned	enc_write_pre_idat(t_enc_ctx *ctx)
{
	writeSignature(&ctx->outv);
	addChunk_IHDR(&ctx->outv, ctx->w, ctx->h,
		ctx->info.color.colortype, ctx->info.color.bitdepth,
		ctx->info.interlace_method);
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	if (ctx->info.unknown_chunks_data[0])
	{
		ctx->state->error = addUnknownChunks(&ctx->outv,
				ctx->info.unknown_chunks_data[0],
				ctx->info.unknown_chunks_size[0]);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	if (ctx->info.iccp_defined)
		addChunk_iCCP(&ctx->outv, &ctx->info,
			&ctx->state->encoder.zlibsettings);
	if (ctx->info.srgb_defined)
		addChunk_sRGB(&ctx->outv, &ctx->info);
	if (ctx->info.gama_defined)
		addChunk_gAMA(&ctx->outv, &ctx->info);
	if (ctx->info.chrm_defined)
		addChunk_cHRM(&ctx->outv, &ctx->info);
#endif
	enc_write_palette(ctx);
	return (0);
}

unsigned	enc_write_idat_anc(t_enc_ctx *ctx)
{
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	if (ctx->info.background_defined)
	{
		ctx->state->error = addChunk_bKGD(&ctx->outv, &ctx->info);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	if (ctx->info.phys_defined)
		addChunk_pHYs(&ctx->outv, &ctx->info);
	if (ctx->info.unknown_chunks_data[1])
	{
		ctx->state->error = addUnknownChunks(&ctx->outv,
				ctx->info.unknown_chunks_data[1],
				ctx->info.unknown_chunks_size[1]);
		if (ctx->state->error)
			return (ctx->state->error);
	}
#endif
	ctx->state->error = addChunk_IDAT(&ctx->outv, ctx->data,
			ctx->datasize, &ctx->state->encoder.zlibsettings);
	return (ctx->state->error);
}

unsigned	enc_write_post(t_enc_ctx *ctx)
{
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	ctx->state->error = enc_write_text(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_write_itext_end(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
#endif
	addChunk_IEND(&ctx->outv);
	return (0);
}
