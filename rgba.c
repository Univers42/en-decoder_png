/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:43:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:17:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	lc_tree_init(t_color_tree *tree,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in)
{
	size_t				psize;
	const unsigned char	*pal;
	size_t				limit;
	size_t				i;

	psize = mode_out->palettesize;
	pal = mode_out->palette;
	limit = (size_t)1u << mode_out->bitdepth;
	if (psize == 0)
	{
		psize = mode_in->palettesize;
		pal = mode_in->palette;
	}
	if (psize < limit)
		limit = psize;
	color_tree_init(tree);
	i = 0;
	while (i != limit)
	{
		color_tree_add(tree, &pal[i * 4], (unsigned int)i);
		++i;
	}
	return (0);
}

static unsigned int	lc_loop_pal(t_conv_io *io,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in, t_color_tree *tree)
{
	unsigned char	rgba[4];
	size_t			i;
	int				index;

	i = 0;
	while (i != io->numpixels)
	{
		get_pixel_color_rgba8(rgba, io->in, i, mode_in);
		index = color_tree_get(tree, rgba);
		if (index < 0)
			return (82);
		if (mode_out->bitdepth == 8)
			io->out[i] = index;
		else
			add_color_bits(io->out, i,
				mode_out->bitdepth, (unsigned int)index);
		++i;
	}
	return (0);
}

static unsigned int	lc_loop_16(t_conv_io *io,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in)
{
	unsigned short	rgba[4];
	size_t			i;

	i = 0;
	while (i != io->numpixels)
	{
		get_pixel_color_rgba16(rgba, io->in, i, mode_in);
		rgba16_to_pixel(io->out, i, mode_out, rgba);
		++i;
	}
	return (0);
}

static unsigned int	lc_loop_8(t_conv_io *io,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in)
{
	unsigned char	rgba[4];
	size_t			i;
	unsigned int	error;

	i = 0;
	while (i != io->numpixels)
	{
		get_pixel_color_rgba8(rgba, io->in, i, mode_in);
		error = rgba8_to_pixel(io->out, i, mode_out, rgba);
		if (error)
			return (error);
		++i;
	}
	return (0);
}

unsigned int	lodepng_convert(t_conv_io *io,
		const t_png_color_mode *mode_out,
		const t_png_color_mode *mode_in)
{
	t_color_tree	tree;
	unsigned int	error;

	if (mode_out->colortype == LCT_PALETTE)
		lc_tree_init(&tree, mode_out, mode_in);
	if (mode_in->bitdepth == 16 && mode_out->bitdepth == 16)
		error = lc_loop_16(io, mode_out, mode_in);
	else if (mode_out->colortype == LCT_PALETTE)
		error = lc_loop_pal(io, mode_out, mode_in, &tree);
	else
		error = lc_loop_8(io, mode_out, mode_in);
	if (mode_out->colortype == LCT_PALETTE)
		color_tree_cleanup(&tree);
	return (error);
}
