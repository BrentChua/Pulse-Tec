/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	NonCopyable.h
*
*	Description -	Derived classes can derive from this class and make the derived 
*					class not copyable.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/28/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_NON_COPYABLE_H_
#define _PSX_NON_COPYABLE_H_

namespace Pulse
{
	class NonCopyable
	{
	protected:

		NonCopyable( void ) { }
		~NonCopyable( void ) { }

	private:

		NonCopyable( const NonCopyable & );
		const NonCopyable & operator = ( const NonCopyable & obj );

	};
}

#endif /* _PSX_NON_COPYABLE_H_ */