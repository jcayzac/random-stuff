#include "commode.h"
#include "hemi.h"
#include "ray.h"
#include "scene.h"

// Org:
//
// AAAA
// AAAA
// AAAA
// AAAA
// BBBB
// BBBB
// CCCC
// CCCC
// DDDD
// DDDD
// EEEE
// EEEE
// ou A = front, B = up, C = right, D = bottom, E = left
CHemi::CHemi(unsigned precision) {
	ffgrid = new SMLVec3f[3*precision*precision];
	unsigned u,v;
	// section A (front)
	for (v=0; v<precision; v++) {
		for (u=0; u<precision; u++) {
			unsigned i = u + v*precision;
			ffgrid[i].x = (((float)(u))/((float)precision))-.5f;
			ffgrid[i].y = (((float)(v))/((float)precision))-.5f;
			ffgrid[i].z = .5f;
		}
	}
	// section B (up)
	for (v=0; v<precision/2; v++) {
		for (u=0; u<precision; u++) {
			unsigned i = u+ (v+precision)*precision;
			ffgrid[i].x = (((float)(u))/((float)precision))-.5f;
			ffgrid[i].y = .5f;
			ffgrid[i].z = ((float)v)/((float)precision);
		}
	}
	// section C (right)
	for (v=0; v<precision/2; v++) {
		for (u=0; u<precision; u++) {
			unsigned i = u + (v+3*precision/2)*precision;
			ffgrid[i].x = -.5f;
			ffgrid[i].y = (((float)(u))/((float)precision))-.5f;
			ffgrid[i].z = ((float)v)/((float)precision);
		}
	}
	// section D (bottom)
	for (v=0; v<precision/2; v++) {
		for (u=0; u<precision; u++) {
			unsigned i = u+ (v+precision)*precision;
			ffgrid[i].x = (((float)(u))/((float)precision))-.5f;
			ffgrid[i].y = -.5f;
			ffgrid[i].z = ((float)v)/((float)precision);
		}
	}
	// section E (left)
	for (v=0; v<precision/2; v++) {
		for (u=0; u<precision; u++) {
			unsigned i = u + (v+3*precision/2)*precision;
			ffgrid[i].x = .5f;
			ffgrid[i].y = (((float)(u))/((float)precision))-.5f;
			ffgrid[i].z = ((float)v)/((float)precision);
		}
	}
	divisions = 3*precision*precision;
}

CHemi::~CHemi(void) {
	delete[] ffgrid;
}



void CHemi::calculateFormFactors(CRadPatch* patch) {

	links = new CLink[divisions*(scn->nb_faces)];
	nb_links = 0;

	for (int i=0; i<divisions; i++) {
		CRay ray(patch->center, ffgrid[i]);
		for (int f=0; f<scn->nb_faces; f++) { // scn = scene !!!!

			CRadPatch* root = &scn->faces[f]; // scn = scene !!!!
			CRadPatch* current = root;
			bool touched = ray.intersection(current);
	//				if (touched) {
	//					CLink l; 
	//					l.F = 0; //// ?????????????
	//					l.q = current;
	//					addLink(l);
						//links[nb_links] = *l;
						//nb_links++;
	//				}
			
/*
				CHemi hemicube(20);
				hemicube.calculateFormFactors(current);
				for (int i=0; i<hemicube.nb_links; i++) {
					if (hemicube.links[i].F > SEUIL_FORMFACTOR) {
						current->subdivide();
						hemicube.links[i].q->subdivide();
					}
				}
*/		
		}
	}
}


void CHemi::addLink(CLink& link) {
	links[nb_links] = link;
	nb_links++;
}