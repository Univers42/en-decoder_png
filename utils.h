/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:15:30 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:03:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <string.h>

/* Memory management prototypes */
void			*lodepng_malloc(size_t size);
void			lodepng_free(void *ptr);
void			*lodepng_realloc(void *ptr, size_t size);

/* Utility functions */
unsigned int	lodepng_max_uint(unsigned int a, unsigned int b);
unsigned int	lodepng_min_uint(unsigned int a, unsigned int b);

#endif
