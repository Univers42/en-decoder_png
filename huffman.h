/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUFFMAN_H
# define HUFFMAN_H

# include "types.h"
# include "bpm.h"

# ifndef NUM_CODE_LENGTH_CODES
#  define NUM_CODE_LENGTH_CODES 19
#  define FIRST_LENGTH_CODE_INDEX 257
#  define LAST_LENGTH_CODE_INDEX 285
# endif

const t_huff_tbl	*hft(void);
void				huffman_tree_init(t_huffman_tree *tree);
void				huffman_tree_cleanup(t_huffman_tree *tree);
unsigned int		huffman_tree_get_code(const t_huffman_tree *tree,
						unsigned int index);
unsigned int		huffman_tree_get_length(const t_huffman_tree *tree,
						unsigned int index);
void				add_huffman_symbol(size_t *bp,
						t_ucvector *compressed,
						unsigned int code, unsigned int bitlen);
unsigned int		huffman_tree_make_2d_tree(t_huffman_tree *tree);
unsigned int		huffman_tree_make_from_len2(t_huffman_tree *tree);
unsigned int		huffman_tree_make_from_len(t_huffman_tree *tree,
						const unsigned int *bitlen,
						size_t numcodes,
						unsigned int maxbitlen);
unsigned int		hcl_bpm(unsigned int *lengths,
						t_bpm_node *leaves,
						size_t numpresent,
						unsigned int maxbitlen);
unsigned int		lodepng_huffman_code_lengths(unsigned int *lengths,
						const unsigned int *frequencies,
						size_t numcodes,
						unsigned int maxbitlen);
unsigned int		huffman_tree_make_from_freq(t_huffman_tree *tree,
						const unsigned int *frequencies,
						size_t mincodes, size_t numcodes);
unsigned int		huffman_decode_symbol(const unsigned char *in,
						size_t *bp,
						const t_huffman_tree *codetree,
						size_t inbitlength);
unsigned int		get_tree_inflate_dynamic(t_huffman_tree *tree_ll,
						t_huffman_tree *tree_d,
						t_inflate_ctx *s);
unsigned int		inflate_huffman_block(t_inflate_ctx *s,
						unsigned int btype);

#endif