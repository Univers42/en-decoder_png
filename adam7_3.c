/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:08:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	a7_int_bits(unsigned char *out, const unsigned char *in,
		size_t *obp, size_t *ibp, unsigned int bpp)
{
	unsigned int	b;

	b = 0;
	while (b < bpp)
	{
		set_bit_of_rev_stream(obp, out,
			read_bit_from_rev_stream(ibp, in));
		++b;
	}
}

static void	a7_int_byte_pass(unsigned char *out,
		const unsigned char *in, unsigned int w, size_t bw,
		unsigned int pass, unsigned int pw, unsigned int ph, size_t ps)
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	b;
	size_t		pis;
	size_t		pos;

	y = 0;
	while (y < ph)
	{
		x = 0;
		while (x < pw)
		{
			pis = ((g_adam7_iy[pass] + (size_t)(y * g_adam7_dy[pass]))
				* w + g_adam7_ix[pass] + x * g_adam7_dx[pass]) * bw;
			pos = ps + (y * pw + x) * bw;
			b = 0;
			while (b < bw)
			{
				out[pos + b] = in[pis + b];
				++b;
			}
			++x;
		}
		++y;
	}
}

static void	a7_int_bit_pass(unsigned char *out,
		const unsigned char *in, unsigned int w, unsigned int bpp,
		unsigned int pass, unsigned int pw, unsigned int ph, size_t ps)
{
	unsigned int	x;
	unsigned int	y;
	size_t		ibp;
	size_t		obp;

	y = 0;
	while (y < ph)
	{
		x = 0;
		while (x < pw)
		{
			ibp = (g_adam7_iy[pass] + (size_t)(y * g_adam7_dy[pass]))
				* (bpp * w) + (g_adam7_ix[pass]
				+ x * g_adam7_dx[pass]) * bpp;
			obp = (8 * ps) + (y * (size_t)(bpp * pw) + x * bpp);
			a7_int_bits(out, in, &obp, &ibp, bpp);
			++x;
		}
		++y;
	}
}

void	adam7_interlace(unsigned char *out, const unsigned char *in,
		unsigned int w, unsigned int h, unsigned int bpp)
{
	unsigned int	passw[7];
	unsigned int	passh[7];
	size_t		filter_passstart[8];
	size_t		padded_passstart[8];
	size_t		passstart[8];
	unsigned int	i;

	adam7_getpassvalues(passw, passh, filter_passstart,
		padded_passstart, passstart, w, h, bpp);
	i = 0;
	while (i != 7)
	{
		if (bpp >= 8)
			a7_int_byte_pass(out, in, w, bpp / 8, i,
				passw[i], passh[i], passstart[i]);
		else
			a7_int_bit_pass(out, in, w, bpp, i,
				passw[i], passh[i], passstart[i]);
		++i;
	}
}
