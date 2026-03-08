/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:15:30 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 00:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <string.h>

/* Memory management prototypes */
void	*lodepng_malloc(size_t size);
void	lodepng_free(void *ptr);
void	*lodepng_realloc(void *ptr, size_t size);

/* Utility macros */
# define LODEPNG_MAX(a, b) ((a) > (b) ? (a) : (b))
# define LODEPNG_MIN(a, b) ((a) < (b) ? (a) : (b))

# define CERROR_TRY_RETURN(ec) \
	do { unsigned int _e = ec; if (_e) return (_e); } while (0)

# define CERROR_RETURN(v, ec) \
	do { v = ec; return ; } while (0)

# define CERROR_RETURN_ERROR(v, ec) \
	do { v = ec; return (v); } while (0)

# define CERROR_BREAK(v, ec) \
	do { v = ec; break ; } while (0)

# define ERROR_BREAK(ec) \
	do { error = ec; break ; } while (0)

# define READBIT(bp, bs) \
	(((bs)[((bp) >> 3)] >> (((bp) & 0x7))) & 1)

#endif
