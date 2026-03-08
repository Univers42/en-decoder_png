/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:21:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

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

#endif

void	lodepng_info_init(t_png_info *info)
{
	lodepng_color_mode_init(&info->color);
	info->interlace_method = 0;
	info->compression_method = 0;
	info->filter_method = 0;
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	info_init_ancillary(info);
#endif
}

void	lodepng_info_cleanup(t_png_info *info)
{
	lodepng_color_mode_cleanup(&info->color);
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	lodepng_text_cleanup(info);
	lodepng_itext_cleanup(info);
	lodepng_clear_icc(info);
	lodepng_unk_chunks_cleanup(info);
#endif
}

unsigned int	lodepng_info_copy(t_png_info *dest,
		const t_png_info *source)
{
	lodepng_info_cleanup(dest);
	*dest = *source;
	lodepng_color_mode_init(&dest->color);
	CERROR_TRY_RETURN(lodepng_color_mode_copy(&dest->color,
			&source->color));
#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
	CERROR_TRY_RETURN(lodepng_text_copy(dest, source));
	CERROR_TRY_RETURN(lodepng_itext_copy(dest, source));
	if (source->iccp_defined)
		CERROR_TRY_RETURN(lodepng_assign_icc(dest, source->iccp_name,
				source->iccp_profile, source->iccp_profile_size));
	lodepng_unk_chunks_init(dest);
	CERROR_TRY_RETURN(lodepng_unk_chunks_copy(dest, source));
#endif
	return (0);
}
