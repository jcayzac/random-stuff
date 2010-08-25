/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2002    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************/

#ifndef _OV_FILE_H_
#define _OV_FILE_H_
#include <stdio.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	
#define X(n) (n)
#define LOOKUP_T const ogg_int32_t
	
#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free
	
	typedef long long ogg_int64_t;
	typedef int ogg_int32_t;
	typedef unsigned int ogg_uint32_t;
	typedef short ogg_int16_t;
	
	typedef struct ogg_buffer_state{
		struct ogg_buffer    *unused_buffers;
		struct ogg_reference *unused_references;
		int                   outstanding;
		int                   shutdown;
	} ogg_buffer_state;
	
	typedef struct ogg_buffer {
		unsigned char      *data;
		long                size;
		int                 refcount;
		
		union {
			ogg_buffer_state  *owner;
			struct ogg_buffer *next;
		} ptr;
	} ogg_buffer;
	
	typedef struct ogg_reference {
		ogg_buffer    *buffer;
		long           begin;
		long           length;
		
		struct ogg_reference *next;
	} ogg_reference;
	
	typedef struct oggpack_buffer {
		int            headbit;
		unsigned char *headptr;
		long           headend;
		
		/* memory management */
		ogg_reference *head;
		ogg_reference *tail;
		
		/* render the byte/bit counter API constant time */
		long              count; /* doesn't count the tail */
	} oggpack_buffer;
	
	typedef struct oggbyte_buffer {
		ogg_reference *baseref;
		
		ogg_reference *ref;
		unsigned char *ptr;
		long           pos;
		long           end;
	} oggbyte_buffer;
	
	typedef struct ogg_sync_state {
		/* decode memory management pool */
		ogg_buffer_state *bufferpool;
		
		/* stream buffers */
		ogg_reference    *fifo_head;
		ogg_reference    *fifo_tail;
		long              fifo_fill;
		
		/* stream sync management */
		int               unsynced;
		int               headerbytes;
		int               bodybytes;
		
	} ogg_sync_state;
	
	typedef struct ogg_stream_state {
		ogg_reference *header_head;
		ogg_reference *header_tail;
		ogg_reference *body_head;
		ogg_reference *body_tail;
		
		int            e_o_s;    /* set when we have buffered the last
		 packet in the logical bitstream */
		int            b_o_s;    /* set after we've written the initial page
		 of a logical bitstream */
		long           serialno;
		long           pageno;
		ogg_int64_t    packetno; /* sequence number for decode; the framing
		 knows where there's a hole in the data,
		 but we need coupling so that the codec
		 (which is in a seperate abstraction
		 layer) also knows about the gap */
		ogg_int64_t    granulepos;
		
		int            lacing_fill;
		ogg_uint32_t   body_fill;
		
		/* decode-side state data */
		int            holeflag;
		int            spanflag;
		int            clearflag;
		int            laceptr;
		ogg_uint32_t   body_fill_next;
		
	} ogg_stream_state;
	
	typedef struct {
		ogg_reference *packet;
		long           bytes;
		long           b_o_s;
		long           e_o_s;
		ogg_int64_t    granulepos;
		ogg_int64_t    packetno;     /* sequence number for decode; the framing
		 knows where there's a hole in the data,
		 but we need coupling so that the codec
		 (which is in a seperate abstraction
		 layer) also knows about the gap */
	} ogg_packet;
	
	typedef struct {
		ogg_reference *header;
		int            header_len;
		ogg_reference *body;
		long           body_len;
	} ogg_page;
	
	/* Ogg BITSTREAM PRIMITIVES: bitstream ************************/
	
	extern void  oggpack_readinit(oggpack_buffer *b,ogg_reference *r);
	extern long  oggpack_look(oggpack_buffer *b,int bits);
	extern void  oggpack_adv(oggpack_buffer *b,int bits);
	extern long  oggpack_read(oggpack_buffer *b,int bits);
	extern long  oggpack_bytes(oggpack_buffer *b);
	extern long  oggpack_bits(oggpack_buffer *b);
	extern int   oggpack_eop(oggpack_buffer *b);
	
	/* Ogg BITSTREAM PRIMITIVES: decoding **************************/
	
	extern ogg_sync_state *ogg_sync_create(void);
	extern int      ogg_sync_destroy(ogg_sync_state *oy);
	extern int      ogg_sync_reset(ogg_sync_state *oy);
	
	extern unsigned char *ogg_sync_bufferin(ogg_sync_state *oy, long size);
	extern int      ogg_sync_wrote(ogg_sync_state *oy, long bytes);
	extern long     ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og);
	extern int      ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
	extern int      ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
	extern int      ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
	extern int      ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op);
	
	/* Ogg BITSTREAM PRIMITIVES: general ***************************/
	
	extern ogg_stream_state *ogg_stream_create(int serialno);
	extern int      ogg_stream_destroy(ogg_stream_state *os);
	extern int      ogg_stream_reset(ogg_stream_state *os);
	extern int      ogg_stream_reset_serialno(ogg_stream_state *os,int serialno);
	extern int      ogg_stream_eos(ogg_stream_state *os);
	
	extern int      ogg_page_checksum_set(ogg_page *og);
	
	extern int      ogg_page_version(ogg_page *og);
	extern int      ogg_page_continued(ogg_page *og);
	extern int      ogg_page_bos(ogg_page *og);
	extern int      ogg_page_eos(ogg_page *og);
	extern ogg_int64_t  ogg_page_granulepos(ogg_page *og);
	extern ogg_uint32_t ogg_page_serialno(ogg_page *og);
	extern ogg_uint32_t ogg_page_pageno(ogg_page *og);
	extern int      ogg_page_packets(ogg_page *og);
	extern int      ogg_page_getbuffer(ogg_page *og, unsigned char **buffer);
	
	extern int      ogg_packet_release(ogg_packet *op);
	extern int      ogg_page_release(ogg_page *og);
	
	extern void     ogg_page_dup(ogg_page *d, ogg_page *s);
	
	/* Ogg BITSTREAM PRIMITIVES: return codes ***************************/
	
