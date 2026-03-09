/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:45:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	enc_auto_bg_setup(t_enc_ctx *ctx,
		t_png_color_profile *prof, unsigned int bg[3])
{
	unsigned int		rgb[4];
	t_png_color_mode	mode16;

	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;
	mode16 = lodepng_color_mode_make(LCT_RGB, 16);
	lodepng_convert_rgb(rgb, bg, &mode16,
		&ctx->state->info_png.color);
	lodepng_color_profile_init(prof);
	ctx->state->error = lodepng_get_color_profile(prof,
			ctx->image, (size_t)ctx->w * ctx->h,
			&ctx->state->info_raw);
	if (ctx->state->error)
		return (ctx->state->error);
	rgb[3] = 65535;
	lodepng_color_profile_add(prof, rgb);
	ctx->state->error = auto_choose_color_from_profile(
			&ctx->info.color, &ctx->state->info_raw, prof);
	return (ctx->state->error);
}

static unsigned int	enc_auto_bg(t_enc_ctx *ctx)
{
	unsigned int		bg[3];
	unsigned int		bg_out[3];
	t_png_color_profile	prof;

	bg[0] = ctx->state->info_png.background_r;
	bg[1] = ctx->state->info_png.background_g;
	bg[2] = ctx->state->info_png.background_b;
	if (enc_auto_bg_setup(ctx, &prof, bg))
		return (ctx->state->error);
	if (lodepng_convert_rgb(bg_out, bg, &ctx->info.color,
			&ctx->state->info_png.color))
		return (104);
	ctx->info.background_r = bg_out[0];
	ctx->info.background_g = bg_out[1];
	ctx->info.background_b = bg_out[2];
	return (0);
}

unsigned int	enc_auto_convert(t_enc_ctx *ctx)
{
	lodepng_info_copy(&ctx->info, &ctx->state->info_png);
	if (!ctx->state->encoder.auto_convert)
		return (0);
	if (ctx->state->info_png.background_defined)
		return (enc_auto_bg(ctx));
	ctx->state->error = lodepng_auto_choose_color(&ctx->info.color,
			ctx->image, (size_t)ctx->w * ctx->h,
			&ctx->state->info_raw);
	return (ctx->state->error);
}
