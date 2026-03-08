/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:45:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 00:45:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
static unsigned	enc_auto_bg_setup(t_enc_ctx *ctx,
		LodePNGColorProfile *prof, unsigned bg[3])
{
	unsigned			rgb[3];
	LodePNGColorMode	mode16;

	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;
	mode16 = lodepng_color_mode_make(LCT_RGB, 16);
	lodepng_convert_rgb(&rgb[0], &rgb[1], &rgb[2],
		bg[0], bg[1], bg[2], &mode16,
		&ctx->state->info_png.color);
	lodepng_color_profile_init(prof);
	ctx->state->error = lodepng_get_color_profile(prof,
			ctx->image, ctx->w, ctx->h, &ctx->state->info_raw);
	if (ctx->state->error)
		return (ctx->state->error);
	lodepng_color_profile_add(prof, rgb[0], rgb[1], rgb[2], 65535);
	ctx->state->error = auto_choose_color_from_profile(
			&ctx->info.color, &ctx->state->info_raw, prof);
	return (ctx->state->error);
}

static unsigned	enc_auto_bg(t_enc_ctx *ctx)
{
	unsigned			bg[3];
	LodePNGColorProfile	prof;

	bg[0] = ctx->state->info_png.background_r;
	bg[1] = ctx->state->info_png.background_g;
	bg[2] = ctx->state->info_png.background_b;
	if (enc_auto_bg_setup(ctx, &prof, bg))
		return (ctx->state->error);
	if (lodepng_convert_rgb(&ctx->info.background_r,
			&ctx->info.background_g, &ctx->info.background_b,
			bg[0], bg[1], bg[2], &ctx->info.color,
			&ctx->state->info_png.color))
		return (104);
	return (0);
}
#endif

unsigned	enc_auto_convert(t_enc_ctx *ctx)
{
	lodepng_info_copy(&ctx->info, &ctx->state->info_png);
	if (!ctx->state->encoder.auto_convert)
		return (0);
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	if (ctx->state->info_png.background_defined)
		return (enc_auto_bg(ctx));
#endif
	ctx->state->error = lodepng_auto_choose_color(&ctx->info.color,
			ctx->image, ctx->w, ctx->h, &ctx->state->info_raw);
	return (ctx->state->error);
}

unsigned	enc_icc_check(t_enc_ctx *ctx)
{
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	unsigned	gray_icc;
	unsigned	gray_png;

	if (!ctx->state->info_png.iccp_defined)
		return (0);
	gray_icc = isGrayICCProfile(ctx->state->info_png.iccp_profile,
			ctx->state->info_png.iccp_profile_size);
	gray_png = (ctx->info.color.colortype == LCT_GREY
			|| ctx->info.color.colortype == LCT_GREY_ALPHA);
	if (!gray_icc && !isRGBICCProfile(
			ctx->state->info_png.iccp_profile,
			ctx->state->info_png.iccp_profile_size))
		return (100);
	if (!ctx->state->encoder.auto_convert && gray_icc != gray_png)
		return (101);
	if (gray_icc && !gray_png)
		return (102);
	if (!gray_icc && gray_png)
	{
		if (ctx->info.color.colortype == LCT_GREY)
			ctx->info.color.colortype = LCT_RGB;
		if (ctx->info.color.colortype == LCT_GREY_ALPHA)
			ctx->info.color.colortype = LCT_RGBA;
		if (ctx->info.color.bitdepth < 8)
			ctx->info.color.bitdepth = 8;
	}
#else
	(void)ctx;
#endif
	return (0);
}

unsigned	enc_convert_data(t_enc_ctx *ctx)
{
	unsigned char	*converted;
	size_t			size;

	if (!lodepng_color_mode_equal(&ctx->state->info_raw,
			&ctx->info.color))
	{
		size = ((size_t)ctx->w * (size_t)ctx->h
				* (size_t)lodepng_get_bpp(&ctx->info.color) + 7) / 8;
		converted = (unsigned char *)lodepng_malloc(size);
		if (!converted && size)
			return (83);
		ctx->state->error = lodepng_convert(converted, ctx->image,
				&ctx->info.color, &ctx->state->info_raw,
				ctx->w, ctx->h);
		if (!ctx->state->error)
			preProcessScanlines(&ctx->data, &ctx->datasize,
				converted, ctx->w, ctx->h, &ctx->info,
				&ctx->state->encoder);
		lodepng_free(converted);
		return (ctx->state->error);
	}
	preProcessScanlines(&ctx->data, &ctx->datasize, ctx->image,
		ctx->w, ctx->h, &ctx->info, &ctx->state->encoder);
	return (0);
}
