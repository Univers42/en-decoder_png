/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static size_t	hcl_fill_leaves(BPMNode *leaves,
				const unsigned *frequencies, size_t numcodes)
{
	size_t		numpresent;
	unsigned	i;

	numpresent = 0;
	i = 0;
	while (i != numcodes)
	{
		if (frequencies[i] > 0)
		{
			leaves[numpresent].weight = (int)frequencies[i];
			leaves[numpresent].index = i;
			++numpresent;
		}
		++i;
	}
	return (numpresent);
}

static void	hcl_handle_trivial(unsigned *lengths,
			BPMNode *leaves, size_t numpresent)
{
	if (numpresent == 0)
	{
		lengths[0] = 1;
		lengths[1] = 1;
	}
	else
	{
		lengths[leaves[0].index] = 1;
		if (leaves[0].index == 0)
			lengths[1] = 1;
		else
			lengths[0] = 1;
	}
}

unsigned	lodepng_huffman_code_lengths(unsigned *lengths,
			const unsigned *frequencies, size_t numcodes,
			unsigned maxbitlen)
{
	unsigned	i;
	size_t		numpresent;
	BPMNode		*leaves;

	if (numcodes == 0 || (1u << maxbitlen) < (unsigned)numcodes)
		return (80);
	leaves = (BPMNode *)lodepng_malloc(numcodes * sizeof(*leaves));
	if (!leaves)
		return (83);
	numpresent = hcl_fill_leaves(leaves, frequencies, numcodes);
	i = 0;
	while (i != numcodes)
		lengths[i++] = 0;
	if (numpresent <= 1)
		hcl_handle_trivial(lengths, leaves, numpresent);
	else
	{
		i = hcl_bpm(lengths, leaves, numpresent, maxbitlen);
		lodepng_free(leaves);
		return (i);
	}
	lodepng_free(leaves);
	return (0);
}

unsigned	HuffmanTree_makeFromFrequencies(HuffmanTree *tree,
			const unsigned *frequencies, size_t mincodes,
			size_t numcodes, unsigned maxbitlen)
{
	unsigned	error;

	while (!frequencies[numcodes - 1] && numcodes > mincodes)
		--numcodes;
	tree->max_bit_len = maxbitlen;
	tree->numcodes = (unsigned)numcodes;
	tree->lengths = (unsigned *)lodepng_realloc(tree->lengths,
			numcodes * sizeof(unsigned));
	if (!tree->lengths)
		return (83);
	memset(tree->lengths, 0, numcodes * sizeof(unsigned));
	error = lodepng_huffman_code_lengths(tree->lengths,
			frequencies, numcodes, maxbitlen);
	if (!error)
		error = HuffmanTree_makeFromLengths2(tree);
	return (error);
}

unsigned	huffmanDecodeSymbol(const unsigned char *in, size_t *bp,
			const HuffmanTree *codetree, size_t inbitlength)
{
	unsigned	treepos;
	unsigned	ct;

	treepos = 0;
	while (1)
	{
		if (*bp >= inbitlength)
			return ((unsigned)(-1));
		ct = codetree->tree2d[(treepos << 1) + READBIT(*bp, in)];
		++(*bp);
		if (ct < codetree->numcodes)
			return (ct);
		treepos = ct - codetree->numcodes;
		if (treepos >= codetree->numcodes)
			return ((unsigned)(-1));
	}
}
