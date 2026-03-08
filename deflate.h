/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFLATE_H
# define DEFLATE_H

# include "types.h"

# define NUM_CODE_LENGTH_CODES 19
# define FIRST_LENGTH_CODE_INDEX 257
# define LAST_LENGTH_CODE_INDEX 285

static const unsigned int	g_clcl_order[NUM_CODE_LENGTH_CODES] = {
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

typedef struct s_dd_ctx
{
	uivector		lz77_encoded;
	t_huffman_tree	tree_ll;
	t_huffman_tree	tree_d;
	t_huffman_tree	tree_cl;
	uivector		freq_ll;
	uivector		freq_d;
	uivector		freq_cl;
	uivector		bitlen_lld;
	uivector		bitlen_lld_e;
	uivector		bitlen_cl;
	size_t			numcodes_ll;
	size_t			numcodes_d;
}	t_dd_ctx;

void		dd_ctx_init(t_dd_ctx *ctx);
void		dd_ctx_cleanup(t_dd_ctx *ctx);
unsigned int	dd_lz77_encode(t_dd_ctx *ctx, t_hash *hash,
				const unsigned char *data, size_t datapos,
				size_t	dataend, const t_compress_settings *s);
unsigned int	dd_build_freq(t_dd_ctx *ctx);
unsigned int	dd_build_trees(t_dd_ctx *ctx);
void		dd_build_bitlen(t_dd_ctx *ctx);
void		dd_rle_encode(t_dd_ctx *ctx);
unsigned int	dd_freq_cl(t_dd_ctx *ctx);
unsigned int	dd_build_cl(t_dd_ctx *ctx);
void		dd_write_header(ucvector *out, size_t *bp,
				t_dd_ctx	*ctx, unsigned int bfinal);
void		dd_write_cls(ucvector *out, size_t *bp, t_dd_ctx *ctx);
unsigned int	dd_write_data(ucvector *out, size_t *bp, t_dd_ctx *ctx);
unsigned int	dd_emit(ucvector *out, size_t *bp, t_dd_ctx *ctx,
				unsigned int	final);
unsigned int	deflate_dynamic(ucvector *out, size_t *bp, t_hash *hash,
				const unsigned char *data, size_t datapos,
				size_t dataend, const t_compress_settings *s,
				unsigned int	final);
unsigned int	deflate_no_compression(ucvector *out,
				const unsigned char	*data, size_t datasize);
unsigned int	df_lz77(ucvector *out, size_t *bp, t_hash *hash,
				const unsigned char *data, size_t datapos,
				size_t dataend, const t_compress_settings *s,
				t_huffman_tree	*tree_ll, t_huffman_tree *tree_d);
void		df_literal(ucvector *out, size_t *bp,
				const unsigned char *data, size_t datapos,
				size_t	dataend, t_huffman_tree *tree_ll);
unsigned int	deflate_fixed(ucvector *out, size_t *bp, t_hash *hash,
				const unsigned char *data, size_t datapos,
				size_t dataend, const t_compress_settings *s,
				unsigned int	final);
unsigned int	lodepng_deflatev(ucvector *out, const unsigned char *in,
				size_t	insize, const t_compress_settings *s);
unsigned int	lodepng_deflate(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const t_compress_settings	*settings);
unsigned int	deflate(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const t_compress_settings	*settings);
void		gen_fixed_litlen_tree(t_huffman_tree *tree);
void		gen_fixed_dist_tree(t_huffman_tree *tree);
void		get_tree_inflate_fixed(t_huffman_tree *tree_ll,
				t_huffman_tree	*tree_d);

#endif
