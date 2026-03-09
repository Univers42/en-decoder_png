/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_profile3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:44:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:39:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "color_profile.h"
#include "bit.h"

static void	cp8_check_alpha(t_cp_ctx *ctx, const unsigned char *rg)
{
	unsigned int	matchkey;

	matchkey = (rg[0] == ctx->prof->key_r
			&& rg[1] == ctx->prof->key_g
			&& rg[2] == ctx->prof->key_b);
	if (rg[3] != 255
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
	else if (rg[3] == 255 && ctx->prof->key && matchkey)
		cp_set_alpha(ctx);
}

static void	cp8_add_color(t_cp_ctx *ctx, t_color_tree *tree,
				const unsigned char *rg)
{
	unsigned char	*p;
	unsigned int	n;

	if (color_tree_has(tree, rg))
		return ;
	color_tree_add(tree, rg, ctx->prof->numcolors);
	if (ctx->prof->numcolors < 256)
	{
		p = ctx->prof->palette;
		n = ctx->prof->numcolors;
		p[n * 4 + 0] = rg[0];
		p[n * 4 + 1] = rg[1];
		p[n * 4 + 2] = rg[2];
		p[n * 4 + 3] = rg[3];
	}
	ctx->prof->numcolors++;
	ctx->nc_done = ctx->prof->numcolors >= ctx->maxnc;
}

static void	cp8_scan_pixel(t_cp_ctx *ctx, t_color_tree *tree,
				const unsigned char *rg)
{
	unsigned int	bits;

	if (!ctx->b_done && ctx->prof->bits < 8)
	{
		bits = get_value_required_bits(rg[0]);
		if (bits > ctx->prof->bits)
			ctx->prof->bits = bits;
	}
	ctx->b_done = (ctx->prof->bits >= ctx->bpp);
	if (!ctx->c_done
		&& (rg[0] != rg[1] || rg[0] != rg[2]))
	{
		ctx->prof->colored = 1;
		ctx->c_done = 1;
		if (ctx->prof->bits < 8)
			ctx->prof->bits = 8;
	}
	if (!ctx->a_done)
		cp8_check_alpha(ctx, rg);
	if (!ctx->nc_done)
		cp8_add_color(ctx, tree, rg);
}

static void	cp8_verify_key(t_cp_ctx *ctx)
{
	size_t			i;
	unsigned char	rg[4];

	i = 0;
	while (i != ctx->npx)
	{
		get_pixel_color_rgba8(rg, ctx->in, i, ctx->mode);
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

void	cp8_scan(t_cp_ctx *ctx, t_color_tree *tree)
{
	size_t			i;
	unsigned char	rg[4];

	i = 0;
	while (i != ctx->npx)
	{
		get_pixel_color_rgba8(rg, ctx->in, i, ctx->mode);
		cp8_scan_pixel(ctx, tree, rg);
		if (ctx->a_done && ctx->nc_done
			&& ctx->c_done && ctx->b_done)
			break ;
		i++;
	}
	if (ctx->prof->key && !ctx->prof->alpha)
		cp8_verify_key(ctx);
	ctx->prof->key_r += (ctx->prof->key_r << 8);
	ctx->prof->key_g += (ctx->prof->key_g << 8);
	ctx->prof->key_b += (ctx->prof->key_b << 8);
}
