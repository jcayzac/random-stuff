
#ifndef __SCENELOADER_H
#define __SCENELOADER_H

#include "material.h"
#include "object.h"
#include "commode.h"
#include "scene.h"


#define SCENEFILE "D:\\infographie\\scene.a3d"


#define OBJECT				 100
#define MATERIAL			 101
	
#define TYPE_BOX			1000
#define TYPE_SPHERE			1001
#define TYPE_CYLINDER		1002
#define TYPE_CONE			1003
#define TYPE_NOISY_SPHERE	1004
#define TYPE_TETRAHEDRON	1005

#define PARSING_OK			2000
#define PARSING_ACC_ERROR	2001


////// object decode par le parseur
class CSceneObject {
public:
	string*		material;
	uint32		type;
	uint32		nbparam;
	float		param[10];
	SMLVec3f	origin;
	SMLVec3f	size;
	SMLVec3f	orientation;
	bool		hollow;

	CSceneObject(void);
	virtual ~CSceneObject(void);
};


////// scene decodee par le parseur
class CParsedScene {
public:
	uint32	nb_objects;
	CSceneObject *object;
	uint32	nb_materials;
	CMaterial *material;

	CParsedScene(void);
	virtual ~CParsedScene(void);
};

/*
////// scene avec les vertices... et les faces...
class C3DScene {
public:
	uint32	nb_objects;
	C3DObject *object;
	uint32	nb_materials;
	CMaterial *material;
};
*/

class CSceneParser {
public:
	CSceneParser(string* file);
	virtual ~CSceneParser(void);
	int parse(string* buffer);
//	C3DScene* getScene(void);
	CScene* getScene(void);
	C3DObject& convertTo3dObject(CSceneObject& obj);
	void setParam(string* s, CSceneObject* obj);
	void addObject(CSceneObject &obj);
	void addMaterial(CMaterial &mat);
	void printObject(CSceneObject& obj);
	void printMaterial(CMaterial& mat);

protected:
	string *scenefile;
	CParsedScene* scene;
	CSceneObject *currentObject;
	CMaterial *currentMaterial;
	int matOrObj;
};

#endif