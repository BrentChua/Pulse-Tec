/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Bindable.h
*
*	Description -	Interface for Bind Unbind functionality.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/28/2010	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_BINDABLE_H_
#define _PSX_BINDABLE_H_

namespace Pulse
{
	class Bindable
	{
	public:

		Bindable( void ) { }

		virtual ~Bindable( void ) { }

		virtual void Bind( void ) { }

		virtual void Unbind( void ) { }

	};

}

#endif /* _PSX_BINDABLE_H_ */