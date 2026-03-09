/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	color_tree_init(t_color_tree *tree)
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

void	color_tree_cleanup(t_color_tree *tree)
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

static int	color_tree_child_index(const unsigned char *rgba, int bit)
{
	return (8 * ((rgba[0] >> bit) & 1) + 4 * ((rgba[1] >> bit) & 1)
		+ 2 * ((rgba[2] >> bit) & 1) + 1 * ((rgba[3] >> bit) & 1));
}

int	color_tree_get(t_color_tree *tree, const unsigned char *rgba)
{
	int	bit;
	int	i;

	bit = 0;
	while (bit < 8)
	{
		i = color_tree_child_index(rgba, bit);
		if (!tree->children[i])
			return (-1);
		tree = tree->children[i];
		bit++;
	}
	if (tree)
		return (tree->index);
	return (-1);
}
