/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:37:14 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:12:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXT_H
# define TEXT_H

void			lodepng_text_init(t_png_info *info);
void			lodepng_text_cleanup(t_png_info *info);
unsigned int	lodepng_text_copy(t_png_info *dest,
					const t_png_info	*source);
void			lodepng_clear_text(t_png_info *info);
unsigned int	lodepng_add_text(t_png_info *info, const char *key,
					const char	*str);
void			lodepng_itext_init(t_png_info *info);
void			lodepng_itext_cleanup(t_png_info *info);
unsigned int	lodepng_itext_copy(t_png_info *dest,
					const t_png_info *source);

#endif
