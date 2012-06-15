/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GlobalDirectory.h
*
*	Description -	Contains all the general directory paths for specific resources.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/14/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GLOBAL_DIRECTORY_H_
#define _PSX_GLOBAL_DIRECTORY_H_

namespace Pulse
{
	#define PSX_PATH_RESOURCES PSX_String("Resources\\")
	
	// Effects
	#define PSX_PATH_EFFECTS PSX_String("Resources\\Effects\\")
	#define PSX_PATH_INPUT_LAYOUTS PSX_String("Resources\\Effects\\InputLayouts\\")
	#define PSX_PATH_SHADERS PSX_String("Resources\\Effects\\Shaders")

	// 
}

#endif /* _PSX_GLOBAL_DIRECTORY_H_ */