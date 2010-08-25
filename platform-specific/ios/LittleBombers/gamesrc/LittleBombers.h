#if !defined(UUID_26908E249C8B4E55B4104ABA2191C54F)
#define UUID_26908E249C8B4E55B4104ABA2191C54F

#include "Game.h"
#include "Model.h"
#include "Texture.h"
#include "Sound.h"
#include "BackgroundMusic.h"
#include "OpenGLES11Renderer.h"

class LittleBombers: protected Game {
public:
	void restartMusic();
	
	// From Game
	bool init(float width, float height);
	void update(float elapsed);
	void draw() const;
	void saveState();
	bool loadState();
	void clear();

private:
	// Event handlers
	void touchesBeganHandler(const std::vector<TouchEvent>& touches);
	void touchesEndedHandler(const std::vector<TouchEvent>& touches);
	void touchesMovedHandler(const std::vector<TouchEvent>& touches);
	void externalAudioHandler(bool systemIsPlaying);
	
private:
	ModelT mStageModel[100], mCharacterModel[8];
	TextureT mStageTexture, mCharacterTexture;
	
	
	boost::shared_ptr<BackgroundMusic> mBackgroundMusic;
	
	
	SoundFX mFX;
	GLfloat _characterRotationY;
	GLfloat _controlX;
	GLfloat _characterX;
	GLfloat _characterTargetX;
	GLfloat _cameraZ;
	GLfloat _cameraTargetZ;	
};

#endif // UUID_26908E249C8B4E55B4104ABA2191C54F

