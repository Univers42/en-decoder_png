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
	unsigned int			index;
	struct s_bpm_node	*tail;
	int					in_use;
}	t_bpm_node;

typedef struct s_bpm_lists
{
	unsigned int	listsize;
	unsigned int	memsize;
	unsigned int	numfree;
	unsigned int	nextfree;
	t_bpm_node		*memory;
	t_bpm_node		**freelist;
	t_bpm_node		**chains0;
	t_bpm_node		**chains1;
}	t_bpm_lists;

t_bpm_node		*bpmnode_create(t_bpm_lists *lists, int weight,
				unsigned int index, t_bpm_node *tail);
void		bpmnode_sort(t_bpm_node *leaves, size_t num);
void		boundary_pm(t_bpm_lists *lists, t_bpm_node *leaves,
				size_t numpresent, int c, int num);

#endif
