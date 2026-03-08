/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:56 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:33:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef COLORS_H
#  define COLORS_H

#  include <stddef.h>
#  include <stdlib.h>
#  include "types.h"
#  include "utils.h"

unsigned int		checkColorValidity(LodePNGColorType colortype,
						unsigned int bd);
unsigned int		getNumColorChannels(LodePNGColorType colortype);
unsigned int		lodepng_get_bpp_lct(LodePNGColorType colortype,
						unsigned int bitdepth);
void				lodepng_palette_clear(LodePNGColorMode *info);
void				lodepng_color_mode_init(LodePNGColorMode *info);
void				lodepng_color_mode_cleanup(LodePNGColorMode *info);
unsigned int		lodepng_color_mode_copy(LodePNGColorMode *dest,
						const LodePNGColorMode *source);
LodePNGColorMode	lodepng_color_mode_make(LodePNGColorType colortype,
						unsigned int bitdepth);
int					lodepng_color_mode_equal(const LodePNGColorMode *a,
						const LodePNGColorMode *b);
unsigned int		lodepng_palette_add(LodePNGColorMode *info,
						unsigned char r, unsigned char g,
						unsigned char b, unsigned char a);
unsigned int		lodepng_get_bpp(const LodePNGColorMode *info);
unsigned int		lodepng_get_channels(const LodePNGColorMode *info);
unsigned int		lodepng_is_greyscale_type(
						const LodePNGColorMode *info);
unsigned int		lodepng_is_alpha_type(const LodePNGColorMode *info);
unsigned int		lodepng_is_palette_type(
						const LodePNGColorMode *info);
unsigned int		lodepng_has_palette_alpha(
						const LodePNGColorMode *info);
unsigned int		lodepng_can_have_alpha(
						const LodePNGColorMode *info);
size_t				lodepng_get_raw_size_lct(unsigned int w,
						unsigned int h, LodePNGColorType colortype,
						unsigned int bitdepth);
size_t				lodepng_get_raw_size(unsigned int w, unsigned int h,
						const LodePNGColorMode *color);
void				addColorBits(unsigned char *out, size_t index,
						unsigned int bits, unsigned int in);
void				color_tree_init(ColorTree *tree);
void				color_tree_cleanup(ColorTree *tree);
int					color_tree_get(ColorTree *tree, unsigned char r,
						unsigned char g, unsigned char b,
						unsigned char a);
int					color_tree_has(ColorTree *tree, unsigned char r,
						unsigned char g, unsigned char b,
						unsigned char a);
void				color_tree_add(ColorTree *tree, unsigned char r,
						unsigned char g, unsigned char b,
						unsigned char a, unsigned int index);
unsigned char		paethPredictor(int a, int b, int c);

unsigned int		lodepng_convert(unsigned char *out,
						const unsigned char *in,
						const LodePNGColorMode *mode_out,
						const LodePNGColorMode *mode_in,
						unsigned int w, unsigned int h);
unsigned int		lodepng_auto_choose_color(LodePNGColorMode *mode_out,
						const unsigned char *image, unsigned int w,
						unsigned int h,
						const LodePNGColorMode *mode_in);
unsigned int		getPaletteTranslucency(const unsigned char *palette,
						size_t palettesize);

# endif