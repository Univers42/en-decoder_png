/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_state_utils.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:50 by marvin            #+#    #+#             */
/*   Updated: 2025/12/30 00:00:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef API_STATE_UTILS_H
#  define API_STATE_UTILS_H

#  include "settings.h"

void	lodepng_state_init(LodePNGState *state);
void	lodepng_state_cleanup(LodePNGState *state);
void	lodepng_state_copy(LodePNGState *dest, const LodePNGState *source);

# endif