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

static const unsigned	LENGTHBASE[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13,
	15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163,
	195, 227, 258};
static const unsigned	LENGTHEXTRA[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
static const unsigned	DISTANCEBASE[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17,
	25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049,
	3073, 4097, 6145, 8193, 12289, 16385, 24577};
static const unsigned	DISTANCEEXTRA[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3,
	4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
# endif

void		HuffmanTree_init(HuffmanTree *tree);
void		HuffmanTree_cleanup(HuffmanTree *tree);
unsigned	HuffmanTree_getCode(const HuffmanTree *tree,
				unsigned index);
unsigned	HuffmanTree_getLength(const HuffmanTree *tree,
				unsigned index);
void		addHuffmanSymbol(size_t *bp, ucvector *compressed,
				unsigned code, unsigned bitlen);
unsigned	HuffmanTree_make2DTree(HuffmanTree *tree);
unsigned	HuffmanTree_makeFromLengths2(HuffmanTree *tree);
unsigned	HuffmanTree_makeFromLengths(HuffmanTree *tree,
				const unsigned *bitlen, size_t numcodes,
				unsigned maxbitlen);
unsigned	hcl_bpm(unsigned *lengths, BPMNode *leaves,
				size_t numpresent, unsigned maxbitlen);
unsigned	lodepng_huffman_code_lengths(unsigned *lengths,
				const unsigned *frequencies, size_t numcodes,
				unsigned maxbitlen);
unsigned	HuffmanTree_makeFromFrequencies(HuffmanTree *tree,
				const unsigned *frequencies, size_t mincodes,
				size_t numcodes, unsigned maxbitlen);
unsigned	huffmanDecodeSymbol(const unsigned char *in, size_t *bp,
				const HuffmanTree *codetree, size_t inbitlength);
unsigned	getTreeInflateDynamic(HuffmanTree *tree_ll,
				HuffmanTree *tree_d, const unsigned char *in,
				size_t *bp, size_t inlength);
unsigned	inflateHuffmanBlock(ucvector *out,
				const unsigned char *in, size_t *bp,
				size_t *pos, size_t inlength, unsigned btype);

#endif