#if !defined(__MATERIAL_H)
#define __MATERIAL_H
#include "color.h"

class CMaterial {
public:
	string	name;
	CColor	col;
	float	light;
	float	specular;
	float	diffuse;
	float	glossiness;
	float	opacity;
	bool	metallic;
	const CMaterial& operator=(const CMaterial&);
			CMaterial(void);
	virtual ~CMaterial(void);
};

#endif
