/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:38:30 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:38:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef INFO_H
#  define INFO_H

void			lodepng_info_init(LodePNGInfo *info);
void			lodepng_info_cleanup(LodePNGInfo *info);
unsigned int	lodepng_info_copy(LodePNGInfo *dest,
					const LodePNGInfo *source);
unsigned int	lodepng_inspect(unsigned int *w, unsigned int *h,
					LodePNGState *state, const unsigned char *in,
					size_t insize);

# endif