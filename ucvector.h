/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ucvector.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:14:47 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 22:14:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef UCVECTOR_H
#  define UCVECTOR_H

#  include <stddef.h>
#  include "types.h"
#  include "utils.h"

#  ifndef UCVECTOR_DEFINED
#   define UCVECTOR_DEFINED

typedef struct s_ucvector
{
	unsigned char	*data;
	size_t			size;
	size_t			allocsize;
}	ucvector;

#  endif

void	ucvector_init(ucvector *v);
void	ucvector_init_buffer(ucvector *v, unsigned char *d, size_t s);
int		ucvector_resize(ucvector *v, size_t s);
int		ucvector_reserve(ucvector *v, size_t s);
int		ucvector_push_back(ucvector *v, unsigned char c);
void	ucvector_cleanup(ucvector *v);

# endif
