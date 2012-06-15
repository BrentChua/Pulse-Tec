/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Assert.h
*
*	Description -	Assertion wrapper.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/13/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_ASSERT_H_
#define _PSX_ASSERT_H_

#include "Platform.h"

#include <assert.h>
#include <stdio.h>

namespace Pulse
{

	// Compile-time static assert.
	template < BOOL bCondition, typename assertMsg > struct StaticAssertFailure;
	template < typename assertMsg > struct StaticAssertFailure< TRUE, assertMsg > { enum { VAL = 1 }; };
	template < SIZE_T unused > struct StaticAssertTest{};

#define StaticAssert( condition, msg )	\
	class Assert_##msg;	\
		typedef StaticAssertTest<sizeof(StaticAssertFailure<(BOOL)(condition), msg>)>	\
		StaticAssertTestTypedef##__COUNTER__

	// Run-time assert.
#ifdef PSX_DEBUG
	// NOTE: Replace old PSX_Assert with PSX_Assert( condition, ... ) signature.
	//#define PSX_Assert( condition, ... )
#endif /* PSX_DEBUG */

}

#endif /* _PSX_ASSERT_H_ */