/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder7.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 01:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 01:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned	enc_run_pipeline(t_enc_ctx *ctx)
{
	ctx->state->error = enc_auto_convert(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_icc_check(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_convert_data(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_write_pre_idat(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	ctx->state->error = enc_write_idat_anc(ctx);
	if (ctx->state->error)
		return (ctx->state->error);
	return (enc_write_post(ctx));
}

unsigned	lodepng_encode(unsigned char **out, size_t *outsize,
		const unsigned char *image, unsigned w, unsigned h,
		LodePNGState *state)
{
	t_enc_ctx	ctx;

	ctx.state = state;
	ctx.image = image;
	ctx.w = w;
	ctx.h = h;
	ctx.data = 0;
	ctx.datasize = 0;
	ucvector_init(&ctx.outv);
	lodepng_info_init(&ctx.info);
	*out = 0;
	*outsize = 0;
	state->error = 0;
	state->error = enc_validate(&ctx);
	if (!state->error)
		enc_run_pipeline(&ctx);
	lodepng_info_cleanup(&ctx.info);
	lodepng_free(ctx.data);
	*out = ctx.outv.data;
	*outsize = ctx.outv.size;
	return (state->error);
}

unsigned	lodepng_encode_memory(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned w, unsigned h,
		LodePNGColorType colortype, unsigned bitdepth)
{
	unsigned		error;
	LodePNGState	state;

	lodepng_state_init(&state);
	state.info_raw.colortype = colortype;
	state.info_raw.bitdepth = bitdepth;
	state.info_png.color.colortype = colortype;
	state.info_png.color.bitdepth = bitdepth;
	lodepng_encode(out, outsize, image, w, h, &state);
	error = state.error;
	lodepng_state_cleanup(&state);
	return (error);
}

unsigned	lodepng_encode32(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned w, unsigned h)
{
	return (lodepng_encode_memory(out, outsize, image,
			w, h, LCT_RGBA, 8));
}

unsigned	lodepng_encode24(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned w, unsigned h)
{
	return (lodepng_encode_memory(out, outsize, image,
			w, h, LCT_RGB, 8));
}
