/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_profile2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:44:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:35:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "color_profile.h"

void	cp_set_alpha(t_cp_ctx *ctx)
{
	ctx->prof->alpha = 1;
	ctx->prof->key = 0;
	ctx->a_done = 1;
	if (ctx->prof->bits < 8)
		ctx->prof->bits = 8;
}

static void	cp16_check_alpha(t_cp_ctx *ctx, const unsigned short *rg)
{
	unsigned int	matchkey;

	matchkey = (rg[0] == ctx->prof->key_r
			&& rg[1] == ctx->prof->key_g
			&& rg[2] == ctx->prof->key_b);
	if (rg[3] != 65535
		&& (rg[3] != 0 || (ctx->prof->key && !matchkey)))
		cp_set_alpha(ctx);
	else if (rg[3] == 0 && !ctx->prof->alpha
		&& !ctx->prof->key)
	{
		ctx->prof->key = 1;
		ctx->prof->key_r = rg[0];
		ctx->prof->key_g = rg[1];
		ctx->prof->key_b = rg[2];
	}
	else if (rg[3] == 65535 && ctx->prof->key && matchkey)
		cp_set_alpha(ctx);
}

static void	cp16_verify_key(t_cp_ctx *ctx)
{
	size_t			i;
	unsigned short	rg[4];

	i = 0;
	while (i != ctx->npx)
	{
		get_pixel_color_rgba16(rg, ctx->in, i, ctx->mode);
		if (rg[3] != 0 && rg[0] == ctx->prof->key_r
			&& rg[1] == ctx->prof->key_g
			&& rg[2] == ctx->prof->key_b)
		{
			cp_set_alpha(ctx);
			break ;
		}
		i++;
	}
}

void	cp16_check_precision(t_cp_ctx *ctx)
{
	size_t			i;
	unsigned short	rg[4];

	i = 0;
	while (i != ctx->npx)
	{
		get_pixel_color_rgba16(rg, ctx->in, i, ctx->mode);
		if ((rg[0] & 255) != ((rg[0] >> 8) & 255)
			|| (rg[1] & 255) != ((rg[1] >> 8) & 255)
			|| (rg[2] & 255) != ((rg[2] >> 8) & 255)
			|| (rg[3] & 255) != ((rg[3] >> 8) & 255))
		{
			ctx->prof->bits = 16;
			ctx->sixteen = 1;
			ctx->b_done = 1;
			ctx->nc_done = 1;
			break ;
		}
		i++;
	}
}

void	cp16_scan(t_cp_ctx *ctx)
{
	size_t			i;
	unsigned short	rg[4];

	i = 0;
	while (i != ctx->npx)
	{
		get_pixel_color_rgba16(rg, ctx->in, i, ctx->mode);
		if (!ctx->c_done
			&& (rg[0] != rg[1] || rg[0] != rg[2]))
		{
			ctx->prof->colored = 1;
			ctx->c_done = 1;
		}
		if (!ctx->a_done)
			cp16_check_alpha(ctx, rg);
		if (ctx->a_done && ctx->nc_done
			&& ctx->c_done && ctx->b_done)
			break ;
		i++;
	}
	if (ctx->prof->key && !ctx->prof->alpha)
		cp16_verify_key(ctx);
}
