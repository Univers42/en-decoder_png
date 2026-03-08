/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:20:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	addcolorbits_mask(unsigned int bits)
{
	if (bits == 1)
		return (7);
	if (bits == 2)
		return (3);
	return (1);
}

void	addColorBits(unsigned char *out, size_t index,
		unsigned int bits, unsigned int in)
{
	unsigned int	m;
	unsigned int	p;

	m = addcolorbits_mask(bits);
	p = index & m;
	in &= (1u << bits) - 1u;
	in = in << (bits * (m - p));
	if (p == 0)
		out[index * bits / 8] = in;
	else
		out[index * bits / 8] |= in;
}

void	color_tree_init(ColorTree *tree)
{
	int	i;

	i = 0;
	while (i != 16)
	{
		tree->children[i] = 0;
		i++;
	}
	tree->index = -1;
}

void	color_tree_cleanup(ColorTree *tree)
{
	int	i;

	i = 0;
	while (i != 16)
	{
		if (tree->children[i])
		{
			color_tree_cleanup(tree->children[i]);
			lodepng_free(tree->children[i]);
		}
		i++;
	}
}

static int	color_tree_child_index(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a, int bit)
{
	return (8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1)
		+ 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1));
}

int	color_tree_get(ColorTree *tree, unsigned char r,
		unsigned char g, unsigned char b, unsigned char a)
{
	int	bit;
	int	i;

	bit = 0;
	while (bit < 8)
	{
		i = color_tree_child_index(r, g, b, a, bit);
		if (!tree->children[i])
			return (-1);
		tree = tree->children[i];
		bit++;
	}
	if (tree)
		return (tree->index);
	return (-1);
}
