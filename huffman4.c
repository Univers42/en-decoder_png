/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	hcl_init_lists(t_bpm_lists *lists, unsigned int maxbitlen)
{
	lists->listsize = maxbitlen;
	lists->memsize = 2 * maxbitlen * (maxbitlen + 1);
	lists->nextfree = 0;
	lists->numfree = lists->memsize;
	lists->memory = (t_bpm_node *)lodepng_malloc(
			lists->memsize * sizeof(*lists->memory));
	lists->freelist = (t_bpm_node **)lodepng_malloc(
			lists->memsize * sizeof(t_bpm_node *));
	lists->chains0 = (t_bpm_node **)lodepng_malloc(
			lists->listsize * sizeof(t_bpm_node *));
	lists->chains1 = (t_bpm_node **)lodepng_malloc(
			lists->listsize * sizeof(t_bpm_node *));
	if (!lists->memory || !lists->freelist
		|| !lists->chains0 || !lists->chains1)
		return (83);
	return (0);
}

static void	hcl_run_bpm(t_bpm_lists *lists,
			unsigned int maxbitlen)
{
	unsigned int	i;

	i = 0;
	while (i != lists->memsize)
	{
		lists->freelist[i] = &lists->memory[i];
		++i;
	}
	bpmnode_create(lists, lists->leaves[0].weight, 1, 0);
	bpmnode_create(lists, lists->leaves[1].weight, 2, 0);
	i = 0;
	while (i != lists->listsize)
	{
		lists->chains0[i] = &lists->memory[0];
		lists->chains1[i] = &lists->memory[1];
		++i;
	}
	i = 2;
	while (i != 2 * lists->numpresent - 2)
	{
		boundary_pm(lists, (int)maxbitlen - 1, (int)i);
		++i;
	}
}

static void	hcl_extract_lengths(t_bpm_lists *lists,
			unsigned int maxbitlen, unsigned int *lengths)
{
	t_bpm_node		*node;
	unsigned int	i;

	node = lists->chains1[maxbitlen - 1];
	while (node)
	{
		i = 0;
		while (i != node->index)
		{
			++lengths[lists->leaves[i].index];
			++i;
		}
		node = node->tail;
	}
}

unsigned int	hcl_bpm(unsigned int *lengths, t_bpm_node *leaves,
			size_t numpresent, unsigned int maxbitlen)
{
	unsigned int	error;
	t_bpm_lists		lists;

	bpmnode_sort(leaves, numpresent);
	error = hcl_init_lists(&lists, maxbitlen);
	if (!error)
	{
		lists.leaves = leaves;
		lists.numpresent = numpresent;
		hcl_run_bpm(&lists, maxbitlen);
		hcl_extract_lengths(&lists, maxbitlen, lengths);
	}
	lodepng_free(lists.memory);
	lodepng_free(lists.freelist);
	lodepng_free(lists.chains0);
	lodepng_free(lists.chains1);
	return (error);
}
