/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Registry.h
*
*	Description -	Global registry. Manages registered objects.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/23/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_REGISTRY_H_
#define _PSX_REGISTRY_H_

#include "Platform.h"
#include "NonCopyable.h"
#include "Log.h"

namespace Pulse
{
	struct ERegistryPriority
	{
		enum Type
		{		
			Lowest,
			VeryLow,
			BelowNormal,
			Normal,
			AboveNormal,
			High,
			VeryHigh,
			Highest,
			Reserved,
		};
	};

	struct ERegistryOperation
	{
		enum Type
		{
			INITIALIZE,
			SHUTDOWN,
			TEST_INITIALIZE,
			TEST,			// General Test unit
			TEST_SHUTDOWN,
			EnumCount,
		};
	};

	class IRegistryObject : private NonCopyable
	{
	public:

		// Initialize object priority
		IRegistryObject( ERegistryPriority::Type primaryPriority, ERegistryPriority::Type secondaryPriority, BOOL bRegisterObject = TRUE );

		// Destructor
		virtual ~IRegistryObject ( void );

	protected:

		friend Registry;

		// Registry object operation handling 
		virtual EErrorCode::Type DoRegistryOperation( ERegistryOperation::Type op ) = 0;

		// Less than registry object comparison
		static BOOL IsLessPriority( const IRegistryObject *plhs, const IRegistryObject *prhs );

	private:

		ERegistryPriority::Type m_primaryPriority;
		ERegistryPriority::Type m_secondaryPriority;

	};

	class Registry
	{
	public:

		static EErrorCode::Type ExecuteOperation( ERegistryOperation::Type op );

	private:

		static void RegisterObject( IRegistryObject *pObj );

		static void UnregisterObject( IRegistryObject *pObj );

	private:

		friend IRegistryObject;
		friend RegisterClassRAII;
		friend class TestRAII;

		// Custom compare trait for sorting registry objects based on their priority
		template < typename T >
		class RegistryObjectLessThan
		{
		public:
			BOOL operator () ( const T &lhs, const T &rhs ) const
			{
				return IRegistryObject::IsLessPriority( lhs, rhs );
			}
		};

		typedef SortedLList< IRegistryObject *, RegistryObjectLessThan<IRegistryObject *> > ObjectList;

		static ObjectList *m_pObjectList;

	};

	#define PSX_REGISTRY_OBJECT_START( _class_, primaryPriority, secondaryPriority )	\
		class RegistryObject##_class_ : public IRegistryObject	\
		{	\
		public:	\
			RegistryObject##_class_##( void ) : IRegistryObject( primaryPriority, secondaryPriority ) { } \
			virtual EErrorCode::Type DoRegistryOperation( ERegistryOperation::Type op );	\
		};	\
		static RegistryObject##_class_ gRegistryObject##_class_##;	\
		\
		EErrorCode::Type RegistryObject##_class_##::DoRegistryOperation( ERegistryOperation::Type op )	\
		{	\
			switch( op ) \
			{

	#define PSX_REGISTRY_OBJECT_END( )	\
			default:	\
			break; \
			}	\
			return EErrorCode::NONE;	\
		}

	// NOTE: These PSX_REGISTRY_ON... macros are used inside the START and END REGISTRY_OBECT... macros above.
	//	They define the starting code of the events. The NO_BREAK version of these macros doesn't append break,
	//		which allows a previous case to fall through.
	#define PSX_REGISTRY_ON_INITIALIZE()				break; case ERegistryOperation::INITIALIZE:
	#define PSX_REGISTRY_ON_INITIALIZE_NO_BREAK()		case ERegistryOperation::INITIALIZE:
	#define PSX_REGISTRY_ON_SHUTDOWN()					break; case ERegistryOperation::SHUTDOWN:
	#define PSX_REGISTRY_ON_SHUTDOWN_NO_BREAK()			case ERegistryOperation::SHUTDOWN:
	#define PSX_REGISTRY_ON_TEST_INITIALIZE()			break; case ERegistryOperation::TEST_INITIALIZE:
	#define PSX_REGISTRY_ON_TEST_INITIALIZE_NO_BREAK()	case ERegistryOperation::TEST_INITIALIZE:
	#define PSX_REGISTRY_ON_TEST()						break; case ERegistryOperation::TEST:
	#define PSX_REGISTRY_ON_TEST_NO_BREAK()				case ERegistryOperation::TEST:
	#define PSX_REGISTRY_ON_TEST_SHUTDOWN()				break; case ERegistryOperation::TEST_SHUTDOWN:
	#define PSX_REGISTRY_ON_TEST_SHUTDOWN_NO_BREAK()	case ERegistryOperation::TEST_SHUTDOWN:

}

#endif /* _PSX_REGISTRY_H_ */