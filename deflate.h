/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:19:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:02:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFLATE_H
# define DEFLATE_H

# include "types.h"

# define NUM_CODE_LENGTH_CODES 19
# define FIRST_LENGTH_CODE_INDEX 257
# define LAST_LENGTH_CODE_INDEX 285

const unsigned int	*clcl_order(void);

typedef struct s_dd_ctx
{
	t_uivector		lz77_encoded;
	t_huffman_tree	tree_ll;
	t_huffman_tree	tree_d;
	t_huffman_tree	tree_cl;
	t_uivector		freq_ll;
	t_uivector		freq_d;
	t_uivector		freq_cl;
	t_uivector		bitlen_lld;
	t_uivector		bitlen_lld_e;
	t_uivector		bitlen_cl;
	size_t			numcodes_ll;
	size_t			numcodes_d;
}	t_dd_ctx;

void				dd_ctx_init(t_dd_ctx *ctx);
void				dd_ctx_cleanup(t_dd_ctx *ctx);
unsigned int		dd_lz77_encode(t_dd_ctx *ctx,
						t_deflate_work *w);
unsigned int		dd_build_freq(t_dd_ctx *ctx);
unsigned int		dd_build_trees(t_dd_ctx *ctx);
void				dd_build_bitlen(t_dd_ctx *ctx);
void				dd_rle_encode(t_dd_ctx *ctx);
unsigned int		dd_freq_cl(t_dd_ctx *ctx);
unsigned int		dd_build_cl(t_dd_ctx *ctx);
void				dd_write_header(t_ucvector *out,
						size_t *bp, t_dd_ctx *ctx,
						unsigned int bfinal);
void				dd_write_cls(t_ucvector *out,
						size_t *bp, t_dd_ctx *ctx);
unsigned int		dd_write_data(t_deflate_work *w,
						t_dd_ctx *ctx);
unsigned int		dd_emit(t_deflate_work *w,
						t_dd_ctx *ctx,
						unsigned int final);
unsigned int		deflate_dynamic(t_deflate_work *w,
						unsigned int final);
unsigned int		deflate_no_compression(t_ucvector *out,
						const unsigned char *data,
						size_t datasize);
unsigned int		df_lz77(t_deflate_work *w,
						t_huffman_tree *tree_ll,
						t_huffman_tree *tree_d);
void				df_literal(t_deflate_work *w,
						t_huffman_tree *tree_ll);
unsigned int		deflate_fixed(t_deflate_work *w,
						unsigned int final);
unsigned int		lodepng_deflatev(t_ucvector *out,
						const unsigned char *in,
						size_t insize,
						const t_compress_settings *s);
void				gen_fixed_litlen_tree(
						t_huffman_tree *tree);
void				gen_fixed_dist_tree(
						t_huffman_tree *tree);
void				get_tree_inflate_fixed(
						t_huffman_tree *tree_ll,
						t_huffman_tree *tree_d);

#endif
