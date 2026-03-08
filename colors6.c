/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:21:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int	color_tree_has(ColorTree *tree, unsigned char r,
		unsigned char g, unsigned char b, unsigned char a)
{
	return (color_tree_get(tree, r, g, b, a) >= 0);
}

static int	ct_child_idx(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a, int bit)
{
	return (8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1)
		+ 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1));
}

void	color_tree_add(ColorTree *tree, unsigned char r,
		unsigned char g, unsigned char b,
		unsigned char a, unsigned int index)
{
	int	bit;
	int	i;

	bit = 0;
	while (bit < 8)
	{
		i = ct_child_idx(r, g, b, a, bit);
		if (!tree->children[i])
		{
			tree->children[i] = (ColorTree *)lodepng_malloc(
					sizeof(ColorTree));
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

unsigned char	paethPredictor(int a, int b, int c)
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
