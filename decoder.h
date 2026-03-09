/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:37:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECODER_H
# define DECODER_H

# include "types.h"

void			dg_handle_chunk(t_png_state *st, t_ucvector *idat,
					const unsigned char *ch, unsigned int *out);
void			dg_chunk_loop(t_png_state *state, t_ucvector *idat,
					const unsigned char *in, size_t insize);
void			dg_postprocess(t_png_state *state, unsigned char **out,
					t_ucvector *scanlines);
void			dg_decompress(t_png_state *state, unsigned char **out,
					t_ucvector *idat);
void			decode_generic(unsigned char **out, t_png_state *state,
					const unsigned char *in, size_t insize);
unsigned int	lodepng_decode(unsigned char **out, t_png_state *state,
					const unsigned char *in, size_t insize);
unsigned int	lodepng_decode32_file(unsigned char **out, unsigned int *w,
					unsigned int *h, const char *filename);
unsigned int	lodepng_decode24_file(unsigned char **out, unsigned int *w,
					unsigned int *h, const char *filename);

#endif