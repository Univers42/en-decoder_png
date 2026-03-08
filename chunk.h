/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:33:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_H
# define CHUNK_H

void			lodepng_chunk_type(char type[5],
					const unsigned char	*chunk);
unsigned char	lodepng_chunk_type_equals(const unsigned char *chunk,
					const char	*type);
unsigned char	lodepng_chunk_ancillary(const unsigned char *chunk);
unsigned char	lodepng_chunk_private(const unsigned char *chunk);
unsigned char	lodepng_chunk_safetocopy(const unsigned char *chunk);
unsigned char	*lodepng_chunk_data(unsigned char *chunk);
const unsigned char	*lodepng_chunk_data_const(const unsigned char *chunk);
unsigned int		lodepng_chunk_check_crc(const unsigned char *chunk);
void			lodepng_chunk_generate_crc(unsigned char *chunk);
unsigned char	*lodepng_chunk_next(unsigned char *chunk);
const unsigned char	*lodepng_chunk_next_const(const unsigned char *chunk);
unsigned char	*lodepng_chunk_find(unsigned char *chunk,
					const unsigned char	*end, const char type[5]);
const unsigned char	*lodepng_chunk_find_const(const unsigned char *chunk,
					const unsigned char	*end, const char type[5]);
unsigned int		lodepng_chunk_append(unsigned char **out,
					size_t	*outlength, const unsigned char *chunk);
unsigned int		lodepng_chunk_create(unsigned char **out,
					size_t *outlength, unsigned int length,
					const char	*type, const unsigned char *data);
void			lodepng_unk_chunks_init(t_png_info *info);
void			lodepng_unk_chunks_cleanup(t_png_info *info);
unsigned int		lodepng_unk_chunks_copy(t_png_info *dest,
					const t_png_info	*src);
unsigned int		add_chunk(ucvector *out, const char *chunk_name,
					const unsigned char	*data, size_t length);
unsigned int		add_chunk_ihdr(ucvector *out, unsigned int w, unsigned int h,
					t_png_color_type colortype, unsigned int bitdepth,
					unsigned	interlace_method);
unsigned int		add_chunk_plte(ucvector *out,
					const t_png_color_mode	*info);
unsigned int		add_chunk_trns(ucvector *out,
					const t_png_color_mode	*info);
unsigned int		add_chunk_idat(ucvector *out, const unsigned char *data,
					size_t datasize,
					t_compress_settings	*zlibsettings);
unsigned int		add_chunk_iend(ucvector *out);
void			write_signature(ucvector *out);
unsigned int		add_unknown_chunks(ucvector *out, unsigned char *data,
					size_t	datasize);

# ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

unsigned int		add_chunk_text(ucvector *out, const char *keyword,
					const char	*textstring);
unsigned int		add_chunk_ztxt(ucvector *out, const char *keyword,
					const char *textstring,
					t_compress_settings	*zlibsettings);
unsigned int		add_chunk_itxt(ucvector *out, unsigned int compressed,
					const char *keyword, const char *langtag,
					const char *transkey, const char *textstring,
					t_compress_settings	*zlibsettings);
unsigned int		add_chunk_bkgd(ucvector *out, const t_png_info *info);
unsigned int		add_chunk_time(ucvector *out, const t_png_time *time);
unsigned int		add_chunk_phys(ucvector *out, const t_png_info *info);
unsigned int		add_chunk_gama(ucvector *out, const t_png_info *info);
unsigned int		add_chunk_chrm(ucvector *out, const t_png_info *info);
unsigned int		add_chunk_srgb(ucvector *out, const t_png_info *info);
unsigned int		add_chunk_iccp(ucvector *out, const t_png_info *info,
					t_compress_settings	*zlibsettings);

# endif
#endif