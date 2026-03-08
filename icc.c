/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:22:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_clear_icc(t_png_info *info)
{
	string_cleanup(&info->iccp_name);
	lodepng_free(info->iccp_profile);
	info->iccp_profile = NULL;
	info->iccp_profile_size = 0;
	info->iccp_defined = 0;
}

unsigned int	lodepng_assign_icc(t_png_info *info, const char *name,
		const unsigned char *profile, unsigned int profile_size)
{
	info->iccp_name = alloc_string(name);
	info->iccp_profile = (unsigned char *)lodepng_malloc(profile_size);
	if (!info->iccp_name || !info->iccp_profile)
		return (83);
	memcpy(info->iccp_profile, profile, profile_size);
	info->iccp_profile_size = profile_size;
	return (0);
}

unsigned int	lodepng_set_icc(t_png_info *info, const char *name,
		const unsigned char *profile, unsigned int profile_size)
{
	if (info->iccp_name)
		lodepng_clear_icc(info);
	info->iccp_defined = 1;
	return (lodepng_assign_icc(info, name, profile, profile_size));
}
