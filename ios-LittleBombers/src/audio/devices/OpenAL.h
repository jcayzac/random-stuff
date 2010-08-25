#if !defined(UUID_43E78880C4DD48B3B6623EAB297E8142)
#define UUID_43E78880C4DD48B3B6623EAB297E8142

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class OpenAL {
public:
	static bool makeContextCurrent();
	static uint32_t createBuffer(const std::string& path);
	static uint32_t createSource(bool looped=false);
	static void releaseSource(uint32_t source);
	static void releaseBuffer(uint32_t buffer);
	static bool bind(uint32_t buffer, uint32_t source);
	static void play(uint32_t source, float gain=1.f, bool restart=false);
	static void stop(uint32_t source);
	static float volume(uint32_t source);
	static void setVolume(uint32_t source, float gain);
	static float volume();
	static void setVolume(float gain);
	static bool isPlaying(uint32_t source);
private:
	static bool init();
	struct openal_instance_t {
		void* context;
		openal_instance_t(): context(0) { }
		~openal_instance_t();
	};
	static openal_instance_t* instance();
};

#endif // UUID_43E78880C4DD48B3B6623EAB297E8142

