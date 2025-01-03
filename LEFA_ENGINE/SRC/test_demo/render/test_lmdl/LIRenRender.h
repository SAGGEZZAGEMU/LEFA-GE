#ifndef LIRENRENDER_H
#define LIRENRENDER_H

#include"model_test/model.h"

typedef struct
{
	LIAlgRandom random;
	LIAlgStrdic* fonts;
	LIAlgU32dic* lights;
	LIAlgU32dic* models;
	LIAlgU32dic* objects;
	//LIPthPaths* paths;

	//LIRenUniqueId id;
	LIAlgStrdic* mesh_builders;

}LIRenRender;





#endif /* LIRENRENDER_H */


