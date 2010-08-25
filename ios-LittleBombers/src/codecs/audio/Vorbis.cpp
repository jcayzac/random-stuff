#include <stdio.h>
#include <errno.h>
#include <Tremor/Tremor.h>
#include "Vorbis.h"

namespace codecs {
	namespace audio {
		Vorbis Vorbis::SELF;
		int Vorbis::accept(const std::string& path) const {
			if (path.find(".ogg")==path.length()-4) return 10;
			return 0;
		}
		
		struct vorbis_struct {
			char* base;
			char* current;
			size_t total;
			vorbis_struct(const Buffer* b) {
				base=(char*)b->address();
				current=base;
				total=b->size();
			}
		};
			
		static size_t read_func(void *ptr, size_t size, size_t nmemb, void* datasource) {
			vorbis_struct& vs = *((vorbis_struct*) datasource);
			size_t remaining = (vs.total-(vs.current-vs.base))/nmemb;
			if (size>remaining) size=remaining;
			if (size>0)
				memcpy(ptr, vs.current, size*nmemb);
			return size;
		}
		static int seek_func(void *datasource, ogg_int64_t offset, int whence) {
			vorbis_struct& vs = *((vorbis_struct*) datasource);
			char* newptr=0;
			switch(whence) {
				case SEEK_SET: newptr=vs.base+offset; break;
				case SEEK_CUR: newptr=vs.current+offset; break;
				case SEEK_END: newptr=vs.base+vs.total-offset; break;
			}
			if (newptr<vs.base) { errno=EINVAL; return -1; }
			if (newptr>=vs.base+vs.total) { errno=EOVERFLOW; return -1; }
			return 0;
		}
		static int close_func(void *datasource) { return 0; }
		static long tell_func(void *datasource) {
			vorbis_struct& vs = *((vorbis_struct*) datasource);
			return (vs.current-vs.base);
		}
		
		Buffer* Vorbis::codecDecode(const Buffer* original, const std::string& path) {
			vorbis_struct vs(original);
			ov_callbacks ovc;
			ovc.read_func  = read_func;
			ovc.seek_func  = seek_func;
			ovc.close_func = close_func;
			ovc.tell_func  = tell_func;
			
			OggVorbis_File vf;
			if (ov_open_callbacks(&vs, &vf, 0, 0, ovc)<0) {
				DEBUG_LOG("[Vorbis] Not a vorbis file: « " << path << " »");
				return 0;
			}
			vorbis_info *vi = ov_info(&vf,-1);
			assert(vi->channels>0 && vi->channels<3);
			AudioEnveloppe* e = createEnveloppe((long)ov_pcm_total(&vf, -1), vi->channels, 16, vi->rate);
			if (!e) {
				DEBUG_LOG("[Vorbis] Error allocating destination buffer");
				ov_clear(&vf);
				return 0;
			}
			
			int off=0;
			while (off<e->dataSize) {
				int current_section;
				int ret = ov_read(&vf, (char*)(e->data()+off), e->dataSize-off, &current_section);
				if (!ret) break;
				else if (ret < 0) { DEBUG_LOG("[Vorbis] Time warp! (๏ັ‥̲๏ັ⟱"); }
				else off += ret;
			}
			ov_clear(&vf);
			return new (nothrow) Buffer((uintptr_t) e, sizeof(AudioEnveloppe) + e->dataSize);
		}

		void* Vorbis::decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate) {
			FILE *fh;
			if (!(fh = fopen(path.c_str(), "r"))) {
				DEBUG_LOG("[Vorbis] Failed to open « " << path << " »");
				return 0;
			}
			OggVorbis_File vf;
			if (ov_open(fh, &vf, 0, 0)<0) {
				DEBUG_LOG("[Vorbis] Not a vorbis file: « " << path << " »");
				fclose(fh);
				return 0;
			}
			vorbis_info *vi = ov_info(&vf,-1);
			channels = (int)vi->channels;
			size = 2*channels*(int) ov_pcm_total(&vf, -1);
			rate = (int)vi->rate;
			bits = 16;
			assert(channels>0 && channels<3);
			
			char* data=(char*) malloc(size);
			int off=0;
			while (off<size) {
				int current_section;
				int ret = ov_read(&vf, &data[off], size-off, &current_section);
				if (!ret) break;
				else if (ret < 0) { DEBUG_LOG("[Vorbis] Time warp! (๏ັ‥̲๏ັ⟱"); }
				else off += ret;
			}
			ov_clear(&vf);
			fclose(fh);
			return data;
		}
	}
}
