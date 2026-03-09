/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:44:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	info_init_ancillary(t_png_info *info)
{
	info->background_defined = 0;
	info->background_r = 0;
	info->background_g = 0;
	info->background_b = 0;
	lodepng_text_init(info);
	lodepng_itext_init(info);
	info->time_defined = 0;
	info->phys_defined = 0;
	info->gama_defined = 0;
	info->chrm_defined = 0;
	info->srgb_defined = 0;
	info->iccp_defined = 0;
	info->iccp_name = NULL;
	info->iccp_profile = NULL;
	lodepng_unk_chunks_init(info);
}

void	lodepng_info_init(t_png_info *info)
{
	lodepng_color_mode_init(&info->color);
	info->interlace_method = 0;
	info->compression_method = 0;
	info->filter_method = 0;
	info_init_ancillary(info);
}

void	lodepng_info_cleanup(t_png_info *info)
{
	lodepng_color_mode_cleanup(&info->color);
	lodepng_text_cleanup(info);
	lodepng_itext_cleanup(info);
	lodepng_clear_icc(info);
	lodepng_unk_chunks_cleanup(info);
}

static unsigned int	info_copy_anc(t_png_info *dest,
		const t_png_info *source)
{
	unsigned int	error;

	error = lodepng_text_copy(dest, source);
	if (error)
		return (error);
	error = lodepng_itext_copy(dest, source);
	if (error)
		return (error);
	if (source->iccp_defined)
	{
		error = lodepng_assign_icc(dest, source->iccp_name,
				source->iccp_profile,
				source->iccp_profile_size);
		if (error)
			return (error);
	}
	lodepng_unk_chunks_init(dest);
	error = lodepng_unk_chunks_copy(dest, source);
	return (error);
}

unsigned int	lodepng_info_copy(t_png_info *dest,
		const t_png_info *source)
{
	unsigned int	error;

	lodepng_info_cleanup(dest);
	*dest = *source;
	lodepng_color_mode_init(&dest->color);
	error = lodepng_color_mode_copy(&dest->color, &source->color);
	if (error)
		return (error);
	return (info_copy_anc(dest, source));
}
