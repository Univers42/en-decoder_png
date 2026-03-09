/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:56 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:23:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# include <stddef.h>
# include <stdlib.h>
# include "types.h"
# include "utils.h"

unsigned int		check_color_validity(t_png_color_type colortype,
						unsigned int bd);
unsigned int		get_num_color_channels(t_png_color_type colortype);
unsigned int		lodepng_get_bpp_lct(t_png_color_type colortype,
						unsigned int bitdepth);
void				lodepng_palette_clear(t_png_color_mode *info);
void				lodepng_color_mode_init(t_png_color_mode *info);
void				lodepng_color_mode_cleanup(t_png_color_mode *info);
unsigned int		lodepng_color_mode_copy(t_png_color_mode *dest,
						const t_png_color_mode *source);
t_png_color_mode	lodepng_color_mode_make(t_png_color_type colortype,
						unsigned int bitdepth);
int					lodepng_color_mode_equal(const t_png_color_mode *a,
						const t_png_color_mode *b);
unsigned int		lodepng_palette_add(t_png_color_mode *info,
						const unsigned char *rgba);
unsigned int		lodepng_get_bpp(const t_png_color_mode *info);
unsigned int		lodepng_get_channels(const t_png_color_mode *info);
unsigned int		lodepng_is_greyscale_type(
						const t_png_color_mode *info);
unsigned int		lodepng_is_alpha_type(const t_png_color_mode *info);
unsigned int		lodepng_is_palette_type(
						const t_png_color_mode *info);
unsigned int		lodepng_has_palette_alpha(
						const t_png_color_mode *info);
unsigned int		lodepng_can_have_alpha(
						const t_png_color_mode *info);
size_t				lodepng_get_raw_size_lct(unsigned int w,
						unsigned int h,
						t_png_color_type colortype,
						unsigned int bitdepth);
size_t				lodepng_get_raw_size(unsigned int w, unsigned int h,
						const t_png_color_mode *color);
void				add_color_bits(unsigned char *out, size_t index,
						unsigned int bits, unsigned int in);
void				color_tree_init(t_color_tree *tree);
void				color_tree_cleanup(t_color_tree *tree);
int					color_tree_get(t_color_tree *tree,
						const unsigned char *rgba);
int					color_tree_has(t_color_tree *tree,
						const unsigned char *rgba);
void				color_tree_add(t_color_tree *tree,
						const unsigned char *rgba,
						unsigned int index);
unsigned char		paeth_predictor(int a, int b, int c);

unsigned int		lodepng_convert(t_conv_io *io,
						const t_png_color_mode *mode_out,
						const t_png_color_mode *mode_in);
unsigned int		lodepng_auto_choose_color(t_png_color_mode *out,
						const unsigned char *image,
						size_t numpixels,
						const t_png_color_mode *mode_in);
unsigned int		lodepng_get_color_profile(
						t_png_color_profile *profile,
						const unsigned char *in,
						size_t numpixels,
						const t_png_color_mode *mode_in);
unsigned int		auto_choose_color_from_profile(
						t_png_color_mode *out,
						const t_png_color_mode *in,
						const t_png_color_profile *prof);
unsigned int		get_palette_translucency(
						const unsigned char *palette,
						size_t palettesize);
unsigned int		lodepng_convert_rgb(unsigned int *out,
						const unsigned int *in,
						const t_png_color_mode *mode_out,
						const t_png_color_mode *mode_in);
unsigned int		rgba8_to_pixel(unsigned char *out,
						size_t i,
						const t_png_color_mode *mode,
						const unsigned char *rgba);
void				rgba16_to_pixel(unsigned char *out,
						size_t i,
						const t_png_color_mode *mode,
						const unsigned short *rgba);
void				get_pixel_color_rgba16(unsigned short *rgba,
						const unsigned char *in,
						size_t i,
						const t_png_color_mode *mode);
void				get_pixel_color_rgba8(unsigned char *rgba,
						const unsigned char *in,
						size_t i,
						const t_png_color_mode *mode);
void				gpc8_grey(unsigned char *rgba,
						const unsigned char *in,
						size_t i,
						const t_png_color_mode *mode);
void				gpc8_rgb(unsigned char *rgba,
						const unsigned char *in,
						size_t i,
						const t_png_color_mode *mode);

#endif
