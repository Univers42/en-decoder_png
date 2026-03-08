/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:32 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:11:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCODER_H
# define ENCODER_H

typedef struct s_lz77_ctx
{
	uivector			*out;
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
	ucvector			outv;
}	t_enc_ctx;

/* encoder.c - LZ77 search helpers */
unsigned int		lz77_init(t_lz77_ctx *ctx);
void			lz77_hash_pos(t_lz77_ctx *ctx);
void			lz77_chain_search(t_lz77_ctx *ctx);

/* encoder2.c - LZ77 lazy + emit + main */
void			lz77_emit(t_lz77_ctx *ctx);
unsigned int		encode_lz77(uivector *out, t_hash *hash,
					const unsigned char *in, size_t inpos,
					size_t insize, unsigned int windowsize,
					unsigned int minmatch, unsigned int nicematch,
					unsigned int	lazymatching);

/* encoder3.c - write_lz77_data + encode validation */
void			write_lz77_data(size_t *bp, ucvector *out,
					const uivector *lz77_encoded,
					const t_huffman_tree *tree_ll,
					const t_huffman_tree	*tree_d);
unsigned int		enc_validate(t_enc_ctx *ctx);

/* palette.c */
void			add_length_distance(uivector *out, unsigned int length,
					unsigned int	distance);

/* encoder4.c - encode color setup */
unsigned int		enc_auto_convert(t_enc_ctx *ctx);
unsigned int		enc_icc_check(t_enc_ctx *ctx);
unsigned int		enc_convert_data(t_enc_ctx *ctx);

/* encoder5.c - chunk writing */
unsigned int		enc_write_pre_idat(t_enc_ctx *ctx);
unsigned int		enc_write_idat_anc(t_enc_ctx *ctx);
unsigned int		enc_write_post(t_enc_ctx *ctx);

/* encoder6.c - text chunk writing */
unsigned int		enc_write_text(t_enc_ctx *ctx);
unsigned int		enc_write_itext_end(t_enc_ctx *ctx);

/* encoder7.c - encode main + wrappers */
unsigned int		lodepng_encode(unsigned char **out, size_t *outsize,
					const unsigned char *image, unsigned int w,
					unsigned int	h, t_png_state *state);
unsigned int		lodepng_encode_memory(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned int w, unsigned int h,
					t_png_color_type	colortype, unsigned int bitdepth);
unsigned int		lodepng_encode32(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned int	w, unsigned int h);
unsigned int		lodepng_encode24(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned int	w, unsigned int h);

/* encoder8.c - disk operations */
# ifdef LODEPNG_COMPILE_DISK
unsigned int		lodepng_encode_file(const char *filename,
					const unsigned char *image, unsigned int w,
					unsigned int	h, t_png_color_type ct, unsigned int bd);
unsigned int		lodepng_encode32_file(const char *filename,
					const unsigned char *image, unsigned int w,
					unsigned int	h);
unsigned int		lodepng_encode24_file(const char *filename,
					const unsigned char *image, unsigned int w,
					unsigned int	h);
# endif

#endif