#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <boost/weak_ptr.hpp>
#include "Game.h"

boost::shared_ptr<Game> Game::instance() {
	static boost::weak_ptr<Game> _instance;
	if(boost::shared_ptr<Game> r = _instance.lock()) return r;
	boost::shared_ptr<Game> game(createGame());
	_instance = game;
	return game;
}

void Game::setupPerspective(float near, float far, float fov, float width, float height) {
	float size = near * tanf(fov*.0087266462f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float x=(size*height)/width;
	glFrustumf(-size,size,-x,x,near,far);
	glViewport(0,0,width,height);
}

void Game::setupOrtho(float near, float far, float width, float height) {
	glOrthof(.0f, width, height, .0f, near, far);
	glViewport(0,0,width,height);
}

Game::~Game() {
	DEBUG_LOG("★★★★★★★ Exiting game ★★★★★★★");
}

