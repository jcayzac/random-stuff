#include "LittleBombers.h"

// Game factory
Game* Game::createGame() { return new (nothrow) LittleBombers; }



void LittleBombers::restartMusic() {
	DEBUG_LOG("[LittleBombers] (Re)starting background music…");
	if (mBackgroundMusic) mBackgroundMusic->play(0,1);
}

bool LittleBombers::init(float width, float height) {
	_characterX = 0;
	_characterTargetX = 0;
	_characterRotationY = 90;
	_cameraZ = 0;
	_cameraTargetZ = 0;
	_controlX = 0;

	// ★★★★★★★★★★★ Load resources ★★★★★★★★★★★
	
	// Background music
	mBackgroundMusic.reset(new (nothrow) BackgroundMusic);
	if (mBackgroundMusic) {
		if (!mBackgroundMusic->init("City Night Crowd.mp4")) {
			DEBUG_LOG("[LittleBombers] Can't start background music…");
			return false;
		}
		mBackgroundMusic->playbackEndedSignal.connect(boost::bind(&LittleBombers::restartMusic, this));
		restartMusic();
	}
	else {
		DEBUG_LOG("[LittleBombers] Can't alloc background music…");
	}
	
	
	for (int i=0; i<10; ++i)
		if (!mStageModel[i].init("Stage1a.md2"))
			return false;
	
	for (int i=0; i<8; ++i) {
		if (!mCharacterModel[i].init("megaman.md2"))
			return false;
		mCharacterModel[i].setAnimation(ModelT::ANIM_WAVE);
	}
	if (!mStageTexture.init("Stage1a_Grid.pvr"))
		return false;
	if (!mCharacterTexture.init("megaman.pvr"))
		return false;
	
	
	if (!mFX.init("Crickets.ogg", true))
		return false;

	setupPerspective(.1f, 1000.f, 60.f, width, height);

	// Setup opengl
	glEnableClientState( GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY);
	glEnableClientState( GL_NORMAL_ARRAY);
	// glEnable( GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Normals are encoded as bytes whose value is -127..+127,
	// so we need them to be normalized:
	glEnable(GL_NORMALIZE);
	// Scale the texture matrix so that -32767..+32767 gets normalized to -1..+1:
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	const float fiou = 1.f/32767.f;
	glScalef(fiou, fiou, 1.f);
	glMatrixMode(GL_MODELVIEW);
	
	glEnable( GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	
	externalAudio.connect(boost::bind(&LittleBombers::externalAudioHandler, this, _1));
	touchesBegan.connect(boost::bind(&LittleBombers::touchesBeganHandler, this, _1));
	touchesEnded.connect(boost::bind(&LittleBombers::touchesEndedHandler, this, _1));
	touchesMoved.connect(boost::bind(&LittleBombers::touchesMovedHandler, this, _1));
	
	GLint alphaBits;
	glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
	return true;
}
void LittleBombers::update(float elapsed) {
	for (int i=0; i<10; ++i)
		mStageModel[i].update(elapsed);
	
	for (int i=0; i<8; ++i)
		mCharacterModel[i].update(elapsed);
	
	_cameraTargetZ = -0.f - fabsf(_characterX * 2.13f);
	_cameraZ += (_cameraTargetZ - _cameraZ) * 0.1f;
	
	_characterTargetX += _controlX * elapsed * 20;
	_characterX += (_characterTargetX - _characterX) * 0.6f;
	
	
}

void LittleBombers::draw() const {	
	glClearColor( 0, 0.0f, 0.12f, 1);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	// Setup Camera
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	
//	if (isLandscape)
//		glRotatef( -90, 0, 0, 1 );
	
	glTranslatef( 0, -5, _cameraZ);
	glRotatef( -45.0f, 1, 0, 0 );
	glRotatef( -90.0f, 0, 0, 1 );
	
	// Lights
	static GLfloat lightPosition[4] = { 0, 200, 0, 1};
	static GLfloat lightDefuse[4] = { 1, 1, 1, 1};
	static GLfloat lightAmbient[4] = { 0.6, 0.6, 0.6, 1};
	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
//	glEnable( GL_LIGHTING);
//	glEnable( GL_LIGHT0);
//	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDefuse);
//	glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient);
//	glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 50);
//	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition);
	
	// Draw Stage
	mStageTexture.bind(0);
	const VertexBufferT& stageVB = mStageModel[0].renderBuffer();
	for (int x=-5; x<=5; ++x) {
		for (int y=-2; y<=2; ++y) {
			glPushMatrix();
			glTranslatef(y*64, x*64, 0);
			glScalef( 2, 2, 2);
			//processRenderBuffer(mStageModel[(y+1)*3+x+1].renderBuffer());
			processRenderBuffer(stageVB);
			glPopMatrix();
		}
	}
	
	// Draw Character
	mCharacterTexture.bind(0);
	glPushMatrix();
	glTranslatef( -8, _characterX, 2.5f);
	glRotatef( _characterRotationY, 0, 0, 1);
	glScalef( 0.119f, 0.119f, 0.119f);
	processRenderBuffer(mCharacterModel[0].renderBuffer());
	glPopMatrix();

	const VertexBufferT& modelVB = mCharacterModel[1].renderBuffer();
	for (int i=1; i<8; ++i) {
		glPushMatrix();
		glTranslatef( -8, i*4, 2.5f);
		glScalef( 0.12f, 0.12f, 0.12f);
		//processRenderBuffer(mCharacterModel[i].renderBuffer());
		processRenderBuffer(modelVB);
		glPopMatrix();
	}
	
	glDisable( GL_LIGHT0);
	glDisable( GL_LIGHTING);
	
}


void LittleBombers::saveState() {
}
bool LittleBombers::loadState() {
	return true;
}
void LittleBombers::clear() {
}



void LittleBombers::touchesBeganHandler(const std::vector<TouchEvent>& touches) {
	const TouchEvent& touch = touches[0];
	
	_controlX = (touch.screenX / 160.f) - 1;
	_characterRotationY = _controlX > 0 ? 90 : -90;
	
	mCharacterModel[0].setAnimation(ModelT::ANIM_MOVE);
	mCharacterModel[0].setFpsScale(fabsf(_controlX));
	
	mFX.play();	
}

void LittleBombers::touchesEndedHandler(const std::vector<TouchEvent>& touches) {
	mCharacterModel[0].setAnimation(ModelT::ANIM_WAVE);
	_controlX = 0;
	_characterRotationY = 0;
}

void LittleBombers::touchesMovedHandler(const std::vector<TouchEvent>& touches) {
	const TouchEvent& touch = touches[0];
	_controlX = (touch.screenX / 160.0f) - 1;
	_characterRotationY = _controlX > 0 ? 90 : -90;
	
	mCharacterModel[0].setFpsScale(fabsf(_controlX));
	
}

void LittleBombers::externalAudioHandler(bool systemIsPlaying) {
	if (systemIsPlaying && mBackgroundMusic) mBackgroundMusic->stop();
	else restartMusic();
}



