/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bpm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:05:28 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:56:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	bpmnode_gc_mark(BPMLists *lists)
{
	unsigned	i;
	BPMNode		*node;

	i = 0;
	while (i != lists->memsize)
	{
		lists->memory[i].in_use = 0;
		++i;
	}
	i = 0;
	while (i != lists->listsize)
	{
		node = lists->chains0[i];
		while (node != 0)
		{
			node->in_use = 1;
			node = node->tail;
		}
		node = lists->chains1[i];
		while (node != 0)
		{
			node->in_use = 1;
			node = node->tail;
		}
		++i;
	}
}

static void	bpmnode_gc_sweep(BPMLists *lists)
{
	unsigned	i;

	lists->numfree = 0;
	i = 0;
	while (i != lists->memsize)
	{
		if (!lists->memory[i].in_use)
			lists->freelist[lists->numfree++] = &lists->memory[i];
		++i;
	}
	lists->nextfree = 0;
}

BPMNode	*bpmnode_create(BPMLists *lists, int weight,
		unsigned index, BPMNode *tail)
{
	BPMNode	*result;

	if (lists->nextfree >= lists->numfree)
	{
		bpmnode_gc_mark(lists);
		bpmnode_gc_sweep(lists);
	}
	result = lists->freelist[lists->nextfree++];
	result->weight = weight;
	result->index = index;
	result->tail = tail;
	return (result);
}

void	boundaryPM(BPMLists *lists, BPMNode *leaves,
		size_t numpresent, int c, int num)
{
	unsigned	lastindex;
	int			sum;

	lastindex = lists->chains1[c]->index;
	if (c == 0)
	{
		if (lastindex >= numpresent)
			return ;
		lists->chains0[c] = lists->chains1[c];
		lists->chains1[c] = bpmnode_create(lists,
				leaves[lastindex].weight, lastindex + 1, 0);
		return ;
	}
	sum = lists->chains0[c - 1]->weight + lists->chains1[c - 1]->weight;
	lists->chains0[c] = lists->chains1[c];
	if (lastindex < numpresent && sum > leaves[lastindex].weight)
	{
		lists->chains1[c] = bpmnode_create(lists,
				leaves[lastindex].weight, lastindex + 1,
				lists->chains1[c]->tail);
		return ;
	}
	lists->chains1[c] = bpmnode_create(lists, sum, lastindex,
			lists->chains1[c - 1]);
	if (num + 1 < (int)(2 * numpresent - 2))
	{
		boundaryPM(lists, leaves, numpresent, c - 1, num);
		boundaryPM(lists, leaves, numpresent, c - 1, num);
	}
}
