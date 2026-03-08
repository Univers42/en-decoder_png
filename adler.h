/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adler.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:23:35 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:23:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADLER_H
# define ADLER_H

unsigned int	update_adler32(unsigned int adler,
					const unsigned char	*data, unsigned int len);
unsigned int	adler32(const unsigned char *data, unsigned int len);

#endif
