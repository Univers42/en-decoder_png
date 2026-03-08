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

unsigned	unfilterScanline(unsigned char *recon,
				const unsigned char *scanline,
				const unsigned char *precon, size_t bytewidth,
				unsigned char filterType, size_t length);
unsigned	unfilter(unsigned char *out, const unsigned char *in,
				unsigned w, unsigned h, unsigned bpp);
void		filterScanline(unsigned char *out,
				const unsigned char *scanline,
				const unsigned char *prevline, size_t length,
				size_t bytewidth, unsigned char filterType);
float		flog2(float f);
unsigned	filter_alloc(unsigned char **attempt, size_t lb);
void		filter_free(unsigned char **attempt);
void		filter_copy_best(unsigned char *out, unsigned char **attempt,
				unsigned char best_type, size_t lb, unsigned y);
unsigned	filter_zero(unsigned char *out, const unsigned char *in,
				unsigned h, size_t lb, size_t bw);
unsigned	filter_predef(unsigned char *out, const unsigned char *in,
				unsigned h, size_t lb, size_t bw,
				const LodePNGEncoderSettings *settings);
unsigned	filter_minsum(unsigned char *out, const unsigned char *in,
				unsigned h, size_t lb, size_t bw);
unsigned	filter_entropy(unsigned char *out, const unsigned char *in,
				unsigned h, size_t lb, size_t bw);
unsigned	filter(unsigned char *out, const unsigned char *in,
				unsigned w, unsigned h,
				const LodePNGColorMode *info,
				const LodePNGEncoderSettings *settings);

#endif
