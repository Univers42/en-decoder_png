/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

const t_adam7_tbl	*a7t(void)
{
	static const t_adam7_tbl	tbl = {
	{0, 4, 0, 2, 0, 1, 0},
	{0, 0, 4, 0, 2, 0, 1},
	{8, 8, 4, 4, 2, 2, 1},
	{8, 8, 8, 4, 4, 2, 2}
	};

	return (&tbl);
}

static void	a7_compute_dims(t_adam7_passes *p)
{
	unsigned int	i;

	i = 0;
	while (i != 7)
	{
		p->passw[i] = (p->w + a7t()->dx[i] - a7t()->ix[i] - 1)
			/ a7t()->dx[i];
		p->passh[i] = (p->h + a7t()->dy[i] - a7t()->iy[i] - 1)
			/ a7t()->dy[i];
		if (p->passw[i] == 0)
			p->passh[i] = 0;
		if (p->passh[i] == 0)
			p->passw[i] = 0;
		++i;
	}
}

static void	a7_compute_starts(t_adam7_passes *p)
{
	unsigned int	i;
	size_t			fval;

	p->filter_passstart[0] = 0;
	p->padded_passstart[0] = 0;
	p->passstart[0] = 0;
	i = 0;
	while (i != 7)
	{
		if (p->passw[i] && p->passh[i])
			fval = p->passh[i]
				* (1 + (p->passw[i] * p->bpp + 7) / 8);
		else
			fval = 0;
		p->filter_passstart[i + 1] = p->filter_passstart[i] + fval;
		p->padded_passstart[i + 1] = p->padded_passstart[i]
			+ p->passh[i] * ((p->passw[i] * p->bpp + 7) / 8);
		p->passstart[i + 1] = p->passstart[i]
			+ (p->passh[i] * p->passw[i] * p->bpp + 7) / 8;
		++i;
	}
}

void	adam7_getpassvalues(t_adam7_passes *p)
{
	a7_compute_dims(p);
	a7_compute_starts(p);
}
