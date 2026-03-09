/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:32 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:37:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCODER_H
# define ENCODER_H

typedef struct s_lz77_ctx
{
	t_uivector			*out;
	t_hash				*hash;
	const unsigned char	*in;
	size_t				insize;
	unsigned int		windowsize;
	unsigned int		minmatch;
	unsigned int		nicematch;
	unsigned int		lazymatching;
	unsigned int		maxchainlength;
	unsigned int		maxlazymatch;
	unsigned int		numzeros;
	unsigned int		length;
	unsigned int		offset;
	unsigned int		lazy;
	unsigned int		lazylength;
	unsigned int		lazyoffset;
	unsigned int		hashval;
	unsigned int		hashpos;
	unsigned int		error;
	size_t				pos;
	size_t				wpos;
	const unsigned char	*lastptr;
}	t_lz77_ctx;

typedef struct s_enc_ctx
{
	t_png_state			*state;
	t_png_info			info;
	const unsigned char	*image;
	unsigned int		w;
	unsigned int		h;
	unsigned char		*data;
	size_t				datasize;
	t_ucvector			outv;
}	t_enc_ctx;

/* encoder.c - LZ77 search helpers */
unsigned int	lz77_init(t_lz77_ctx *ctx);
void			lz77_hash_pos(t_lz77_ctx *ctx);
void			lz77_try_match(t_lz77_ctx *ctx,
					unsigned int cur_off);

/* encoder9.c - LZ77 chain search */
void			lz77_chain_search(t_lz77_ctx *ctx);

/* encoder2.c - LZ77 lazy + emit + main */
void			lz77_emit(t_lz77_ctx *ctx);
unsigned int	encode_lz77(t_uivector *out,
					t_deflate_work *w);

/* encoder3.c - write_lz77_data + encode validation */
void			write_lz77_data(t_deflate_work *w,
					const t_uivector *lz77,
					const t_huffman_tree *tree_ll,
					const t_huffman_tree *tree_d);
unsigned int	enc_validate(t_enc_ctx *ctx);

/* palette.c */
void			add_length_distance(t_uivector *out,
					unsigned int length,
					unsigned int distance);

/* encoder4.c - encode color setup */
unsigned int	enc_auto_convert(t_enc_ctx *ctx);
unsigned int	enc_icc_check(t_enc_ctx *ctx);
unsigned int	enc_convert_data(t_enc_ctx *ctx);

/* encoder5.c - chunk writing */
unsigned int	enc_write_pre_idat(t_enc_ctx *ctx);
unsigned int	enc_write_idat_anc(t_enc_ctx *ctx);
unsigned int	enc_write_post(t_enc_ctx *ctx);

/* encoder6.c - text chunk writing */
unsigned int	enc_write_text(t_enc_ctx *ctx);
unsigned int	enc_write_itext_end(t_enc_ctx *ctx);

/* encoder7.c - encode main */
unsigned int	lodepng_encode(unsigned char **out,
					size_t *outsize, t_enc_ctx *ctx);

#endif