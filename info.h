/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:38:30 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 22:37:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFO_H
# define INFO_H

void			lodepng_info_init(t_png_info *info);
void			lodepng_info_cleanup(t_png_info *info);
unsigned int	lodepng_info_copy(t_png_info *dest,
					const t_png_info	*source);
unsigned int	lodepng_inspect(t_png_state *state,
					const unsigned char *in, size_t insize);

#endif
