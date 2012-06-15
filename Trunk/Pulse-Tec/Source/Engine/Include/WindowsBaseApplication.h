/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseWinBase.h
*
*	Description -	This will contain Microsoft specific stuff.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/26/2009	-	Creation of this file
**************************************************************************************/

#ifndef _WINDOWS_BASE_APPLICATION_H_
#define _WINDOWS_BASE_APPLICATION_H_

#include "PulseSTD.h"
#include "IApplication.h"

namespace Pulse
{

	class WindowsBaseApplication : public IApplication
	{
	public:

		/********************************************************************************
		*	Function Name :	GetApplicationDataDirectory
		*	Description	  :	Returns the absolute path directory for the application to 
		*						save data.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	CHAR * - C-Style string containing the absolute path.
		*							  Returns NULL if invalid.
		*	Example		  : 
		********************************************************************************/
		virtual const CHAR * GetApplicationDataDirectory( const CHAR *appDirectory );
	
	};
};

#endif /* _WINDOWS_BASE_APPLICATION_H_ */