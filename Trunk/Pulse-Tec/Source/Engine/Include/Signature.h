/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Signature.h
*
*	Description -	Class for holding signatures (GUID).
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/06/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_SIGNATURE_H_
#define _PSX_SIGNATURE_H_

#include "PulseSTD.h"

namespace Pulse
{
	struct Signature
	{
		//Signature( void ) 
		//{ 
		//	PSX_ZeroMem( this, sizeof( Signature ) ); 
		//}

		BOOL operator == ( const Signature &sig ) const
		{
			return ( memcmp( this, &sig, sizeof( Signature ) ) == 0 );
		}

		Pulse::DWORD	l;
		Pulse::WORD	w1, w2;
		Pulse::BYTE	b1, b2, b3, b4, b5, b6, b7, b8;
	};

	// Macro used to declare a signature if it needs to be in a class or struct.
	#define PSX_DECLARE_SIGNATURE( _name )	\
		const static Signature _name;

	// Use this to define signatures declared inside classes.
	// This can also be used w/o DECLARE_SIGNATURE if Signature doesn't exist in a class or struct.
	#define PSX_DEFINE_SIGNATURE( _name, _l, _w1, _w2, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8 )		\
		const Signature _name = { _l , _w1 , _w2 , _b1 , _b2 , _b3 , _b4 , _b5 , _b6 , _b7 , _b8 };

}

#endif /* _PSX_SIGNATURE_H_ */