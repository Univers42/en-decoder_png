/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_profile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:44:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:35:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "color_profile.h"
#include "bit.h"

static void	cp_init_flags(t_cp_ctx *ctx)
{
	ctx->bpp = lodepng_get_bpp(ctx->mode);
	ctx->sixteen = 0;
	ctx->maxnc = 257;
	if (ctx->bpp <= 8)
		ctx->maxnc = lodepng_min_uint(257,
				ctx->prof->numcolors + (1u << ctx->bpp));
	if (lodepng_is_greyscale_type(ctx->mode))
		ctx->c_done = 1;
	else
		ctx->c_done = 0;
	if (lodepng_can_have_alpha(ctx->mode))
		ctx->a_done = 0;
	else
		ctx->a_done = 1;
	ctx->nc_done = 0;
	if (ctx->prof->bits == 1 && ctx->bpp == 1)
		ctx->b_done = 1;
	else
		ctx->b_done = 0;
}

static void	cp_init_done(t_cp_ctx *ctx)
{
	if (ctx->prof->alpha)
		ctx->a_done = 1;
	if (ctx->prof->colored)
		ctx->c_done = 1;
	if (ctx->prof->bits == 16)
		ctx->nc_done = 1;
	if (ctx->prof->bits >= ctx->bpp)
		ctx->b_done = 1;
	if (ctx->prof->numcolors >= ctx->maxnc)
		ctx->nc_done = 1;
}

static void	cp_fill_tree(t_cp_ctx *ctx, t_color_tree *tree)
{
	size_t				i;
	const unsigned char	*color;

	if (ctx->nc_done)
		return ;
	i = 0;
	while (i < ctx->prof->numcolors)
	{
		color = &ctx->prof->palette[i * 4];
		color_tree_add(tree, color, i);
		i++;
	}
}

unsigned int	lodepng_get_color_profile(t_png_color_profile *profile,
				const unsigned char *in, size_t numpixels,
				const t_png_color_mode *mode_in)
{
	t_cp_ctx		ctx;
	t_color_tree	tree;

	ctx.prof = profile;
	ctx.in = in;
	ctx.npx = numpixels;
	ctx.mode = mode_in;
	cp_init_flags(&ctx);
	profile->numpixels += numpixels;
	color_tree_init(&tree);
	cp_init_done(&ctx);
	cp_fill_tree(&ctx, &tree);
	if (mode_in->bitdepth == 16)
		cp16_check_precision(&ctx);
	if (ctx.sixteen)
		cp16_scan(&ctx);
	else
		cp8_scan(&ctx, &tree);
	color_tree_cleanup(&tree);
	return (0);
}

unsigned int	lodepng_color_profile_add(t_png_color_profile *profile,
				const unsigned int *rgba)
{
	unsigned int		error;
	unsigned char		image[8];
	t_png_color_mode	mode;

	lodepng_color_mode_init(&mode);
	image[0] = rgba[0] >> 8;
	image[1] = rgba[0];
	image[2] = rgba[1] >> 8;
	image[3] = rgba[1];
	image[4] = rgba[2] >> 8;
	image[5] = rgba[2];
	image[6] = rgba[3] >> 8;
	image[7] = rgba[3];
	mode.bitdepth = 16;
	mode.colortype = LCT_RGBA;
	error = lodepng_get_color_profile(profile, image, 1, &mode);
	lodepng_color_mode_cleanup(&mode);
	return (error);
}
