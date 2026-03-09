/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADAM7_H
# define ADAM7_H

# include <stddef.h>

const t_adam7_tbl	*a7t(void);
void				adam7_getpassvalues(t_adam7_passes *p);
void				adam7_deinterlace(unsigned char *out,
						const unsigned char *in, t_adam7_passes *p);
void				adam7_interlace(unsigned char *out,
						const unsigned char *in, t_adam7_passes *p);

#endif
