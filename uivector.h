/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uivector.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:44:32 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 22:44:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef UIVECTOR_H
#  define UIVECTOR_H

#  include <stddef.h>
#  include "types.h"
#  include "utils.h"

#  ifndef UIVECTOR_DEFINED
#   define UIVECTOR_DEFINED

typedef struct s_uivector
{
	unsigned int	*data;
	size_t			size;
	size_t			allocsize;
}	uivector;

#  endif

void	uivector_init(uivector *v);
int		uivector_resize(uivector *v, size_t s);
int		uivector_resizev(uivector *v, size_t s, unsigned int val);
int		uivector_push_back(uivector *v, unsigned int val);
void	uivector_cleanup(uivector *v);

# endif