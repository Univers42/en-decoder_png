/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:37:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_H
# define CHUNK_H

void				lodepng_chunk_type(char type[5],
						const unsigned char *chunk);
unsigned char		lodepng_chunk_type_equals(
						const unsigned char *chunk,
						const char *type);
unsigned char		lodepng_chunk_ancillary(
						const unsigned char *chunk);
unsigned char		lodepng_chunk_private(
						const unsigned char *chunk);
unsigned char		lodepng_chunk_safetocopy(
						const unsigned char *chunk);
unsigned char		*lodepng_chunk_data(unsigned char *chunk);
const unsigned char	*lodepng_chunk_data_const(
						const unsigned char *chunk);
unsigned int		lodepng_chunk_check_crc(
						const unsigned char *chunk);
void				lodepng_chunk_generate_crc(
						unsigned char *chunk);
unsigned char		*lodepng_chunk_next(unsigned char *chunk);
const unsigned char	*lodepng_chunk_next_const(
						const unsigned char *chunk);
unsigned char		*lodepng_chunk_find(unsigned char *chunk,
						const unsigned char *end,
						const char type[5]);
const unsigned char	*lodepng_chunk_find_const(
						const unsigned char *chunk,
						const unsigned char *end,
						const char type[5]);
unsigned int		lodepng_chunk_append(unsigned char **out,
						size_t *outlength,
						const unsigned char *chunk);
unsigned int		lodepng_chunk_create(t_ucvector *out,
						unsigned int length, const char *type,
						const unsigned char *data);
void				lodepng_unk_chunks_init(t_png_info *info);
void				lodepng_unk_chunks_cleanup(
						t_png_info *info);
unsigned int		lodepng_unk_chunks_copy(t_png_info *dest,
						const t_png_info *src);
unsigned int		add_chunk(t_ucvector *out,
						const char *chunk_name,
						const unsigned char *data,
						size_t length);
unsigned int		add_chunk_ihdr(t_ucvector *out,
						const t_png_info *info,
						unsigned int w, unsigned int h);
unsigned int		add_chunk_plte(t_ucvector *out,
						const t_png_color_mode *info);
unsigned int		add_chunk_trns(t_ucvector *out,
						const t_png_color_mode *info);
unsigned int		add_chunk_idat(t_ucvector *out,
						const unsigned char *data,
						size_t datasize,
						t_compress_settings *zlibsettings);
unsigned int		add_chunk_iend(t_ucvector *out);
void				write_signature(t_ucvector *out);
unsigned int		add_unknown_chunks(t_ucvector *out,
						unsigned char *data, size_t datasize);
unsigned int		add_chunk_text(t_ucvector *out,
						const char *keyword,
						const char *textstring);
unsigned int		add_chunk_ztxt(t_ucvector *out,
						const char *keyword,
						const char *textstring,
						t_compress_settings *zlibsettings);
unsigned int		add_chunk_itxt(t_ucvector *out,
						unsigned int compressed,
						const char *keyword,
						const char *langtag);
unsigned int		add_chunk_bkgd(t_ucvector *out,
						const t_png_info *info);
unsigned int		add_chunk_time(t_ucvector *out,
						const t_png_time *time);
unsigned int		add_chunk_phys(t_ucvector *out,
						const t_png_info *info);
unsigned int		add_chunk_gama(t_ucvector *out,
						const t_png_info *info);
unsigned int		add_chunk_chrm(t_ucvector *out,
						const t_png_info *info);
unsigned int		add_chunk_srgb(t_ucvector *out,
						const t_png_info *info);
unsigned int		add_chunk_iccp(t_ucvector *out,
						const t_png_info *info,
						t_compress_settings *zlibsettings);
#endif