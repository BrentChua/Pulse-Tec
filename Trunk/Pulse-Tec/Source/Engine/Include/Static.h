/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Static.h
*
*	Description -	Trait class making derived classes non-copyable, 
*					non-constructable and static
*
*	Comments	-	Umm... I don't know what the hell this is... I forgot what I was doing...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/05/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_STATIC_H_
#define _PSX_STATIC_H_

namespace Pulse
{
	class Static
	{
	public:
		~Static( void );
	private:
		Static( void );
		Static( const Static & );
	};
}

#endif /* _PSX_STATIC_H_ */