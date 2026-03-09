/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	a7_deint_bits(unsigned char *out, const unsigned char *in,
		size_t *obp, size_t *ibp)
{
	set_bit_of_rev_stream0(obp, out,
		read_bit_from_rev_stream(ibp, in));
}

static void	a7_deint_byte_pass(unsigned char *out,
		const unsigned char *in, const t_adam7_passes *p,
		unsigned int pass)
{
	unsigned int	x;
	unsigned int	y;
	size_t			pis;
	size_t			pos;
	size_t			bw;

	bw = p->bpp / 8;
	y = 0;
	while (y < p->passh[pass])
	{
		x = 0;
		while (x < p->passw[pass])
		{
			pis = p->passstart[pass] + (y * p->passw[pass] + x) * bw;
			pos = a7t()->iy[pass] + (size_t)(y * a7t()->dy[pass]);
			pos = (pos * p->w + a7t()->ix[pass]
					+ x * a7t()->dx[pass]) * bw;
			memcpy(&out[pos], &in[pis], bw);
			++x;
		}
		++y;
	}
}

static void	a7_deint_bit_pass(unsigned char *out,
		const unsigned char *in, const t_adam7_passes *p,
		unsigned int pass)
{
	unsigned int	x;
	unsigned int	y;
	size_t			ibp;
	size_t			obp;

	y = 0;
	while (y < p->passh[pass])
	{
		x = 0;
		while (x < p->passw[pass])
		{
			ibp = 8 * p->passstart[pass];
			ibp += y * (size_t)(p->bpp * p->passw[pass])
				+ x * p->bpp;
			obp = a7t()->iy[pass]
				+ (size_t)(y * a7t()->dy[pass]);
			obp = obp * (p->bpp * p->w);
			obp += (a7t()->ix[pass]
					+ x * a7t()->dx[pass]) * p->bpp;
			a7_deint_bits(out, in, &obp, &ibp);
			++x;
		}
		++y;
	}
}

void	adam7_deinterlace(unsigned char *out, const unsigned char *in,
		t_adam7_passes *p)
{
	unsigned int	i;

	adam7_getpassvalues(p);
	i = 0;
	while (i != 7)
	{
		if (p->bpp >= 8)
			a7_deint_byte_pass(out, in, p, i);
		else
			a7_deint_bit_pass(out, in, p, i);
		++i;
	}
}
