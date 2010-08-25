#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <smlmath.h>
#include <smlxmatrix.h>

#include "commode.h"
#include "image.h"
#include "console.h"
#include "main.h"

#include "tetrahedron.h"
#include "sphere.h"
#include "noisysphere.h"
#include "box.h"
#include "cylinder.h"
#include "cone.h"

#include "scene.h"
#include "screen.h"
#include "sceneloader.h"

//CConsole con;
CScene *scn;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	// test de couleurs
	/*CScreen screen(PHOSPHOR_LONG,ILLU_D65,1.0f,19,4.0f/3.0f);
	CColor c,c2;
	c.set(0.0f,0.0f,0.0f);
	for (unsigned l=440; l<600; l++) {
		c2.set(l);
		c+=c2;
	}
	SMLVec3f rgb=screen.getColorRGB(c);
	FILE* f = fopen("color.txt", "wb");
	fprintf(f,"matrice XYZ->RGB:\n");
	fprintf(f,"%f %f %f\n", screen.XYZtoRGB[0][0], screen.XYZtoRGB[0][1], screen.XYZtoRGB[0][2]);
	fprintf(f,"%f %f %f\n", screen.XYZtoRGB[1][0], screen.XYZtoRGB[1][1], screen.XYZtoRGB[1][2]);
	fprintf(f,"%f %f %f\n\n", screen.XYZtoRGB[2][0], screen.XYZtoRGB[2][1], screen.XYZtoRGB[2][2]);
	fprintf(f,"X=%f Y=%f Z=%f\n",c.X,c.Y,c.Z);
	fprintf(f,"=> R=%i G=%i B=%i\n", (int)(rgb.x*255.0f), (int)(rgb.y*255.0f), (int)(rgb.z*255.0f)); 
	fclose(f);
*/
	// scene de test
/*	CBox wall_left;
	wall_left.setSize(10.0f,4000.0f,4000.0f);
	wall_left.setOrigin(-2000.0f,2000.0f,0.0f);

	CBox wall_right;
	wall_right.setSize(10.0f,4000.0f,4000.0f);
	wall_right.setOrigin(2000.0f,2000.0f,0.0f);

	CBox wall_back;
	wall_back.setSize(4000.0f,4000.0f,10.0f);
	wall_back.setOrigin(.0f,2000.0f,-2000.0);

	CBox floor;
	floor.setSize(4000.0f,10.0f,4000.0f);
	floor.setOrigin(.0f,-5.0f,.0f);

	CBox wall_up;
	wall_up.setSize(4000.0f,10.0f,4000.0f);
	wall_up.setOrigin(.0f,4000.0f,.0f);

	CSphere egg(3);
	egg.setSize(450.0f,600.0f,450.0f);
	egg.setOrigin(.0f,600.0f,.0f);
	egg.setOrientation(.0f,.0f,pi/4.0f);

	CSphere ball(3);
	ball.setSize(600.0f,600.0f,600.0f);
	ball.setOrigin(-1000.0f,600.0f,-200.0f);

	CBox cube;
	cube.setSize(1200.0f,1200.0f,1200.0f);
	cube.setOrigin(1000.0f,600.0f,1000.0f);
	cube.setOrientation(.0f,pi/4.0f,.0f);

	scene.addObject(*(wall_left.getWorldObject()));
	scene.addObject(*(wall_right.getWorldObject()));
	scene.addObject(*(wall_back.getWorldObject()));
	scene.addObject(*(floor.getWorldObject()));
	scene.addObject(*(wall_up.getWorldObject()));
	scene.addObject(*(egg.getWorldObject()));
	scene.addObject(*(ball.getWorldObject()));
	scene.addObject(*(cube.getWorldObject()));

  */

	// test de parsing
/*	
	CSceneParser parser(new string("scene.a3d"));
	CScene *scn = parser.getScene();
	scn->dump("scene.off");
	delete scn;
*/
/*
	// test d'intersections
	CFace f;
	f.vert[0].x = -1.0f;
	f.vert[0].y = -1.0f;
	f.vert[0].z = 1.0f;
	f.vert[1].x = .0f;
	f.vert[1].y = 1.0f;
	f.vert[1].z = 1.0f;
	f.vert[2].x = 1.0f;
	f.vert[2].y = -1.0f;
	f.vert[2].z = 1.0f;
	SMLVec3f A,B;
	A.x = f.vert[2].x-f.vert[0].x;
	A.y = f.vert[2].y-f.vert[0].y;
	A.z = f.vert[2].z-f.vert[0].z;
	B.x = f.vert[1].x-f.vert[0].x;
	B.y = f.vert[1].y-f.vert[0].y;
	B.z = f.vert[1].z-f.vert[0].z;
	f.norm.x = A.y*B.z - A.z*B.y;
	f.norm.y = A.z*B.x - A.x*B.z;
	f.norm.z = A.x*B.y - A.y*B.x;
	f.norm.Normalize();
	f.plane_value = - f.norm.Dot(f.vert[0]);
	SMLVec3f start, dir;
	start.x = start.y = start.z = 0.0f;
	dir.x   = 0.0f;
	dir.y	= 0.0f;
	dir.z	= 1.0f;
	CRay r(start,dir);
	if (f.intersection(r)) {
		Tuv i = f.getUV(r.end);
	}
*/
	// test images
	/*
	CImage image;
	image.readTGA("image1.tga");
	image.writeTGA("write.tga");
	*/
	// 
	CSceneParser parser(new string("scene.a3d"));
	scn = parser.getScene();
	CImage image;
	CScreen screen(PHOSPHOR_LONG,ILLU_D65,1.0f,19,4.0f/3.0f);
	scn->render(screen,image);

	return 0;
}
