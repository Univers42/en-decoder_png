/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned	hcl_init_lists(BPMLists *lists, unsigned maxbitlen)
{
	lists->listsize = maxbitlen;
	lists->memsize = 2 * maxbitlen * (maxbitlen + 1);
	lists->nextfree = 0;
	lists->numfree = lists->memsize;
	lists->memory = (BPMNode *)lodepng_malloc(
			lists->memsize * sizeof(*lists->memory));
	lists->freelist = (BPMNode **)lodepng_malloc(
			lists->memsize * sizeof(BPMNode *));
	lists->chains0 = (BPMNode **)lodepng_malloc(
			lists->listsize * sizeof(BPMNode *));
	lists->chains1 = (BPMNode **)lodepng_malloc(
			lists->listsize * sizeof(BPMNode *));
	if (!lists->memory || !lists->freelist
		|| !lists->chains0 || !lists->chains1)
		return (83);
	return (0);
}

static void	hcl_run_bpm(BPMLists *lists, BPMNode *leaves,
			size_t numpresent, unsigned maxbitlen)
{
	unsigned	i;

	i = 0;
	while (i != lists->memsize)
	{
		lists->freelist[i] = &lists->memory[i];
		++i;
	}
	bpmnode_create(lists, leaves[0].weight, 1, 0);
	bpmnode_create(lists, leaves[1].weight, 2, 0);
	i = 0;
	while (i != lists->listsize)
	{
		lists->chains0[i] = &lists->memory[0];
		lists->chains1[i] = &lists->memory[1];
		++i;
	}
	i = 2;
	while (i != 2 * numpresent - 2)
	{
		boundaryPM(lists, leaves, numpresent,
			(int)maxbitlen - 1, (int)i);
		++i;
	}
}

static void	hcl_extract_lengths(BPMLists *lists, BPMNode *leaves,
			unsigned maxbitlen, unsigned *lengths)
{
	BPMNode		*node;
	unsigned	i;

	node = lists->chains1[maxbitlen - 1];
	while (node)
	{
		i = 0;
		while (i != node->index)
		{
			++lengths[leaves[i].index];
			++i;
		}
		node = node->tail;
	}
}

unsigned	hcl_bpm(unsigned *lengths, BPMNode *leaves,
			size_t numpresent, unsigned maxbitlen)
{
	unsigned	error;
	BPMLists	lists;

	bpmnode_sort(leaves, numpresent);
	error = hcl_init_lists(&lists, maxbitlen);
	if (!error)
	{
		hcl_run_bpm(&lists, leaves, numpresent, maxbitlen);
		hcl_extract_lengths(&lists, leaves, maxbitlen, lengths);
	}
	lodepng_free(lists.memory);
	lodepng_free(lists.freelist);
	lodepng_free(lists.chains0);
	lodepng_free(lists.chains1);
	return (error);
}
