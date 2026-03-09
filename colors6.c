/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int	color_tree_has(t_color_tree *tree, const unsigned char *rgba)
{
	return (color_tree_get(tree, rgba) >= 0);
}

static int	ct_child_idx(const unsigned char *rgba, int bit)
{
	return (8 * ((rgba[0] >> bit) & 1) + 4 * ((rgba[1] >> bit) & 1)
		+ 2 * ((rgba[2] >> bit) & 1) + 1 * ((rgba[3] >> bit) & 1));
}

void	color_tree_add(t_color_tree *tree, const unsigned char *rgba,
		unsigned int index)
{
	int	bit;
	int	i;

	bit = 0;
	while (bit < 8)
	{
		i = ct_child_idx(rgba, bit);
		if (!tree->children[i])
		{
			tree->children[i] = (t_color_tree *)lodepng_malloc(
					sizeof(t_color_tree));
			color_tree_init(tree->children[i]);
		}
		tree = tree->children[i];
		bit++;
	}
	tree->index = (int)index;
}

static int	abs_diff(int a, int b)
{
	if (a > b)
		return (a - b);
	return (b - a);
}

unsigned char	paeth_predictor(int a, int b, int c)
{
	int	p;
	int	pa;
	int	pb;
	int	pc;

	p = a + b - c;
	pa = abs_diff(p, a);
	pb = abs_diff(p, b);
	pc = abs_diff(p, c);
	if (pa <= pb && pa <= pc)
		return ((unsigned char)a);
	if (pb <= pc)
		return ((unsigned char)b);
	return ((unsigned char)c);
}
