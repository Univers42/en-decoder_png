/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	make_lengths2_generate(HuffmanTree *tree,
			uivector *blcount, uivector *nextcode)
{
	unsigned	bits;
	unsigned	n;

	bits = 0;
	while (bits != tree->numcodes)
	{
		++blcount->data[tree->lengths[bits]];
		++bits;
	}
	bits = 1;
	while (bits <= tree->max_bit_len)
	{
		nextcode->data[bits] = (nextcode->data[bits - 1]
				+ blcount->data[bits - 1]) << 1;
		++bits;
	}
	n = 0;
	while (n != tree->numcodes)
	{
		if (tree->lengths[n] != 0)
			tree->tree1d[n] = nextcode->data[tree->lengths[n]]++;
		++n;
	}
}

unsigned	HuffmanTree_makeFromLengths2(HuffmanTree *tree)
{
	uivector	blcount;
	uivector	nextcode;
	unsigned	error;

	error = 0;
	uivector_init(&blcount);
	uivector_init(&nextcode);
	tree->tree1d = (unsigned *)lodepng_malloc(
			tree->numcodes * sizeof(unsigned));
	if (!tree->tree1d)
		error = 83;
	if (!uivector_resizev(&blcount, tree->max_bit_len + 1, 0)
		|| !uivector_resizev(&nextcode, tree->max_bit_len + 1, 0))
		error = 83;
	if (!error)
		make_lengths2_generate(tree, &blcount, &nextcode);
	uivector_cleanup(&blcount);
	uivector_cleanup(&nextcode);
	if (!error)
		return (HuffmanTree_make2DTree(tree));
	return (error);
}

unsigned	HuffmanTree_makeFromLengths(HuffmanTree *tree,
			const unsigned *bitlen, size_t numcodes,
			unsigned maxbitlen)
{
	unsigned	i;

	tree->lengths = (unsigned *)lodepng_malloc(
			numcodes * sizeof(unsigned));
	if (!tree->lengths)
		return (83);
	i = 0;
	while (i != numcodes)
	{
		tree->lengths[i] = bitlen[i];
		++i;
	}
	tree->numcodes = (unsigned)numcodes;
	tree->max_bit_len = maxbitlen;
	return (HuffmanTree_makeFromLengths2(tree));
}
