
#include "radiosity.h"
#include <float.h>


CRadiosity::CRadiosity(CScene* s) { 
	
	hemi = new CHemicube(20);	
	for( int g=0; g<s->nb_faces; g++) {		// pour chaque patch, on projete tous les autres dessus
		CRadPatch* current = &s->faces[g];
		current->hemiLocal = hemi;

		for (int k=0; k<s->nb_faces; k++) {
			if (k != g) {

				current->project(&s->faces[k]);
			}
		}

										// calcul de la radiosite 
		for( int c=0; c<3; c++) {		// for each color component
					
					/*
					*	Note:  Need to do in this order to save time and space.
					*	Reflective color component is multipled into components in form factor matrix.
					*	This should be done only once before the iteration starts
					*	However, there are three of these color components, and creating three 
					*	form factor matrices would take too much space
					*	So, must do colors one at a time.
					*
					*/
			
			// get this patch's reflectance of this color component
			SMLVec3f *reflectance	= &current->R;
			float *patchFormFactors	= current->ff;			
			float componentSum		= 0;
						
			
			// for each other patch in scene, add in contribution
			int otherPatchCount = 0;
			for( int gOther=0; gOther<s->nb_faces; gOther++) { // for each other group
				
				CRadPatch* tmp = &s->faces[gOther];			
				float component = tmp->Bs.data()[c];
				// form factor of this patch times radiosity of other patch
				componentSum += /*current->ff[gOther] **/ component;
			}	
					
			// set patch radiosity to new value 
			current->Bg.data()[c] = current->E.data()[c] + reflectance->data()[c] * componentSum;	
		}
	
		float max = -infinity;
		if (current->Bg.data()[0] > max) max = current->Bg.data()[0];
		if (current->Bg.data()[1] > max) max = current->Bg.data()[1];
		if (current->Bg.data()[1] > max) max = current->Bg.data()[2];

		float invmax = 1/max;
		current->Bg.x *= invmax;
		current->Bg.y *= invmax;
		current->Bg.z *= invmax;

		char buff[56];
		sprintf(buff,"patch %i, radiosity=%f,%f,%f\n",g,current->Bg.data()[0],current->Bg.data()[1],current->Bg.data()[2]);
		debug(buff);
	}
}

	

	