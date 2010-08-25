
#ifndef __RADIOSITY_H
#define __RADIOSITY_H


#include "scene.h"
#include "hemicube.h"
#include "commode.h"

class CRadiosity {
	public:
		CRadiosity(CScene* scene);
		~CRadiosity(void);
		void run(void);
		
		CHemicube *hemi;
		CScene *s;

		float iterThreshold;
		float subDivideThreshold;
		float gamma;
//		float maxPatchArea;					// for initial patch subdivision before calculations
		
	};






inline CRadiosity::~CRadiosity() {
	delete hemi;
}
	
#endif