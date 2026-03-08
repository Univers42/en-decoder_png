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
	Hash				*hash;
	const unsigned char	*in;
	size_t				insize;
	unsigned			windowsize;
	unsigned			minmatch;
	unsigned			nicematch;
	unsigned			lazymatching;
	unsigned			maxchainlength;
	unsigned			maxlazymatch;
	unsigned			numzeros;
	unsigned			length;
	unsigned			offset;
	unsigned			lazy;
	unsigned			lazylength;
	unsigned			lazyoffset;
	unsigned			hashval;
	unsigned			hashpos;
	unsigned			error;
	size_t				pos;
	size_t				wpos;
	const unsigned char	*lastptr;
}	t_lz77_ctx;

typedef struct s_enc_ctx
{
	LodePNGState		*state;
	LodePNGInfo			info;
	const unsigned char	*image;
	unsigned			w;
	unsigned			h;
	unsigned char		*data;
	size_t				datasize;
	ucvector			outv;
}	t_enc_ctx;

/* encoder.c - LZ77 search helpers */
unsigned		lz77_init(t_lz77_ctx *ctx);
void			lz77_hash_pos(t_lz77_ctx *ctx);
void			lz77_chain_search(t_lz77_ctx *ctx);

/* encoder2.c - LZ77 lazy + emit + main */
void			lz77_emit(t_lz77_ctx *ctx);
unsigned		encodeLZ77(uivector *out, Hash *hash,
					const unsigned char *in, size_t inpos,
					size_t insize, unsigned windowsize,
					unsigned minmatch, unsigned nicematch,
					unsigned lazymatching);

/* encoder3.c - writeLZ77data + encode validation */
void			writeLZ77data(size_t *bp, ucvector *out,
					const uivector *lz77_encoded,
					const HuffmanTree *tree_ll,
					const HuffmanTree *tree_d);
unsigned		enc_validate(t_enc_ctx *ctx);

/* palette.c */
void			addLengthDistance(uivector *out, unsigned length,
					unsigned distance);

/* encoder4.c - encode color setup */
unsigned		enc_auto_convert(t_enc_ctx *ctx);
unsigned		enc_icc_check(t_enc_ctx *ctx);
unsigned		enc_convert_data(t_enc_ctx *ctx);

/* encoder5.c - chunk writing */
unsigned		enc_write_pre_idat(t_enc_ctx *ctx);
unsigned		enc_write_idat_anc(t_enc_ctx *ctx);
unsigned		enc_write_post(t_enc_ctx *ctx);

/* encoder6.c - text chunk writing */
unsigned		enc_write_text(t_enc_ctx *ctx);
unsigned		enc_write_itext_end(t_enc_ctx *ctx);

/* encoder7.c - encode main + wrappers */
unsigned		lodepng_encode(unsigned char **out, size_t *outsize,
					const unsigned char *image, unsigned w,
					unsigned h, LodePNGState *state);
unsigned		lodepng_encode_memory(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned w, unsigned h,
					LodePNGColorType colortype, unsigned bitdepth);
unsigned		lodepng_encode32(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned w, unsigned h);
unsigned		lodepng_encode24(unsigned char **out,
					size_t *outsize, const unsigned char *image,
					unsigned w, unsigned h);

/* encoder8.c - disk operations */
# ifdef LODEPNG_COMPILE_DISK
unsigned		lodepng_encode_file(const char *filename,
					const unsigned char *image, unsigned w,
					unsigned h, LodePNGColorType ct, unsigned bd);
unsigned		lodepng_encode32_file(const char *filename,
					const unsigned char *image, unsigned w,
					unsigned h);
unsigned		lodepng_encode24_file(const char *filename,
					const unsigned char *image, unsigned w,
					unsigned h);
# endif

#endif