#define  OGG_SUCCESS   0
	
#define  OGG_HOLE     -10
#define  OGG_SPAN     -11
#define  OGG_EVERSION -12
#define  OGG_ESERIAL  -13
#define  OGG_EINVAL   -14
#define  OGG_EEOS     -15
	
	
	typedef struct vorbis_info{
		int version;
		int channels;
		long rate;
		
		/* The below bitrate declarations are *hints*.
		 Combinations of the three values carry the following implications:
		 
		 all three set to the same value: 
		 implies a fixed rate bitstream
		 only nominal set: 
		 implies a VBR stream that averages the nominal bitrate.  No hard 
		 upper/lower limit
		 upper and or lower set: 
		 implies a VBR bitstream that obeys the bitrate limits. nominal 
		 may also be set to give a nominal rate.
		 none set:
		 the coder does not care to speculate.
		 */
		
		long bitrate_upper;
		long bitrate_nominal;
		long bitrate_lower;
		long bitrate_window;
		
		void *codec_setup;
	} vorbis_info;
	
	/* vorbis_dsp_state buffers the current vorbis audio
	 analysis/synthesis state.  The DSP state belongs to a specific
	 logical bitstream ****************************************************/
	typedef struct vorbis_dsp_state{
		int analysisp;
		vorbis_info *vi;
		
		ogg_int32_t **pcm;
		ogg_int32_t **pcmret;
		int      pcm_storage;
		int      pcm_current;
		int      pcm_returned;
		
		int  preextrapolate;
		int  eofflag;
		
		long lW;
		long W;
		long nW;
		long centerW;
		
		ogg_int64_t granulepos;
		ogg_int64_t sequence;
		
		void       *backend_state;
	} vorbis_dsp_state;
	
	typedef struct vorbis_block{
		/* necessary stream state for linking to the framing abstraction */
		ogg_int32_t  **pcm;       /* this is a pointer into local storage */ 
		oggpack_buffer opb;
		
		long  lW;
		long  W;
		long  nW;
		int   pcmend;
		int   mode;
		
		int         eofflag;
		ogg_int64_t granulepos;
		ogg_int64_t sequence;
		vorbis_dsp_state *vd; /* For read-only access of configuration */
		
		/* local storage to avoid remallocing; it's up to the mapping to
		 structure it */
		void               *localstore;
		long                localtop;
		long                localalloc;
		long                totaluse;
		struct alloc_chain *reap;
		
	} vorbis_block;
	
	/* vorbis_block is a single block of data to be processed as part of
	 the analysis/synthesis stream; it belongs to a specific logical
	 bitstream, but is independant from other vorbis_blocks belonging to
	 that logical bitstream. *************************************************/
	
	struct alloc_chain{
		void *ptr;
		struct alloc_chain *next;
	};
	
	/* vorbis_info contains all the setup information specific to the
	 specific compression/decompression mode in progress (eg,
	 psychoacoustic settings, channel setup, options, codebook
	 etc). vorbis_info and substructures are in backends.h.
	 *********************************************************************/
	
	/* the comments are not part of vorbis_info so that vorbis_info can be
	 static storage */
	typedef struct vorbis_comment{
		/* unlimited user comment fields.  libvorbis writes 'libvorbis'
		 whatever vendor is set to in encode */
		char **user_comments;
		int   *comment_lengths;
		int    comments;
		char  *vendor;
		
	} vorbis_comment;
	
	
	/* libvorbis encodes in two abstraction layers; first we perform DSP
	 and produce a packet (see docs/analysis.txt).  The packet is then
	 coded into a framed OggSquish bitstream by the second layer (see
	 docs/framing.txt).  Decode is the reverse process; we sync/frame
	 the bitstream and extract individual packets, then decode the
	 packet back into PCM audio.
	 
	 The extra framing/packetizing is used in streaming formats, such as
	 files.  Over the net (such as with UDP), the framing and
	 packetization aren't necessary as they're provided by the transport
	 and the streaming layer is not used */
	
	/* Vorbis PRIMITIVES: general ***************************************/
	
	extern void     vorbis_info_init(vorbis_info *vi);
	extern void     vorbis_info_clear(vorbis_info *vi);
	extern int      vorbis_info_blocksize(vorbis_info *vi,int zo);
	extern void     vorbis_comment_init(vorbis_comment *vc);
	extern void     vorbis_comment_add(vorbis_comment *vc, char *comment); 
	extern void     vorbis_comment_add_tag(vorbis_comment *vc, 
										   char *tag, char *contents);
	extern char    *vorbis_comment_query(vorbis_comment *vc, char *tag, int count);
	extern int      vorbis_comment_query_count(vorbis_comment *vc, char *tag);
	extern void     vorbis_comment_clear(vorbis_comment *vc);
	
	extern int      vorbis_block_init(vorbis_dsp_state *v, vorbis_block *vb);
	extern int      vorbis_block_clear(vorbis_block *vb);
	extern void     vorbis_dsp_clear(vorbis_dsp_state *v);
	
	/* Vorbis PRIMITIVES: synthesis layer *******************************/
	extern int      vorbis_synthesis_headerin(vorbis_info *vi,vorbis_comment *vc,
											  ogg_packet *op);
	
	extern int      vorbis_synthesis_init(vorbis_dsp_state *v,vorbis_info *vi);
	extern int      vorbis_synthesis_restart(vorbis_dsp_state *v);
	extern int      vorbis_synthesis(vorbis_block *vb,ogg_packet *op,int decodep);
	extern int      vorbis_synthesis_blockin(vorbis_dsp_state *v,vorbis_block *vb);
	extern int      vorbis_synthesis_pcmout(vorbis_dsp_state *v,ogg_int32_t ***pcm);
	extern int      vorbis_synthesis_read(vorbis_dsp_state *v,int samples);
	extern long     vorbis_packet_blocksize(vorbis_info *vi,ogg_packet *op);
	
	/* Vorbis ERRORS and return codes ***********************************/
	
