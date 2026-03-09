/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uivector.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:44:32 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:28:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UIVECTOR_H
# define UIVECTOR_H

# include <stddef.h>
# include "types.h"
# include "utils.h"

# ifndef UIVECTOR_DEFINED
#  define UIVECTOR_DEFINED

typedef struct s_uivector
{
	unsigned int	*data;
	size_t			size;
	size_t			allocsize;
}	t_uivector;

# endif

void	uivector_init(t_uivector *v);
int		uivector_resize(t_uivector *v, size_t s);
int		uivector_resizev(t_uivector *v, size_t s, unsigned int val);
int		uivector_push_back(t_uivector *v, unsigned int val);
void	uivector_cleanup(t_uivector *v);

#endif
