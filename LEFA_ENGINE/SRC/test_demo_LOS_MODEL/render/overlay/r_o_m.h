#ifndef R_O_M_H
#define R_O_M_H

typedef struct _LIRenOverlayManager LIRenOverlayManager;
struct _LIRenOverlayManager 
{

	LIAlgU32dic* 				overlays;
	//Ogre::OverlayManager* overlay_manager;
	LIRenContainerFactory* 		container_factory;
	LIRenImageOverlayFactory* 	image_factory;
	LIRenScaledOverlayFactory* 	scaled_factory;
	LIRenTextOverlayFactory* 	text_factory;
/*
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
	Ogre::OverlaySystem* overlay_system;
#endif
*/

};

#endif
