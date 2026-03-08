/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:12:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	uf_none_sub(unsigned char *recon,
		const unsigned char *scanline, size_t bw, size_t len,
		unsigned char type)
{
	size_t	i;

	i = 0;
	if (type == 0)
	{
		while (i != len)
		{
			recon[i] = scanline[i];
			++i;
		}
		return ;
	}
	while (i != bw)
	{
		recon[i] = scanline[i];
		++i;
	}
	while (i < len)
	{
		recon[i] = scanline[i] + recon[i - bw];
		++i;
	}
}

static void	uf_up(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, size_t len)
{
	size_t	i;

	i = 0;
	if (precon)
	{
		while (i != len)
		{
			recon[i] = scanline[i] + precon[i];
			++i;
		}
	}
	else
	{
		while (i != len)
		{
			recon[i] = scanline[i];
			++i;
		}
	}
}

static void	uf_avg(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, size_t bw, size_t len)
{
	size_t	i;

	i = 0;
	if (precon)
	{
		while (i != bw)
		{
			recon[i] = scanline[i] + (precon[i] >> 1);
			++i;
		}
		while (i < len)
		{
			recon[i] = scanline[i]
				+ ((recon[i - bw] + precon[i]) >> 1);
			++i;
		}
	}
	else
	{
		while (i != bw)
		{
			recon[i] = scanline[i];
			++i;
		}
		while (i < len)
		{
			recon[i] = scanline[i] + (recon[i - bw] >> 1);
			++i;
		}
	}
}

static void	uf_paeth(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, size_t bw, size_t len)
{
	size_t	i;

	i = 0;
	if (precon)
	{
		while (i != bw)
		{
			recon[i] = scanline[i] + precon[i];
			++i;
		}
		while (i < len)
		{
			recon[i] = scanline[i] + paethPredictor(
					recon[i - bw], precon[i], precon[i - bw]);
			++i;
		}
	}
	else
	{
		while (i != bw)
		{
			recon[i] = scanline[i];
			++i;
		}
		while (i < len)
		{
			recon[i] = scanline[i] + recon[i - bw];
			++i;
		}
	}
}

unsigned	unfilterScanline(unsigned char *recon,
		const unsigned char *scanline,
		const unsigned char *precon, size_t bytewidth,
		unsigned char filterType, size_t length)
{
	if (filterType <= 1)
		uf_none_sub(recon, scanline, bytewidth, length, filterType);
	else if (filterType == 2)
		uf_up(recon, scanline, precon, length);
	else if (filterType == 3)
		uf_avg(recon, scanline, precon, bytewidth, length);
	else if (filterType == 4)
		uf_paeth(recon, scanline, precon, bytewidth, length);
	else
		return (36);
	return (0);
}
