/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ucvector3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:12:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	ucvector_cleanup(t_ucvector *v)
{
	lodepng_free(v->data);
	v->data = NULL;
	v->size = 0;
	v->allocsize = 0;
}
