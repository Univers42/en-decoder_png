/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:17 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:28:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
# define HASH_H

# include "types.h"

# ifndef HASH_STRUCT_DEFINED

typedef struct s_hash
{
	int				*head;
	unsigned short	*chain;
	int				*val;
	int				*headz;
	unsigned short	*chainz;
	unsigned short	*zeros;
}	t_hash;

# endif

unsigned int	hash_init(t_hash *hash, unsigned int windowsize);
void			hash_cleanup(t_hash *hash);
unsigned int	get_hash(const unsigned char *data, size_t size, size_t pos);
unsigned int	count_zeros(const unsigned char *data, size_t size,
					size_t pos);
void			update_hash_chain(t_hash *hash, size_t wpos,
					unsigned int hashval, unsigned short numzeros);

#endif
