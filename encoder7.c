/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder7.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 01:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
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

unsigned int	lodepng_encode(unsigned char **out,
		size_t *outsize, t_enc_ctx *ctx)
{
	*out = 0;
	*outsize = 0;
	ucvector_init(&ctx->outv);
	lodepng_info_init(&ctx->info);
	ctx->data = 0;
	ctx->datasize = 0;
	ctx->state->error = 0;
	ctx->state->error = enc_validate(ctx);
	if (!ctx->state->error)
		enc_run_pipeline(ctx);
	lodepng_info_cleanup(&ctx->info);
	lodepng_free(ctx->data);
	*out = ctx->outv.data;
	*outsize = ctx->outv.size;
	return (ctx->state->error);
}
