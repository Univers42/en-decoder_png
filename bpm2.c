/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bpm2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:05:28 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:03:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	sort_merge(t_bpm_node *dst, t_bpm_node *src,
		size_t lo_mid[2], size_t hi)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = lo_mid[0];
	j = lo_mid[1];
	k = lo_mid[0];
	while (k < hi)
	{
		if (i < lo_mid[1]
			&& (j >= hi || src[i].weight <= src[j].weight))
			dst[k++] = src[i++];
		else
			dst[k++] = src[j++];
	}
}

static void	sort_pass(t_bpm_node *dst, t_bpm_node *src,
		size_t width, size_t num)
{
	size_t	i;
	size_t	lo_mid[2];
	size_t	hi;

	i = 0;
	while (i < num)
	{
		lo_mid[0] = i;
		lo_mid[1] = i + width;
		hi = i + 2 * width;
		if (lo_mid[1] > num)
			lo_mid[1] = num;
		if (hi > num)
			hi = num;
		sort_merge(dst, src, lo_mid, hi);
		i += 2 * width;
	}
}

static void	sort_copy(t_bpm_node *dst, t_bpm_node *src, size_t num)
{
	size_t	i;

	i = 0;
	while (i < num)
	{
		dst[i] = src[i];
		++i;
	}
}

void	bpmnode_sort(t_bpm_node *leaves, size_t num)
{
	t_bpm_node	*mem;
	size_t		width;
	int			toggle;

	if (num <= 1)
		return ;
	mem = (t_bpm_node *)lodepng_malloc(sizeof(t_bpm_node) * num);
	if (!mem)
		return ;
	width = 1;
	toggle = 0;
	while (width < num)
	{
		if (toggle)
			sort_pass(leaves, mem, width, num);
		else
			sort_pass(mem, leaves, width, num);
		toggle = !toggle;
		width *= 2;
	}
	if (toggle)
		sort_copy(leaves, mem, num);
	lodepng_free(mem);
}
