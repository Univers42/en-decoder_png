/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_profile.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:44:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 03:33:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_PROFILE_H
# define COLOR_PROFILE_H

# include "types.h"

void			cp_set_alpha(t_cp_ctx *ctx);
void			cp16_check_precision(t_cp_ctx *ctx);
void			cp16_scan(t_cp_ctx *ctx);
void			cp8_scan(t_cp_ctx *ctx,
					t_color_tree *tree);

#endif
