/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:56:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:12:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	uf_sub(unsigned char *recon,
		const unsigned char *scanline, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	while (i != d->bw)
	{
		recon[i] = scanline[i];
		++i;
	}
	while (i < d->len)
	{
		recon[i] = scanline[i] + recon[i - d->bw];
		++i;
	}
}

static void	uf_up(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (precon)
	{
		while (i != d->len)
		{
			recon[i] = scanline[i] + precon[i];
			++i;
		}
	}
	else
		memcpy(recon, scanline, d->len);
}

static void	uf_avg(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (!precon)
	{
		memcpy(recon, scanline, d->bw);
		i = d->bw;
		while (i < d->len)
		{
			recon[i] = scanline[i] + (recon[i - d->bw] >> 1);
			++i;
		}
		return ;
	}
	while (i != d->bw)
	{
		recon[i] = scanline[i] + (precon[i] >> 1);
		++i;
	}
	while (i < d->len)
	{
		recon[i] = scanline[i]
			+ ((recon[i - d->bw] + precon[i]) >> 1);
		++i;
	}
}

static void	uf_paeth(unsigned char *recon, const unsigned char *scanline,
		const unsigned char *precon, const t_filter_dim *d)
{
	size_t	i;

	i = 0;
	if (!precon)
	{
		memcpy(recon, scanline, d->bw);
		i = d->bw;
		while (i < d->len)
		{
			recon[i] = scanline[i] + recon[i - d->bw];
			++i;
		}
		return ;
	}
	while (i != d->bw)
	{
		recon[i] = scanline[i] + precon[i];
		++i;
	}
	while (i < d->len)
	{
		recon[i] = scanline[i] + paeth_predictor(recon[i - d->bw],
				precon[i], precon[i - d->bw]);
		++i;
	}
}

unsigned int	unfilter_scanline(unsigned char *recon,
		const unsigned char *scanline,
		const unsigned char *precon, const t_filter_dim *d)
{
	if (d->type == 0)
		memcpy(recon, scanline, d->len);
	else if (d->type == 1)
		uf_sub(recon, scanline, d);
	else if (d->type == 2)
		uf_up(recon, scanline, precon, d);
	else if (d->type == 3)
		uf_avg(recon, scanline, precon, d);
	else if (d->type == 4)
		uf_paeth(recon, scanline, precon, d);
	else
		return (36);
	return (0);
}
