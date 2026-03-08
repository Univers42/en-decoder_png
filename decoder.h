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

void		dg_handle_chunk(LodePNGState *st, ucvector *idat,
				const unsigned char *ch, unsigned *iend,
				unsigned *unk, unsigned *cpos);
void		dg_chunk_loop(LodePNGState *state, ucvector *idat,
				const unsigned char *in, size_t insize);
void		dg_postprocess(LodePNGState *state, unsigned char **out,
				ucvector *scanlines, unsigned w, unsigned h);
void		dg_decompress(LodePNGState *state, unsigned char **out,
				ucvector *idat, unsigned w, unsigned h);
void		decodeGeneric(unsigned char **out, unsigned *w, unsigned *h,
				LodePNGState *state,
				const unsigned char *in, size_t insize);
unsigned	lodepng_decode(unsigned char **out, unsigned *w, unsigned *h,
				LodePNGState *state,
				const unsigned char *in, size_t insize);
unsigned	lodepng_decode_memory(unsigned char **out, unsigned *w,
				unsigned *h, const unsigned char *in, size_t insize,
				LodePNGColorType colortype, unsigned bitdepth);
unsigned	lodepng_decode32_memory(unsigned char **out, unsigned *w,
				unsigned *h, const unsigned char *in, size_t insize);
unsigned	lodepng_decode32(unsigned char **out, unsigned *w,
				unsigned *h, const unsigned char *in, size_t insize);
unsigned	lodepng_decode24(unsigned char **out, unsigned *w,
				unsigned *h, const unsigned char *in, size_t insize);
# ifdef LODEPNG_COMPILE_DISK
unsigned	lodepng_decode_file(unsigned char **out, unsigned *w,
				unsigned *h, const char *filename,
				LodePNGColorType colortype, unsigned bitdepth);
unsigned	lodepng_decode32_file(unsigned char **out, unsigned *w,
				unsigned *h, const char *filename);
unsigned	lodepng_decode24_file(unsigned char **out, unsigned *w,
				unsigned *h, const char *filename);
# endif

#endif