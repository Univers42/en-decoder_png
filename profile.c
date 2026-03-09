/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:05:22 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 22:23:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"

unsigned int	is_gray_icc_profile(const unsigned char *profile,
					unsigned int size)
{
	if (size < 20)
		return (0);
	return (profile[16] == 'G' && profile[17] == 'R'
		&& profile[18] == 'A' && profile[19] == 'Y');
}

unsigned int	is_rgb_icc_profile(const unsigned char *profile,
					unsigned int size)
{
	if (size < 20)
		return (0);
	return (profile[16] == 'R' && profile[17] == 'G'
		&& profile[18] == 'B' && profile[19] == ' ');
}
