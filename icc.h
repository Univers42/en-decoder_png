/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:37:53 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:37:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ICC_H
#  define ICC_H

#  include "str.h"

void			lodepng_clear_icc(LodePNGInfo *info);
unsigned int	lodepng_assign_icc(LodePNGInfo *info, const char *name,
					const unsigned char *profile,
					unsigned int profile_size);
unsigned int	lodepng_set_icc(LodePNGInfo *info, const char *name,
					const unsigned char *profile,
					unsigned int profile_size);
unsigned		isGrayICCProfile(const unsigned char *profile,
					unsigned size);
unsigned		isRGBICCProfile(const unsigned char *profile,
					unsigned size);

# endif