/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:37:14 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:37:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef TEXT_H
#  define TEXT_H

void			LodePNGText_init(LodePNGInfo *info);
void			LodePNGText_cleanup(LodePNGInfo *info);
unsigned int	LodePNGText_copy(LodePNGInfo *dest,
					const LodePNGInfo *source);
void			lodepng_clear_text(LodePNGInfo *info);
unsigned int	lodepng_add_text(LodePNGInfo *info, const char *key,
					const char *str);
void			LodePNGIText_init(LodePNGInfo *info);
void			LodePNGIText_cleanup(LodePNGInfo *info);
unsigned int	LodePNGIText_copy(LodePNGInfo *dest,
					const LodePNGInfo *source);
void			lodepng_clear_itext(LodePNGInfo *info);
unsigned int	lodepng_add_itext(LodePNGInfo *info, const char *key,
					const char *langtag, const char *transkey,
					const char *str);

# endif