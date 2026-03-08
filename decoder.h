/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:11:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECODER_H
# define DECODER_H

# include "types.h"

void		dg_handle_chunk(t_png_state *st, ucvector *idat,
				const unsigned char *ch, unsigned int *iend,
				unsigned int	*unk, unsigned int *cpos);
void		dg_chunk_loop(t_png_state *state, ucvector *idat,
				const unsigned char	*in, size_t insize);
void		dg_postprocess(t_png_state *state, unsigned char **out,
				ucvector	*scanlines, unsigned int w, unsigned int h);
void		dg_decompress(t_png_state *state, unsigned char **out,
				ucvector	*idat, unsigned int w, unsigned int h);
void		decode_generic(unsigned char **out, unsigned int *w, unsigned int *h,
				t_png_state *state,
				const unsigned char	*in, size_t insize);
unsigned int	lodepng_decode(unsigned char **out, unsigned int *w, unsigned int *h,
				t_png_state *state,
				const unsigned char	*in, size_t insize);
unsigned int	lodepng_decode_memory(unsigned char **out, unsigned int *w,
				unsigned int *h, const unsigned char *in, size_t insize,
				t_png_color_type	colortype, unsigned int bitdepth);
unsigned int	lodepng_decode32_memory(unsigned char **out, unsigned int *w,
				unsigned int	*h, const unsigned char *in, size_t insize);
unsigned int	lodepng_decode32(unsigned char **out, unsigned int *w,
				unsigned int	*h, const unsigned char *in, size_t insize);
unsigned int	lodepng_decode24(unsigned char **out, unsigned int *w,
				unsigned int	*h, const unsigned char *in, size_t insize);
# ifdef LODEPNG_COMPILE_DISK
unsigned int	lodepng_decode_file(unsigned char **out, unsigned int *w,
				unsigned int *h, const char *filename,
				t_png_color_type	colortype, unsigned int bitdepth);
unsigned int	lodepng_decode32_file(unsigned char **out, unsigned int *w,
				unsigned int	*h, const char *filename);
unsigned int	lodepng_decode24_file(unsigned char **out, unsigned int *w,
				unsigned int	*h, const char *filename);
# endif

#endif