#if !defined(GAME_H_INCLUDED)
#define GAME_H_INCLUDED

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <boost/signals2.hpp>

// Closely match UITouchEvent from the iPhone SDK
struct TouchEvent {
	typedef enum {
		BEGAN,      // whenever a finger touches the surface.
		MOVED,      // whenever a finger moves on the surface.
		STATIONARY, // whenever a finger is touching the surface but hasn't moved since the previous event.
		ENDED,      // whenever a finger leaves the surface.
		CANCELLED   // whenever a touch doesn't end but we need to stop tracking (e.g. putting device to face)
	} Phase;
	
	double timestamp;
	Phase phase;
	int tapCount;
	float screenX, screenY;
	float previousX, previousY;
};

namespace signals {
	typedef boost::signals2::signal<void (const std::vector<TouchEvent>&)> touches_t;
	typedef boost::signals2::signal<void (bool)> external_audio_t;
};


class Game: boost::noncopyable {
protected:
	Game() { }
public:
	virtual ~Game();
	static boost::shared_ptr<Game> instance();
	
	// ★★★★★ Implement this: ★★★★★
	virtual bool init(float width, float height)=0;
	virtual void update(float elapsed)=0;
	virtual void draw() const=0;
	
	virtual void saveState()=0;
	virtual bool loadState()=0;
	virtual void clear()=0;
	
public:
	// Signals
	signals::touches_t touchesBegan;
	signals::touches_t touchesEnded;
	signals::touches_t touchesMoved;
	signals::external_audio_t externalAudio;
	
private:
	static Game* createGame();
	// ★★★★★★★★★★★★★★★★★★★★★★★★★★★

protected:
	void setupPerspective(float near, float far, float fov, float width, float height);
	void setupOrtho(float near, float far, float width, float height);
};

#endif // GAME_H_INCLUDED
