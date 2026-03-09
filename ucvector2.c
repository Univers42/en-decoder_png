/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ucvector2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:12:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	ucvector_init(t_ucvector *v)
{
	v->data = NULL;
	v->size = 0;
	v->allocsize = 0;
}

void	ucvector_init_buffer(t_ucvector *v, unsigned char *d, size_t s)
{
	v->data = d;
	v->size = s;
	v->allocsize = s;
}

int	ucvector_resize(t_ucvector *v, size_t s)
{
	size_t			n;
	unsigned char	*p;

	if (s > v->allocsize)
	{
		n = s;
		if (!n)
			n = 1;
		p = (unsigned char *)lodepng_realloc(v->data, n);
		if (!p)
			return (0);
		v->data = p;
		v->allocsize = n;
	}
	v->size = s;
	return (1);
}

int	ucvector_reserve(t_ucvector *v, size_t s)
{
	if (s <= v->allocsize)
		return (1);
	return (ucvector_resize(v, s));
}

int	ucvector_push_back(t_ucvector *v, unsigned char c)
{
	if (v->size + 1 > v->allocsize)
	{
		if (!ucvector_resize(v, v->size + 1))
			return (0);
	}
	v->data[v->size] = c;
	v->size++;
	return (1);
}
