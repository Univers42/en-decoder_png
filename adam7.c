/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:08:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	a7_compute_dims(unsigned int passw[7], unsigned int passh[7],
		unsigned int w, unsigned int h)
{
	unsigned int	i;

	i = 0;
	while (i != 7)
	{
		passw[i] = (w + g_adam7_dx[i] - g_adam7_ix[i] - 1)
			/ g_adam7_dx[i];
		passh[i] = (h + g_adam7_dy[i] - g_adam7_iy[i] - 1)
			/ g_adam7_dy[i];
		if (passw[i] == 0)
			passh[i] = 0;
		if (passh[i] == 0)
			passw[i] = 0;
		++i;
	}
}

static void	a7_compute_starts(unsigned int passw[7], unsigned int passh[7],
		size_t fp[8], size_t pp[8], size_t ps[8], unsigned int bpp)
{
	unsigned int	i;
	size_t			fval;

	fp[0] = 0;
	pp[0] = 0;
	ps[0] = 0;
	i = 0;
	while (i != 7)
	{
		if (passw[i] && passh[i])
			fval = passh[i] * (1 + (passw[i] * bpp + 7) / 8);
		else
			fval = 0;
		fp[i + 1] = fp[i] + fval;
		pp[i + 1] = pp[i] + passh[i] * ((passw[i] * bpp + 7) / 8);
		ps[i + 1] = ps[i] + (passh[i] * passw[i] * bpp + 7) / 8;
		++i;
	}
}

void	adam7_getpassvalues(unsigned int passw[7], unsigned int passh[7],
		size_t fp[8], size_t pp[8], size_t ps[8],
		unsigned int w, unsigned int h, unsigned int bpp)
{
	a7_compute_dims(passw, passh, w, h);
	a7_compute_starts(passw, passh, fp, pp, ps, bpp);
}
