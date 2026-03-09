/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:32:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	huffman_tree_init(t_huffman_tree *tree)
{
	tree->tree2d = 0;
	tree->tree1d = 0;
	tree->lengths = 0;
}

void	huffman_tree_cleanup(t_huffman_tree *tree)
{
	lodepng_free(tree->tree2d);
	lodepng_free(tree->tree1d);
	lodepng_free(tree->lengths);
}

unsigned int	huffman_tree_get_code(const t_huffman_tree *tree,
				unsigned int index)
{
	return (tree->tree1d[index]);
}

unsigned int	huffman_tree_get_length(const t_huffman_tree *tree,
				unsigned int index)
{
	return (tree->lengths[index]);
}

void	add_huffman_symbol(size_t *bp, t_ucvector *compressed,
		unsigned int code, unsigned int bitlen)
{
	add_bits_to_stream_rev(bp, compressed, code, bitlen);
}
