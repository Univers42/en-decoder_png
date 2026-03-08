/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uivector.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 00:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	uivector_init(uivector *v)
{
	v->data = NULL;
	v->size = 0;
	v->allocsize = 0;
}

int	uivector_resize(uivector *v, size_t s)
{
	size_t			n;
	unsigned int	*p;

	if (s > v->allocsize)
	{
		n = s;
		if (!n)
			n = 1;
		p = (unsigned int *)lodepng_realloc(v->data, n * sizeof(unsigned int));
		if (!p)
			return (0);
		v->data = p;
		v->allocsize = n;
	}
	v->size = s;
	return (1);
}

int	uivector_resizev(uivector *v, size_t s, unsigned int val)
{
	size_t	i;

	if (!uivector_resize(v, s))
		return (0);
	i = 0;
	while (i < s)
	{
		v->data[i] = val;
		i++;
	}
	return (1);
}

int	uivector_push_back(uivector *v, unsigned int val)
{
	if (v->size + 1 > v->allocsize)
	{
		if (!uivector_resize(v, v->size + 1))
			return (0);
	}
	v->data[v->size] = val;
	v->size++;
	return (1);
}

void	uivector_cleanup(uivector *v)
{
	lodepng_free(v->data);
	v->data = NULL;
	v->size = 0;
	v->allocsize = 0;
}
