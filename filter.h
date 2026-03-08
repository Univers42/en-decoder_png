/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:14:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTER_H
# define FILTER_H

# include "types.h"

unsigned int	unfilter_scanline(unsigned char *recon,
				const unsigned char *scanline,
				const unsigned char *precon, size_t bytewidth,
				unsigned char	filter_type, size_t length);
unsigned int	unfilter(unsigned char *out, const unsigned char *in,
				unsigned int	w, unsigned int h, unsigned int bpp);
void		filter_scanline(unsigned char *out,
				const unsigned char *scanline,
				const unsigned char *prevline, size_t length,
				size_t	bytewidth, unsigned char filter_type);
float		flog2(float f);
unsigned int	filter_alloc(unsigned char **attempt, size_t lb);
void		filter_free(unsigned char **attempt);
void		filter_copy_best(unsigned char *out, unsigned char **attempt,
				unsigned char	best_type, size_t lb, unsigned int y);
unsigned int	filter_zero(unsigned char *out, const unsigned char *in,
				unsigned int	h, size_t lb, size_t bw);
unsigned int	filter_predef(unsigned char *out, const unsigned char *in,
				unsigned int h, size_t lb, size_t bw,
				const t_encoder_settings	*settings);
unsigned int	filter_minsum(unsigned char *out, const unsigned char *in,
				unsigned int	h, size_t lb, size_t bw);
unsigned int	filter_entropy(unsigned char *out, const unsigned char *in,
				unsigned int	h, size_t lb, size_t bw);
unsigned int	filter(unsigned char *out, const unsigned char *in,
				unsigned int w, unsigned int h,
				const t_png_color_mode *info,
				const t_encoder_settings	*settings);

#endif
