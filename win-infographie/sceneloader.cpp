

#include "sceneloader.h"
#include "color.h"
#include "commode.h"

#include "sphere.h"
#include "noisysphere.h"
#include "cone.h"
#include "cylinder.h"
#include "tetrahedron.h"
#include "box.h"



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CSceneObject::CSceneObject() {
	material = NULL;
	type = 0;
	nbparam = 0;
	origin.x = 0; origin.y = 0;	origin.z = 0;
	size.x = 0;	size.y = 0;	size.z = 0;
	orientation.x = 0; orientation.y = 0; orientation.z = 0; 
	hollow = false;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CSceneObject::~CSceneObject() {
}







/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


CParsedScene::CParsedScene(){ 

	nb_objects   = 0;
	object	     = NULL;
	nb_materials = 0;
	material     = NULL;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CParsedScene::~CParsedScene(){ 

}







/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CSceneParser::CSceneParser(string* file) {
	scenefile = file;
	currentMaterial = NULL;
	currentObject = NULL;
	scene = new CParsedScene();
                               
	debug ("\n*** opening scene file...\n");
	
	char* buffer = new char[256];
	ifstream* sceneFile = new ifstream(file->c_str(),ios::binary);
	int cpt =0;
	do {
		sceneFile->getline(buffer,256);
		if (buffer[0]!='\0') {
			cpt++;
			string* str = new string(buffer);
			if (parse(str) != PARSING_OK) {
				string s("Erreur à la ligne ");
				char* cpt_str = new char[20];
				itoa(cpt,cpt_str,10);
				s += cpt_str;
				s += ": chaine imcompréhensible => "; 
				s += buffer;
				debug (s.c_str());
			}
		}
	} while (!sceneFile->eof());
	sceneFile->close();
	debug ("*** scene file read and closed.");
	
	debug ("\nListe des materiaux...\n");
	for (int i=0; i<scene->nb_materials; i++) {
		printMaterial(scene->material[i]);	
	}
	debug ("\nListe des objets...\n");
	for (int j=0; j<scene->nb_objects; j++) {
		printObject(scene->object[j]);	
	}



}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
	     
void CSceneParser::printObject(CSceneObject& obj) {

	char* cpt_str = new char[20];
	int cpt =0;

	string s("object : {(type=");

	s += itoa(obj.type,cpt_str,10);
	s += ")(material=";
	s += obj.material->c_str();
	s += ")(param("; 
	if (obj.nbparam > 0) {
		for (int j=0; j<obj.nbparam; j++) {
			s += itoa(obj.param[j],cpt_str,10);
			if (j != obj.nbparam -1) s += ",";
		}
	}
	s += ")(origin(";

	char* buff = new char[200];
	sprintf(buff,"%f,%f,%f",obj.origin.x,obj.origin.y,obj.origin.z);
	s += buff;
	s += "))(orientation(";
	sprintf(buff,"%f,%f,%f",obj.orientation.x,obj.orientation.y,obj.orientation.z);
	s += buff;
	s += "))(size(";
	sprintf(buff,"%f,%f,%f",obj.size.x,obj.size.y,obj.size.z);
	s += buff;
	s += "))(hollow=";
	if (obj.hollow == true) {
		s += "TRUE";
	} else {
		s += "FALSE";
	}
	s += ")}\n";
	debug (s);
}




/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void CSceneParser::printMaterial(CMaterial& mat) {
	
	char* cpt_str = new char[20];
	int cpt =0;
	string s("material : {(name=");

	s += mat.name.c_str();
	s += ")(color=";
	
	char* buff = new char[256];
	sprintf(buff,"%f,%f,%f", mat.col.X,mat.col.Y,mat.col.Z);
	s += buff;
	s += ")(diffuse=";
	s += itoa(mat.diffuse,cpt_str,10);
	s += ")(glossiness=";
	s += itoa(mat.glossiness,cpt_str,10);
	s += ")(light=";
	s += itoa(mat.light,cpt_str,10);
	s += ")(opacity=";
	s += itoa(mat.opacity,cpt_str,10);
	s += ")(specular=";
	s += itoa(mat.specular,cpt_str,10);
	s += ")(opacity=";
	s += itoa(mat.opacity,cpt_str,10);
	s += ")(metallic=";
	if (mat.metallic == true) {
		s += "TRUE";
	} else {
		s += "FALSE";
	}
	s += ")}\n";
	debug (s);

}





/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CSceneParser::~CSceneParser() {
	delete currentMaterial;
	delete currentObject;
	delete scene;
	delete scenefile;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int CSceneParser::parse(string* buffer) {

	bool ok = false;
	while (!ok) {
		if (buffer->find("\t") != -1) {
			buffer->replace(buffer->find("\t"),1,"");	
			ok = false;
		} else if (buffer->find("  ") != -1) {
			buffer->replace(buffer->find("  "),2," ");	
			ok = false;
		} else if (buffer->find(", ") != -1) {
			buffer->replace(buffer->find(", "),2,",");
			ok = false;
		} else {
			ok = true;
		}
	}

	if (buffer->find("//") != -1) {
		return PARSING_OK;
	}

	if (strcmp(buffer->c_str(),"\r") == 0) {
		return PARSING_OK;
			
	} else if (buffer->find("nb_materials") != -1) {
		int start = buffer->find("nb_materials ")+13;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		int nb_materials = atoi(s->c_str());
		// allocation du nombre de materiaux
		scene->material = new CMaterial[nb_materials];
		return PARSING_OK;
	
	} else if (buffer->find("nb_objects") != -1) {
		int start = buffer->find("nb_objects ")+11;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		int nb_objects = atoi(s->c_str());
		// allocation du nombre d'objets
		scene->object = new CSceneObject[nb_objects];
		return PARSING_OK;
	
	} else if (buffer->find("{") != -1) {
		if (buffer->find("material") != -1) {
			scene->nb_materials++;
			currentMaterial = new CMaterial();
			currentMaterial->metallic = false;
			matOrObj = MATERIAL;
			return PARSING_OK;
			
		} else if (buffer->find("object") != -1) {
			scene->nb_objects++;
			currentObject = new CSceneObject();
			currentObject->hollow = false;
			matOrObj = OBJECT;
			return PARSING_OK;

		} else return PARSING_ACC_ERROR;
	
	} else if (buffer->find("}") != -1) {

		// ajout de l'objet ou du materiaux
		if (matOrObj == OBJECT) {
			addObject(*currentObject);
			return PARSING_OK;

		} else if (matOrObj == MATERIAL) {
			addMaterial(*currentMaterial);
			return PARSING_OK;
		}

	} else if (buffer->find("name ") != -1) {
		int start = buffer->find("name ")+5;
		int end = buffer->length()-1;
		currentMaterial->name = buffer->substr(start,end-start);
		return PARSING_OK;

	} else if (buffer->find("color ") != -1) {
		int start = buffer->find("color ")+6;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start));
		int w_min = atoi(s->substr(0,s->find("-") - 1).c_str());
		int w_max = atoi(s->substr(s->find("-") + 1, s->length() - s->find("-")).c_str());
		currentMaterial->col.addRange(w_min, w_max);
		return PARSING_OK;

	} else if (buffer->find("light ") != -1) {
		int start = buffer->find("light ")+6;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		currentMaterial->light = atof(s->c_str());
		return PARSING_OK;

	} else if (buffer->find("specular ") != -1) {
		int start = buffer->find("specular ")+9;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		currentMaterial->specular = atof(s->c_str());
		return PARSING_OK;

	} else if (buffer->find("diffuse ") != -1) {
		int start = buffer->find("diffuse ")+8;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		currentMaterial->diffuse = atof(s->c_str());
		return PARSING_OK;

	} else if (buffer->find("glossiness ") != -1) {
		int start = buffer->find("glossiness ")+11;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		currentMaterial->glossiness = atof(s->c_str());
		return PARSING_OK;

	} else if (buffer->find("opacity ") != -1) {
		int start = buffer->find("opacity ")+8;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		currentMaterial->opacity = atof(s->c_str());
		return PARSING_OK;

	} else if (buffer->find("metallic") != -1) {
		currentMaterial->metallic = true;
		return PARSING_OK;

	} else if (buffer->find("material ") != -1) {
		int start = buffer->find("material ")+9;
		int end = buffer->length()-1;
		currentObject->material = new string(buffer->substr(start,end-start));
		return PARSING_OK;

	} else if (buffer->find("origin ") != -1) {
		int start = buffer->find("origin ")+7;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 

		// on recupere les 3 coordonnees
		int x_start	= 0;
		int x_end	= s->find_first_of(",") - 1; 
		int y_start	= x_end + 2;
		int y_end	= s->find_last_of(",") - 1;
		int z_start	= y_end + 2;
		int z_end	= s->length() - 1;
		currentObject->origin.x = atof(s->substr(x_start,x_end-x_start+1).c_str()); 
		currentObject->origin.y = atof(s->substr(y_start,y_end-y_start+1).c_str());
		currentObject->origin.z = atof(s->substr(z_start,z_end-z_start+1).c_str());
		return PARSING_OK;

	} else if (buffer->find("size ") != -1) {
		int start = buffer->find("size ")+5;
		int end = buffer->length()-2;
		string* s = new string(buffer->substr(start,end)); 

		// on recupere les 3 coordonnees
		int x_start	= 0;
		int x_end	= s->find_first_of(",") - 1; 
		int y_start	= x_end + 2;
		int y_end	= s->find_last_of(",") - 1;
		int z_start	= y_end + 2;
		int z_end	= s->length()-1;
		currentObject->size.x = atof(s->substr(x_start,x_end-x_start+1).c_str()); 
		currentObject->size.y = atof(s->substr(y_start,y_end-y_start+1).c_str());
		currentObject->size.z = atof(s->substr(z_start,z_end-z_start+1).c_str());
		return PARSING_OK;

	} else if (buffer->find("orientation ") != -1) {
		int start = buffer->find("orientation ")+12;
		int end = buffer->length()-2;
		string* s = new string(buffer->substr(start,end)); 
		
		// on recupere les 3 coordonnees
		int x_start	= 0;
		int x_end	= s->find_first_of(",") -1; 
		int y_start	= x_end + 2;
		int y_end	= s->find_last_of(",") - 1;
		int z_start	= y_end + 2;
		int z_end	= s->length()-1;
		currentObject->orientation.x = atof(s->substr(x_start,x_end-x_start+1).c_str()); 
		currentObject->orientation.y = atof(s->substr(y_start,y_end-y_start+1).c_str());
		currentObject->orientation.z = atof(s->substr(z_start,z_end-z_start+1).c_str());
		return PARSING_OK;

	} else if (buffer->find("hollow") != -1) {
		currentObject->hollow = true;
		return PARSING_OK;

	} else if (buffer->find("type ") != -1) {
		int start = buffer->find("type ")+5;
		int end = buffer->length()-1;
		string* s = new string(buffer->substr(start,end-start)); 
		// quel type d'object ?
		if (s->find("sphere(") != -1) {
			currentObject->type = TYPE_SPHERE;
			setParam(s, currentObject);
			return PARSING_OK;

		} else if (s->find("cone(") != -1) {
			currentObject->type = TYPE_CONE;
			setParam(s, currentObject);
			return PARSING_OK;

		} else if (s->find("box") != -1) {
			currentObject->type = TYPE_BOX;
			setParam(s, currentObject);
			return PARSING_OK;

		} else if (s->find("cylinder(") != -1) {
			currentObject->type = TYPE_CYLINDER;
			setParam(s, currentObject);
			return PARSING_OK;
		
		} else if (s->find("noisy_sphere(") != -1) {
			currentObject->type = TYPE_NOISY_SPHERE;
			setParam(s, currentObject);
			return PARSING_OK;
		
		} else if (s->find("tetrahedron(") != -1) {
			currentObject->type = TYPE_TETRAHEDRON;
			setParam(s, currentObject);
			return PARSING_OK;
		}
	} 
	return PARSING_ACC_ERROR;
}




/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

C3DObject& CSceneParser::convertTo3dObject(CSceneObject& obj) {
	C3DObject *o = NULL;
	switch(obj.type) {
	case TYPE_SPHERE:       o = new CSphere(obj.param[0]); break;
	case TYPE_CONE:         o = new CCone(obj.param[0]); break;
	case TYPE_CYLINDER:     o = new CCylinder(obj.param[0]); break;
	case TYPE_BOX:          o = new CBox(); break;
	case TYPE_NOISY_SPHERE: o = new CNoisySphere(obj.param[0],obj.param[1]); break;
	case TYPE_TETRAHEDRON:  o = new CTetrahedron(); break;
	}
	if (o!=NULL) {
		o->setOrientation(obj.orientation.x,obj.orientation.y,obj.orientation.z);
		o->setOrigin(obj.origin.x,obj.origin.y,obj.origin.z);
		o->setSize(obj.size.x,obj.size.y,obj.size.z);
	}
	return  *o;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

CScene* CSceneParser::getScene() {

	CScene* finalScene = new CScene();
	debug ("\n\n*** generating scene...\n");

	for (int i=0; i<scene->nb_materials; i++) {
		finalScene->addMaterial(scene->material[i]);
	}
	
	for (int j=0; j<scene->nb_objects; j++) {
		finalScene->addObject(*(convertTo3dObject(scene->object[j]).getWorldObject()));
	}

	char* buff = new char[256];
	sprintf(buff,"\n- nb_faces = %i \n- nb_vertices = %i \n- nb_materiaux = %i", finalScene->nb_faces, finalScene->nb_verts, finalScene->nb_mats);
	debug (buff);
	delete buff;
	debug ("\n\n*** scene created.\n");

	return finalScene;
}




	
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void CSceneParser::setParam(string* str, CSceneObject* obj) {
	

	if (str->find("(") == -1 || str->find(")") == -1) {
		obj->nbparam = 0;
		return;
	}

	// on enleve les superflues
	string* s = new string(str->substr(str->find("(") , str->length() -2));


	// 0 param
	if ( s->find(")") == (s->find("(") +1) )  {
		obj->nbparam = 0;
			
	// plusieurs param
	} else {
		int param_start = s->find("(") + 1;
		int param_end   = s->find(",");
		// 1 param
		if (param_end == -1) {
			param_end = s->find(")") - 1;
			obj->param[obj->nbparam++] = atof(s->substr(param_start,param_end).c_str());
		return;
		}
		
		while (param_end != -2) {	
			obj->param[obj->nbparam++] = atof(s->substr(param_start,param_end).c_str());
			s = new string(s->substr(param_end + 1, s->length() - 1));

			param_start = 0;
			param_end = s->find(",") ;
			// cas du dernier param
			if (param_end == - 1) {
				param_end = s->find(")") ;
				obj->param[obj->nbparam++] = atof(s->substr(param_start,param_end).c_str());
				break;
			}
		}	
	}
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void CSceneParser::addObject(CSceneObject &obj) {

	scene->object[scene->nb_objects-1] = obj;
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void CSceneParser::addMaterial(CMaterial &mat) {
	
	scene->material[scene->nb_materials-1] = mat;	
}