/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_choose_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:45:34 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "settings.h"

static unsigned int	get_palette_bits(size_t n)
{
	if (n <= 2)
		return (1);
	if (n <= 4)
		return (2);
	if (n <= 16)
		return (4);
	return (8);
}

static unsigned int	setup_palette_mode(t_png_color_mode *out,
				const t_png_color_mode *in,
				const t_png_color_profile *prof)
{
	unsigned int		error;
	const unsigned char	*p;
	size_t				i;

	error = 0;
	p = prof->palette;
	lodepng_palette_clear(out);
	i = 0;
	while (i != prof->numcolors)
	{
		error = lodepng_palette_add(out, &p[i * 4]);
		if (error)
			break ;
		i++;
	}
	out->colortype = LCT_PALETTE;
	out->bitdepth = get_palette_bits(prof->numcolors);
	if (in->colortype == LCT_PALETTE && in->bitdepth == out->bitdepth
		&& in->palettesize >= out->palettesize)
	{
		lodepng_color_mode_cleanup(out);
		lodepng_color_mode_copy(out, in);
	}
	return (error);
}

static void	setup_direct_mode(t_png_color_mode *out,
				const t_png_color_profile *prof,
				unsigned int alpha, unsigned int key)
{
	unsigned int	bits;
	unsigned int	mask;

	bits = prof->bits;
	if (prof->key && prof->numpixels <= 16 && bits < 8)
		bits = 8;
	out->bitdepth = bits;
	if (alpha && prof->colored)
		out->colortype = LCT_RGBA;
	else if (alpha)
		out->colortype = LCT_GREY_ALPHA;
	else if (prof->colored)
		out->colortype = LCT_RGB;
	else
		out->colortype = LCT_GREY;
	if (!key)
		return ;
	mask = (1u << out->bitdepth) - 1u;
	out->key_r = prof->key_r & mask;
	out->key_g = prof->key_g & mask;
	out->key_b = prof->key_b & mask;
	out->key_defined = 1;
}

unsigned int	auto_choose_color_from_profile(
				t_png_color_mode *out,
				const t_png_color_mode *in,
				const t_png_color_profile *prof)
{
	unsigned int	alpha;
	unsigned int	key;
	unsigned int	bits;
	unsigned int	pal_ok;

	alpha = prof->alpha;
	key = prof->key;
	bits = prof->bits;
	out->key_defined = 0;
	if (key && prof->numpixels <= 16)
	{
		alpha = 1;
		key = 0;
		if (bits < 8)
			bits = 8;
	}
	pal_ok = (prof->numcolors <= 256 && bits <= 8);
	if (prof->numpixels < prof->numcolors * 2)
		pal_ok = 0;
	if (!prof->colored && bits <= get_palette_bits(prof->numcolors))
		pal_ok = 0;
	if (pal_ok)
		return (setup_palette_mode(out, in, prof));
	setup_direct_mode(out, prof, alpha, key);
	return (0);
}

unsigned int	lodepng_auto_choose_color(t_png_color_mode *out,
				const unsigned char *image,
				size_t numpixels,
				const t_png_color_mode *mode_in)
{
	unsigned int		error;
	t_png_color_profile	prof;

	lodepng_color_profile_init(&prof);
	error = lodepng_get_color_profile(&prof, image,
			numpixels, mode_in);
	if (error)
		return (error);
	return (auto_choose_color_from_profile(out, mode_in, &prof));
}
