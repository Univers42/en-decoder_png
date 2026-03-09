/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTER_H
# define FILTER_H

# include "types.h"

unsigned int	unfilter_scanline(unsigned char *recon,
					const unsigned char *scanline,
					const unsigned char *precon,
					const t_filter_dim *d);
unsigned int	unfilter(unsigned char *out,
					const unsigned char *in,
					unsigned int h,
					t_filter_dim *d);
void			filter_scanline(unsigned char *out,
					const unsigned char *scanline,
					const unsigned char *prevline,
					const t_filter_dim *d);
float			flog2(float f);
unsigned int	filter_alloc(unsigned char **attempt,
					size_t lb);
void			filter_free(unsigned char **attempt);
void			filter_copy_best(unsigned char *out_row,
					unsigned char **attempt,
					unsigned char best_type,
					size_t lb);
unsigned int	filter_zero(t_filter_ctx *f);
unsigned int	filter_minsum(t_filter_ctx *f);
unsigned int	filter_entropy(t_filter_ctx *f);
unsigned int	filter_predef(t_filter_ctx *f,
					const t_encoder_settings *settings);
unsigned int	filter_brute(t_filter_ctx *f,
					const t_encoder_settings *settings);
unsigned int	filter(t_filter_ctx *f,
					const t_png_color_mode *info,
					const t_encoder_settings *settings);

#endif
