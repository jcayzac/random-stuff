#include "material.h"

CMaterial::CMaterial(void) {

}

CMaterial::~CMaterial(void) {

}

const CMaterial& CMaterial::operator=(const CMaterial& src) {
	name		= src.name;
	col			= src.col;
	diffuse		= src.diffuse;
	glossiness	= src.glossiness;
	light		= src.light;
	metallic	= src.metallic;
	opacity		= src.opacity;
	specular	= src.specular;
	return *this;
}
