/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:12:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	fs_sub(unsigned char *out,
		const unsigned char *scanline, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	while (i != d->bw)
	{
		out[i] = scanline[i];
		++i;
	}
	while (i < d->len)
	{
		out[i] = scanline[i] - scanline[i - d->bw];
		++i;
	}
}

static void	fs_up(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (prevline)
	{
		while (i != d->len)
		{
			out[i] = scanline[i] - prevline[i];
			++i;
		}
	}
	else
		memcpy(out, scanline, d->len);
}

static void	fs_avg(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (!prevline)
	{
		memcpy(out, scanline, d->bw);
		i = d->bw;
		while (i < d->len)
		{
			out[i] = scanline[i] - (scanline[i - d->bw] >> 1);
			++i;
		}
		return ;
	}
	while (i != d->bw)
	{
		out[i] = scanline[i] - (prevline[i] >> 1);
		++i;
	}
	while (i < d->len)
	{
		out[i] = scanline[i]
			- ((scanline[i - d->bw] + prevline[i]) >> 1);
		++i;
	}
}

static void	fs_paeth(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (!prevline)
	{
		memcpy(out, scanline, d->bw);
		i = d->bw;
		while (i < d->len)
		{
			out[i] = scanline[i] - scanline[i - d->bw];
			++i;
		}
		return ;
	}
	while (i != d->bw)
	{
		out[i] = scanline[i] - prevline[i];
		++i;
	}
	while (i < d->len)
	{
		out[i] = scanline[i] - paeth_predictor(scanline[i - d->bw],
				prevline[i], prevline[i - d->bw]);
		++i;
	}
}

void	filter_scanline(unsigned char *out, const unsigned char *scanline,
		const unsigned char *prevline, const t_filter_dim *d)
{
	if (d->type == 0)
		memcpy(out, scanline, d->len);
	else if (d->type == 1)
		fs_sub(out, scanline, d);
	else if (d->type == 2)
		fs_up(out, scanline, prevline, d);
	else if (d->type == 3)
		fs_avg(out, scanline, prevline, d);
	else if (d->type == 4)
		fs_paeth(out, scanline, prevline, d);
}
