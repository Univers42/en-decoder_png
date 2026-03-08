/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	make2d_clear_uninit(t_huffman_tree *tree)
{
	unsigned int	n;

	n = 0;
	while (n < tree->numcodes * 2)
	{
		if (tree->tree2d[n] == 32767)
			tree->tree2d[n] = 0;
		++n;
	}
}

static unsigned int	make2d_inner(t_huffman_tree *tree, unsigned int n,
				unsigned int *nodefilled, unsigned int *treepos)
{
	unsigned int		i;
	unsigned char	bit;

	i = 0;
	while (i != tree->lengths[n])
	{
		bit = (unsigned char)((tree->tree1d[n]
					>> (tree->lengths[n] - i - 1)) & 1);
		if (*treepos > 2147483647 || *treepos + 2 > tree->numcodes)
			return (55);
		if (tree->tree2d[2 * (*treepos) + bit] == 32767)
		{
			if (i + 1 == tree->lengths[n])
			{
				tree->tree2d[2 * (*treepos) + bit] = n;
				*treepos = 0;
			}
			else
			{
				++(*nodefilled);
				tree->tree2d[2 * (*treepos) + bit]
					= *nodefilled + tree->numcodes;
				*treepos = *nodefilled;
			}
		}
		else
			*treepos = tree->tree2d[2 * (*treepos) + bit]
				- tree->numcodes;
		++i;
	}
	return (0);
}

unsigned int	huffman_tree_make_2d_tree(t_huffman_tree *tree)
{
	unsigned int	nodefilled;
	unsigned int	treepos;
	unsigned int	n;
	unsigned int	error;

	nodefilled = 0;
	treepos = 0;
	tree->tree2d = (unsigned int *)lodepng_malloc(
			tree->numcodes * 2 * sizeof(unsigned int));
	if (!tree->tree2d)
		return (83);
	n = 0;
	while (n < tree->numcodes * 2)
		tree->tree2d[n++] = 32767;
	n = 0;
	while (n < tree->numcodes)
	{
		error = make2d_inner(tree, n, &nodefilled, &treepos);
		if (error)
			return (error);
		++n;
	}
	make2d_clear_uninit(tree);
	return (0);
}
