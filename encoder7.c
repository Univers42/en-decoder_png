/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder7.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 01:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:58:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	enc_run_pipeline(t_enc_ctx *ctx)
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

unsigned int	lodepng_encode(unsigned char **out, size_t *outsize,
		const unsigned char *image, unsigned int w, unsigned int h,
		t_png_state *state)
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

unsigned int	lodepng_encode_memory(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned int w, unsigned int h,
		t_png_color_type colortype, unsigned int bitdepth)
{
	unsigned int	error;
	t_png_state		state;

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

unsigned int	lodepng_encode32(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned int w, unsigned int h)
{
	return (lodepng_encode_memory(out, outsize, image,
			w, h, LCT_RGBA, 8));
}

unsigned int	lodepng_encode24(unsigned char **out,
		size_t *outsize, const unsigned char *image,
		unsigned int w, unsigned int h)
{
	return (lodepng_encode_memory(out, outsize, image,
			w, h, LCT_RGB, 8));
}
