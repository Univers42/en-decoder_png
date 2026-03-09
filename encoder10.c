/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder10.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 03:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	icc_fix_gray_to_rgb(t_enc_ctx *ctx)
{
	if (ctx->info.color.colortype == LCT_GREY)
		ctx->info.color.colortype = LCT_RGB;
	if (ctx->info.color.colortype == LCT_GREY_ALPHA)
		ctx->info.color.colortype = LCT_RGBA;
	if (ctx->info.color.bitdepth < 8)
		ctx->info.color.bitdepth = 8;
}

unsigned int	enc_icc_check(t_enc_ctx *ctx)
{
	unsigned int	gray_icc;
	unsigned int	gray_png;

	if (!ctx->state->info_png.iccp_defined)
		return (0);
	gray_icc = is_gray_icc_profile(ctx->state->info_png.iccp_profile,
			ctx->state->info_png.iccp_profile_size);
	gray_png = (ctx->info.color.colortype == LCT_GREY
			|| ctx->info.color.colortype == LCT_GREY_ALPHA);
	if (!gray_icc && !is_rgb_icc_profile(
			ctx->state->info_png.iccp_profile,
			ctx->state->info_png.iccp_profile_size))
		return (100);
	if (!ctx->state->encoder.auto_convert && gray_icc != gray_png)
		return (101);
	if (gray_icc && !gray_png)
		return (102);
	if (!gray_icc && gray_png)
		icc_fix_gray_to_rgb(ctx);
	return (0);
}

static unsigned int	enc_convert_and_scan(t_enc_ctx *ctx)
{
	unsigned char	*converted;
	size_t			size;
	t_conv_io		io;

	size = ((size_t)ctx->w * (size_t)ctx->h
			* (size_t)lodepng_get_bpp(&ctx->info.color) + 7) / 8;
	converted = (unsigned char *)lodepng_malloc(size);
	if (!converted && size)
		return (83);
	io = (t_conv_io){converted, ctx->image,
		(size_t)ctx->w * ctx->h};
	ctx->state->error = lodepng_convert(&io,
			&ctx->info.color, &ctx->state->info_raw);
	if (!ctx->state->error)
		pre_process_scanlines(ctx, converted);
	lodepng_free(converted);
	return (ctx->state->error);
}

unsigned int	enc_convert_data(t_enc_ctx *ctx)
{
	if (!lodepng_color_mode_equal(&ctx->state->info_raw,
			&ctx->info.color))
		return (enc_convert_and_scan(ctx));
	pre_process_scanlines(ctx, ctx->image);
	return (0);
}
