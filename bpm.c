/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bpm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:05:28 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	bpmnode_gc_mark(t_bpm_lists *lists)
{
	unsigned int	i;
	t_bpm_node		*node;

	i = 0;
	while (i != lists->memsize)
		lists->memory[i++].in_use = 0;
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

static void	bpmnode_gc_sweep(t_bpm_lists *lists)
{
	unsigned int	i;

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

t_bpm_node	*bpmnode_create(t_bpm_lists *lists, int weight,
		unsigned int index, t_bpm_node *tail)
{
	t_bpm_node	*result;

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

static int	boundary_pm_base(t_bpm_lists *lists, int c)
{
	unsigned int	lastindex;

	lastindex = lists->chains1[c]->index;
	if (lastindex >= lists->numpresent)
		return (1);
	lists->chains0[c] = lists->chains1[c];
	lists->chains1[c] = bpmnode_create(lists,
			lists->leaves[lastindex].weight, lastindex + 1, 0);
	return (1);
}

void	boundary_pm(t_bpm_lists *lists, int c, int num)
{
	unsigned int	lastindex;
	int				sum;

	if (c == 0 && boundary_pm_base(lists, c))
		return ;
	lastindex = lists->chains1[c]->index;
	sum = lists->chains0[c - 1]->weight + lists->chains1[c - 1]->weight;
	lists->chains0[c] = lists->chains1[c];
	if (lastindex < lists->numpresent
		&& sum > lists->leaves[lastindex].weight)
	{
		lists->chains1[c] = bpmnode_create(lists,
				lists->leaves[lastindex].weight, lastindex + 1,
				lists->chains1[c]->tail);
		return ;
	}
	lists->chains1[c] = bpmnode_create(lists, sum, lastindex,
			lists->chains1[c - 1]);
	if (num + 1 < (int)(2 * lists->numpresent - 2))
	{
		boundary_pm(lists, c - 1, num);
		boundary_pm(lists, c - 1, num);
	}
}