#define OV_FALSE      -1  
#define OV_EOF        -2
#define OV_HOLE       -3
	
#define OV_EREAD      -128
#define OV_EFAULT     -129
#define OV_EIMPL      -130
#define OV_EINVAL     -131
#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OV_EVERSION   -134
#define OV_ENOTAUDIO  -135
#define OV_EBADPACKET -136
#define OV_EBADLINK   -137
#define OV_ENOSEEK    -138

#define CHUNKSIZE 1024
/* The function prototypes for the callbacks are basically the same as for
 * the stdio functions fread, fseek, fclose, ftell. 
 * The one difference is that the FILE * arguments have been replaced with
 * a void * - this is to be used as a pointer to whatever internal data these
 * functions might need. In the stdio case, it's just a FILE * cast to a void *
 * 
 * If you use other functions, check the docs for these functions and return
 * the right values. For seek_func(), you *MUST* return -1 if the stream is
 * unseekable
 */
typedef struct {
  size_t (*read_func)  (void *ptr, size_t size, size_t nmemb, void *datasource);
  int    (*seek_func)  (void *datasource, ogg_int64_t offset, int whence);
  int    (*close_func) (void *datasource);
  long   (*tell_func)  (void *datasource);
} ov_callbacks;

#define  NOTOPEN   0
#define  PARTOPEN  1
#define  OPENED    2
#define  STREAMSET 3
#define  INITSET   4

