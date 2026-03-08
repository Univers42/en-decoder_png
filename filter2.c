/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:12:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	fs_none_sub(unsigned char *out,
		const unsigned char *scanline, size_t bw, size_t len,
		unsigned char type)
{
	size_t	i;

	i = 0;
	if (type == 0)
	{
		while (i != len)
		{
			out[i] = scanline[i];
			++i;
		}
		return ;
	}
	while (i != bw)
	{
		out[i] = scanline[i];
		++i;
	}
	while (i < len)
	{
		out[i] = scanline[i] - scanline[i - bw];
		++i;
	}
}

static void	fs_up(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, size_t len)
{
	size_t	i;

	i = 0;
	if (prevline)
	{
		while (i != len)
		{
			out[i] = scanline[i] - prevline[i];
			++i;
		}
	}
	else
	{
		while (i != len)
		{
			out[i] = scanline[i];
			++i;
		}
	}
}

static void	fs_avg(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, size_t bw, size_t len)
{
	size_t	i;

	i = 0;
	if (prevline)
	{
		while (i != bw)
		{
			out[i] = scanline[i] - (prevline[i] >> 1);
			++i;
		}
		while (i < len)
		{
			out[i] = scanline[i]
				- ((scanline[i - bw] + prevline[i]) >> 1);
			++i;
		}
	}
	else
	{
		while (i != bw)
		{
			out[i] = scanline[i];
			++i;
		}
		while (i < len)
		{
			out[i] = scanline[i] - (scanline[i - bw] >> 1);
			++i;
		}
	}
}

static void	fs_paeth(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, size_t bw, size_t len)
{
	size_t	i;

	i = 0;
	if (prevline)
	{
		while (i != bw)
		{
			out[i] = scanline[i] - prevline[i];
			++i;
		}
		while (i < len)
		{
			out[i] = scanline[i] - paethPredictor(
					scanline[i - bw], prevline[i],
					prevline[i - bw]);
			++i;
		}
	}
	else
	{
		while (i != bw)
		{
			out[i] = scanline[i];
			++i;
		}
		while (i < len)
		{
			out[i] = scanline[i] - scanline[i - bw];
			++i;
		}
	}
}

void	filterScanline(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, size_t length,
		size_t bytewidth, unsigned char filterType)
{
	if (filterType <= 1)
		fs_none_sub(out, scanline, bytewidth, length, filterType);
	else if (filterType == 2)
		fs_up(out, scanline, prevline, length);
	else if (filterType == 3)
		fs_avg(out, scanline, prevline, bytewidth, length);
	else if (filterType == 4)
		fs_paeth(out, scanline, prevline, bytewidth, length);
}
