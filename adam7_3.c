/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	a7_int_bits(unsigned char *out, const unsigned char *in,
		size_t *obp, size_t *ibp)
{
	set_bit_of_rev_stream(obp, out,
		read_bit_from_rev_stream(ibp, in));
}

static void	a7_int_byte_pass(unsigned char *out,
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
			pis = a7t()->iy[pass]
				+ (size_t)(y * a7t()->dy[pass]);
			pis = (pis * p->w + a7t()->ix[pass]
					+ x * a7t()->dx[pass]) * bw;
			pos = p->passstart[pass]
				+ (y * p->passw[pass] + x) * bw;
			memcpy(&out[pos], &in[pis], bw);
			++x;
		}
		++y;
	}
}

static void	a7_int_bit_pass(unsigned char *out,
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
			ibp = a7t()->iy[pass]
				+ (size_t)(y * a7t()->dy[pass]);
			ibp = ibp * (p->bpp * p->w);
			ibp += (a7t()->ix[pass]
					+ x * a7t()->dx[pass]) * p->bpp;
			obp = 8 * p->passstart[pass];
			obp += y * (size_t)(p->bpp * p->passw[pass])
				+ x * p->bpp;
			a7_int_bits(out, in, &obp, &ibp);
			++x;
		}
		++y;
	}
}

void	adam7_interlace(unsigned char *out, const unsigned char *in,
		t_adam7_passes *p)
{
	unsigned int	i;

	adam7_getpassvalues(p);
	i = 0;
	while (i != 7)
	{
		if (p->bpp >= 8)
			a7_int_byte_pass(out, in, p, i);
		else
			a7_int_bit_pass(out, in, p, i);
		++i;
	}
}
