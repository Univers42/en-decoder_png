/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adler.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:15:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	adler_get_amount(unsigned int len)
{
	if (len > 5552)
		return (5552);
	return (len);
}

unsigned int	update_adler32(unsigned int adler,
		const unsigned char *data, unsigned int len)
{
	unsigned int	s1;
	unsigned int	s2;
	unsigned int	amount;

	s1 = adler & 0xffff;
	s2 = (adler >> 16) & 0xffff;
	while (len > 0)
	{
		amount = adler_get_amount(len);
		len -= amount;
		while (amount > 0)
		{
			s1 += (*data++);
			s2 += s1;
			--amount;
		}
		s1 %= 65521;
		s2 %= 65521;
	}
	return ((s2 << 16) | s1);
}

unsigned int	adler32(const unsigned char *data, unsigned int len)
{
	return (update_adler32(1L, data, len));
}
