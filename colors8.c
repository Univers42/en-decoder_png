/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	addcolorbits_mask(unsigned int bits)
{
	if (bits == 1)
		return (7);
	if (bits == 2)
		return (3);
	return (1);
}

void	add_color_bits(unsigned char *out, size_t index,
		unsigned int bits, unsigned int in)
{
	unsigned int	m;
	unsigned int	p;

	m = addcolorbits_mask(bits);
	p = index & m;
	in &= (1u << bits) - 1u;
	in = in << (bits * (m - p));
	if (p == 0)
		out[index * bits / 8] = in;
	else
		out[index * bits / 8] |= in;
}
