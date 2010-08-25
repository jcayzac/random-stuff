#if !defined(UUID_3821EF6D51984857BEED68D6C5CC3F36)
#define UUID_3821EF6D51984857BEED68D6C5CC3F36

#include <boost/signals2.hpp>
#include <string>

class BackgroundMusic {
	typedef boost::signals2::signal<void ()> playback_ended_signal_t;
public:
	BackgroundMusic() {}
	~BackgroundMusic();
	BackgroundMusic(const BackgroundMusic& o): mOpaque(o.mOpaque) { }
	
	bool init(const std::string& path);
	void play(int loops=0, float initialVolume=1.f);
	void stop(bool notifyDelegate=true);
	float volume() const;
	void setVolume(float value);
	bool isPlaying() const;
	playback_ended_signal_t playbackEndedSignal;
private:
	struct opaque_t;
	boost::shared_ptr<opaque_t> mOpaque;
};

#endif // UUID_3821EF6D51984857BEED68D6C5CC3F36

