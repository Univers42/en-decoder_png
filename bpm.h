/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bpm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:05:28 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:05:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BPM_H
# define BPM_H

# include <stddef.h>
# include "utils.h"

typedef struct s_bpm_node
{
	int					weight;
	unsigned			index;
	struct s_bpm_node	*tail;
	int					in_use;
}	BPMNode;

typedef struct s_bpm_lists
{
	unsigned	listsize;
	unsigned	memsize;
	unsigned	numfree;
	unsigned	nextfree;
	BPMNode		*memory;
	BPMNode		**freelist;
	BPMNode		**chains0;
	BPMNode		**chains1;
}	BPMLists;

BPMNode		*bpmnode_create(BPMLists *lists, int weight,
				unsigned index, BPMNode *tail);
void		bpmnode_sort(BPMNode *leaves, size_t num);
void		boundaryPM(BPMLists *lists, BPMNode *leaves,
				size_t numpresent, int c, int num);

#endif
