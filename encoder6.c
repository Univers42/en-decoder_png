/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:55:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 00:55:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

static unsigned	enc_write_text_one(t_enc_ctx *ctx, size_t i)
{
	if (strlen(ctx->info.text_keys[i]) > 79)
		return (66);
	if (strlen(ctx->info.text_keys[i]) < 1)
		return (67);
	if (ctx->state->encoder.text_compression)
		addChunk_zTXt(&ctx->outv, ctx->info.text_keys[i],
			ctx->info.text_strings[i],
			&ctx->state->encoder.zlibsettings);
	else
		addChunk_tEXt(&ctx->outv, ctx->info.text_keys[i],
			ctx->info.text_strings[i]);
	return (0);
}

unsigned	enc_write_text(t_enc_ctx *ctx)
{
	size_t	i;

	if (ctx->info.time_defined)
		addChunk_tIME(&ctx->outv, &ctx->info.time);
	i = 0;
	while (i != ctx->info.text_num)
	{
		ctx->state->error = enc_write_text_one(ctx, i);
		if (ctx->state->error)
			return (ctx->state->error);
		++i;
	}
	return (0);
}

static void	enc_write_id(t_enc_ctx *ctx)
{
	size_t		i;
	unsigned	found;

	found = 0;
	i = 0;
	while (i != ctx->info.text_num)
	{
		if (!strcmp(ctx->info.text_keys[i], "LodePNG"))
		{
			found = 1;
			break ;
		}
		++i;
	}
	if (!found)
		addChunk_tEXt(&ctx->outv, "LodePNG", LODEPNG_VERSION_STRING);
}

static unsigned	enc_write_itext_one(t_enc_ctx *ctx, size_t i)
{
	if (strlen(ctx->info.itext_keys[i]) > 79)
		return (66);
	if (strlen(ctx->info.itext_keys[i]) < 1)
		return (67);
	addChunk_iTXt(&ctx->outv,
		ctx->state->encoder.text_compression,
		ctx->info.itext_keys[i], ctx->info.itext_langtags[i],
		ctx->info.itext_transkeys[i], ctx->info.itext_strings[i],
		&ctx->state->encoder.zlibsettings);
	return (0);
}

unsigned	enc_write_itext_end(t_enc_ctx *ctx)
{
	size_t	i;

	if (ctx->state->encoder.add_id)
		enc_write_id(ctx);
	i = 0;
	while (i != ctx->info.itext_num)
	{
		ctx->state->error = enc_write_itext_one(ctx, i);
		if (ctx->state->error)
			return (ctx->state->error);
		++i;
	}
	if (ctx->info.unknown_chunks_data[2])
	{
		ctx->state->error = addUnknownChunks(&ctx->outv,
				ctx->info.unknown_chunks_data[2],
				ctx->info.unknown_chunks_size[2]);
		if (ctx->state->error)
			return (ctx->state->error);
	}
	return (0);
}

#endif
