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

static void	make_lengths2_generate(t_huffman_tree *tree,
			uivector *blcount, uivector *nextcode)
{
	unsigned int	bits;
	unsigned int	n;

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

unsigned int	huffman_tree_make_from_len2(t_huffman_tree *tree)
{
	uivector	blcount;
	uivector	nextcode;
	unsigned int	error;

	error = 0;
	uivector_init(&blcount);
	uivector_init(&nextcode);
	tree->tree1d = (unsigned int *)lodepng_malloc(
			tree->numcodes * sizeof(unsigned int));
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
		return (huffman_tree_make_2d_tree(tree));
	return (error);
}

unsigned int	huffman_tree_make_from_len(t_huffman_tree *tree,
			const unsigned int *bitlen, size_t numcodes,
			unsigned int maxbitlen)
{
	unsigned int	i;

	tree->lengths = (unsigned int *)lodepng_malloc(
			numcodes * sizeof(unsigned int));
	if (!tree->lengths)
		return (83);
	i = 0;
	while (i != numcodes)
	{
		tree->lengths[i] = bitlen[i];
		++i;
	}
	tree->numcodes = (unsigned int)numcodes;
	tree->max_bit_len = maxbitlen;
	return (huffman_tree_make_from_len2(tree));
}
