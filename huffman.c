/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	HuffmanTree_init(HuffmanTree *tree)
{
	tree->tree2d = 0;
	tree->tree1d = 0;
	tree->lengths = 0;
}

void	HuffmanTree_cleanup(HuffmanTree *tree)
{
	lodepng_free(tree->tree2d);
	lodepng_free(tree->tree1d);
	lodepng_free(tree->lengths);
}

unsigned	HuffmanTree_getCode(const HuffmanTree *tree, unsigned index)
{
	return (tree->tree1d[index]);
}

unsigned	HuffmanTree_getLength(const HuffmanTree *tree, unsigned index)
{
	return (tree->lengths[index]);
}

void	addHuffmanSymbol(size_t *bp, ucvector *compressed,
		unsigned code, unsigned bitlen)
{
	addBitsToStreamReversed(bp, compressed, code, bitlen);
}