typedef struct OggVorbis_File {
  void            *datasource; /* Pointer to a FILE *, etc. */
  int              seekable;
  ogg_int64_t      offset;
  ogg_int64_t      end;
  ogg_sync_state   *oy; 

  /* If the FILE handle isn't seekable (eg, a pipe), only the current
     stream appears */
  int              links;
  ogg_int64_t     *offsets;
  ogg_int64_t     *dataoffsets;
  ogg_uint32_t    *serialnos;
  ogg_int64_t     *pcmlengths;
  vorbis_info     *vi;
  vorbis_comment  *vc;

  /* Decoding working state local storage */
  ogg_int64_t      pcm_offset;
  int              ready_state;
  ogg_uint32_t     current_serialno;
  int              current_link;

  ogg_int64_t      bittrack;
  ogg_int64_t      samptrack;

  ogg_stream_state *os; /* take physical pages, weld into a logical
                          stream of packets */
  vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
  vorbis_block     vb; /* local working space for packet->PCM decode */

  ov_callbacks callbacks;

} OggVorbis_File;

extern int ov_clear(OggVorbis_File *vf);
extern int ov_open(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
extern int ov_open_callbacks(void *datasource, OggVorbis_File *vf,
		char *initial, long ibytes, ov_callbacks callbacks);

extern int ov_test(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
extern int ov_test_callbacks(void *datasource, OggVorbis_File *vf,
		char *initial, long ibytes, ov_callbacks callbacks);
extern int ov_test_open(OggVorbis_File *vf);

extern long ov_bitrate(OggVorbis_File *vf,int i);
extern long ov_bitrate_instant(OggVorbis_File *vf);
extern long ov_streams(OggVorbis_File *vf);
extern long ov_seekable(OggVorbis_File *vf);
extern long ov_serialnumber(OggVorbis_File *vf,int i);

extern ogg_int64_t ov_raw_total(OggVorbis_File *vf,int i);
extern ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i);
extern ogg_int64_t ov_time_total(OggVorbis_File *vf,int i);

extern int ov_raw_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_time_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_time_seek_page(OggVorbis_File *vf,ogg_int64_t pos);

extern ogg_int64_t ov_raw_tell(OggVorbis_File *vf);
extern ogg_int64_t ov_pcm_tell(OggVorbis_File *vf);
extern ogg_int64_t ov_time_tell(OggVorbis_File *vf);

extern vorbis_info *ov_info(OggVorbis_File *vf,int link);
extern vorbis_comment *ov_comment(OggVorbis_File *vf,int link);

extern long ov_read(OggVorbis_File *vf,char *buffer,int length,
		    int *bitstream);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


