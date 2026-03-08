/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:02:18 by marvin           ###   ########.fr       */
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

static const unsigned int	g_lengthbase[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13,
	15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163,
	195, 227, 258};
static const unsigned int	g_lengthextra[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
static const unsigned int	g_distancebase[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17,
	25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049,
	3073, 4097, 6145, 8193, 12289, 16385, 24577};
static const unsigned int	g_distanceextra[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3,
	4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
# endif

void		huffman_tree_init(t_huffman_tree *tree);
void		huffman_tree_cleanup(t_huffman_tree *tree);
unsigned int	huffman_tree_get_code(const t_huffman_tree *tree,
				unsigned int index);
unsigned int	huffman_tree_get_length(const t_huffman_tree *tree,
				unsigned int index);
void		add_huffman_symbol(size_t *bp, ucvector *compressed,
				unsigned int code, unsigned int bitlen);
unsigned int	huffman_tree_make_2d_tree(t_huffman_tree *tree);
unsigned int	huffman_tree_make_from_len2(t_huffman_tree *tree);
unsigned int	huffman_tree_make_from_len(t_huffman_tree *tree,
				const unsigned int *bitlen, size_t numcodes,
				unsigned int maxbitlen);
unsigned int	hcl_bpm(unsigned int *lengths, t_bpm_node *leaves,
				size_t numpresent, unsigned int maxbitlen);
unsigned int	lodepng_huffman_code_lengths(unsigned int *lengths,
				const unsigned int *frequencies, size_t numcodes,
				unsigned int maxbitlen);
unsigned int	huffman_tree_make_from_freq(t_huffman_tree *tree,
				const unsigned int *frequencies, size_t mincodes,
				size_t numcodes, unsigned int maxbitlen);
unsigned int	huffman_decode_symbol(const unsigned char *in, size_t *bp,
				const t_huffman_tree *codetree, size_t inbitlength);
unsigned int	get_tree_inflate_dynamic(t_huffman_tree *tree_ll,
				t_huffman_tree *tree_d, const unsigned char *in,
				size_t *bp, size_t inlength);
unsigned int	inflate_huffman_block(ucvector *out,
				const unsigned char *in, size_t *bp,
				size_t *pos, size_t inlength, unsigned int btype);

#endif