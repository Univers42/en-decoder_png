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
					const unsigned char *chunk);
unsigned char	lodepng_chunk_type_equals(const unsigned char *chunk,
					const char *type);
unsigned char	lodepng_chunk_ancillary(const unsigned char *chunk);
unsigned char	lodepng_chunk_private(const unsigned char *chunk);
unsigned char	lodepng_chunk_safetocopy(const unsigned char *chunk);
unsigned char	*lodepng_chunk_data(unsigned char *chunk);
const unsigned char	*lodepng_chunk_data_const(const unsigned char *chunk);
unsigned		lodepng_chunk_check_crc(const unsigned char *chunk);
void			lodepng_chunk_generate_crc(unsigned char *chunk);
unsigned char	*lodepng_chunk_next(unsigned char *chunk);
const unsigned char	*lodepng_chunk_next_const(const unsigned char *chunk);
unsigned char	*lodepng_chunk_find(unsigned char *chunk,
					const unsigned char *end, const char type[5]);
const unsigned char	*lodepng_chunk_find_const(const unsigned char *chunk,
					const unsigned char *end, const char type[5]);
unsigned		lodepng_chunk_append(unsigned char **out,
					size_t *outlength, const unsigned char *chunk);
unsigned		lodepng_chunk_create(unsigned char **out,
					size_t *outlength, unsigned length,
					const char *type, const unsigned char *data);
void			LodePNGUnknownChunks_init(LodePNGInfo *info);
void			LodePNGUnknownChunks_cleanup(LodePNGInfo *info);
unsigned		LodePNGUnknownChunks_copy(LodePNGInfo *dest,
					const LodePNGInfo *src);
unsigned		addChunk(ucvector *out, const char *chunkName,
					const unsigned char *data, size_t length);
unsigned		addChunk_IHDR(ucvector *out, unsigned w, unsigned h,
					LodePNGColorType colortype, unsigned bitdepth,
					unsigned interlace_method);
unsigned		addChunk_PLTE(ucvector *out,
					const LodePNGColorMode *info);
unsigned		addChunk_tRNS(ucvector *out,
					const LodePNGColorMode *info);
unsigned		addChunk_IDAT(ucvector *out, const unsigned char *data,
					size_t datasize,
					LodePNGCompressSettings *zlibsettings);
unsigned		addChunk_IEND(ucvector *out);
void			writeSignature(ucvector *out);
unsigned		addUnknownChunks(ucvector *out, unsigned char *data,
					size_t datasize);

# ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

unsigned		addChunk_tEXt(ucvector *out, const char *keyword,
					const char *textstring);
unsigned		addChunk_zTXt(ucvector *out, const char *keyword,
					const char *textstring,
					LodePNGCompressSettings *zlibsettings);
unsigned		addChunk_iTXt(ucvector *out, unsigned compressed,
					const char *keyword, const char *langtag,
					const char *transkey, const char *textstring,
					LodePNGCompressSettings *zlibsettings);
unsigned		addChunk_bKGD(ucvector *out, const LodePNGInfo *info);
unsigned		addChunk_tIME(ucvector *out, const LodePNGTime *time);
unsigned		addChunk_pHYs(ucvector *out, const LodePNGInfo *info);
unsigned		addChunk_gAMA(ucvector *out, const LodePNGInfo *info);
unsigned		addChunk_cHRM(ucvector *out, const LodePNGInfo *info);
unsigned		addChunk_sRGB(ucvector *out, const LodePNGInfo *info);
unsigned		addChunk_iCCP(ucvector *out, const LodePNGInfo *info,
					LodePNGCompressSettings *zlibsettings);

# endif
#endif