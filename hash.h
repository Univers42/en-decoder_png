/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:17 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:20:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HASH_H
#  define HASH_H

#  include "types.h"

#  ifndef HASH_STRUCT_DEFINED

typedef struct s_hash
{
	int				*head;
	unsigned short	*chain;
	int				*val;
	int				*headz;
	unsigned short	*chainz;
	unsigned short	*zeros;
}	Hash;

#  endif

unsigned int	hash_init(Hash *hash, unsigned int windowsize);
void			hash_cleanup(Hash *hash);
unsigned int	getHash(const unsigned char *data, size_t size, size_t pos);
unsigned int	countZeros(const unsigned char *data, size_t size,
					size_t pos);
void			updateHashChain(Hash *hash, size_t wpos,
					unsigned int hashval, unsigned short numzeros);

# endif