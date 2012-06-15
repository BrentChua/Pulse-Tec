/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- IResourcePoolItem.h
*
*	Description - The base resource item class used by the resource pool.
*
*	Comments	- Also contains ResourceRegisterCode class that determins the type of resrouce 
*					item.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/10/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_RESOURCE_POOL_ITEM_H_
#define _PSX_RESOURCE_POOL_ITEM_H_

#include "PulseSTD.h"
#include "IRefCounter.h"
#include "StoragePool.h"
#include "String.h"
#include "BitFlags.h"

namespace Pulse
{

#define PSX_MakeStoragePoolFriend(className) 	friend class ResourcePool<className>;	\
												friend class Storage<className>;

	class IResourcePool;

	class ResourceRegisterCode
	{
	public:

		enum {
			INVALID_CODE = 0xFFFFFFFF,
		};

		union
		{
			struct
			{
				WORD type;
				WORD subType;
			};
			DWORD value;
		};

		ResourceRegisterCode( void ) : value(INVALID_CODE) { }
		ResourceRegisterCode( DWORD _type ) : value( _type ) { }
		ResourceRegisterCode( WORD _type, WORD _subType ) : type(_type), subType(_subType) { }

		void SetInvalid( void ) { value = INVALID_RESOURCE_HANDLE; }

		const BOOL IsValid( void ) { return value != INVALID_RESOURCE_HANDLE; }

		const BOOL operator < ( const ResourceRegisterCode &rhs ) const { return value < rhs.value; }
		const BOOL operator > ( const ResourceRegisterCode &rhs ) const { return value > rhs.value; }
		const BOOL operator == ( const ResourceRegisterCode &rhs ) const { return value == rhs.value; }
	};

	PSX_EnumBegin( EResDescType )
		UNKNOWN,
		VIEWPORT,
		INPUT_LAYOUT,
		BLEND_STATE,
		RASTERIZER_STATE,
		SAMPLER_STATE,
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		SWAP_CHAIN,
		VERTEX_DECLARATION,
		VERTEX_BUFFER,
		INDEX_BUFFER,
		CONSTANT_BUFFER,
		SHADER,
		MATERIAL,
		SHADER_TECHNIQUE,
		FONT,
	PSX_EnumEnd()

	struct BaseResDesc
	{
		virtual ~BaseResDesc( void ) { }

		virtual EResDescType::Type GetDescType( void ) = 0 
		{ 
			return EResDescType::UNKNOWN; 
		}
	};

	// NOTE: Some Guidelines on the Create, Destroy, Disable, Restore Resources.
	//	Create and Destroy are now the constructor and destructors. 
	//	Minimize code in your constructor and place them in CreateResource.
	//	Minimize code in your destructor and place them in DestroyResource.
	//	Graphics resources sometimes gets broken. Call this to release important resources.
	//	After calling DisableResource call RestoreResource to reload the resource.

	class IResourcePoolItem : public RefCounter
	{
	public:

		enum RESOURCE_FLAG
		{
			RESOURCE_FLAG_CREATED = 0,
			RESOURCE_FLAG_LOADED,
			RESOURCE_FLAG_DISABLED,
			RESOURCE_FLAG_ALTERED
		};

		IResourcePoolItem( void );			// Called on creation.
		virtual ~IResourcePoolItem( void );	// Called on destruction

		// These defines the base interface for all derived resources
		//virtual BOOL CreateResource( BaseResDesc *pDesc ) = 0;
		virtual BOOL CreateResource( BaseResDesc *pDesc )	{ SetFlagCreated();		return TRUE;	}	// Your new Constructor
		virtual BOOL DestroyResource( void )				{ SetFlagDestroyed();	return TRUE;	}	// Your new destructor
		virtual BOOL DisableResource( void )				{ SetFlagDisabled();	return TRUE;	}	// Disables/Releases resources
		virtual BOOL RestoreResource( void )				{ SetFlagRestored();	return TRUE;	}	// Reloads your resourcess

		ResourceRegisterCode	GetResourceType( void ) const;
		IResourcePool *			GetResourcePool( void ) const;
		HRes					GetResourceHandle( void ) const;
		BitFlagsU32				GetResourceFlags( void ) const;

		BOOL IsCreated( void ) const;
		BOOL IsDisabled( void ) const;
		BOOL IsLoaded( void ) const;
		const String * GetResourceName( void ) const;
		
		void SetResourceName( const CHAR *pName );
		void SetFlagAltered( bool bAltered );
		BOOL GetFlagAltered( void );

		virtual INT Release( void );

	protected:

		void SetResourceCode( ResourceRegisterCode code );
		void SetResourcePool( IResourcePool *pResPool );
		void SetResourceHandle( HRes hRes );
		void SetResourceFlag( DWORD flagBit, BOOL bOn );

		void SetFlagCreated( void );
		void SetFlagDisabled( void );
		void SetFlagLoaded( void );
		void SetFlagUnloaded( void );
		void SetFlagRestored( void );
		void SetFlagDestroyed( void );
		void SetFlagSaved( void );

	private:

		template < typename T >
		friend class ResourcePool;

		IResourcePoolItem( const IResourcePoolItem & );
		IResourcePoolItem & operator = ( const IResourcePoolItem & rhs );

		ResourceRegisterCode	m_resCode;
		IResourcePool			*m_pResPool;
		HRes					m_hRes;
		BitFlagsU32				m_resFlags;

	};

	PSX_INLINE IResourcePoolItem::IResourcePoolItem( void )
		: m_resCode(0), m_pResPool(NULL), m_hRes(0), m_resFlags(0)
	{

	}

	PSX_INLINE IResourcePoolItem::~IResourcePoolItem( void )
	{
		PSX_Assert( !IsCreated(), "Failed to destroy resource." );
	}

	PSX_INLINE ResourceRegisterCode IResourcePoolItem::GetResourceType( void ) const
	{
		return m_resCode;
	}

	PSX_INLINE IResourcePool *	IResourcePoolItem::GetResourcePool( void ) const
	{
		return m_pResPool;
	}

	PSX_INLINE HRes IResourcePoolItem::GetResourceHandle( void ) const
	{
		return m_hRes;
	}

	PSX_INLINE BitFlagsU32 IResourcePoolItem::GetResourceFlags( void ) const
	{
		return m_resFlags;
	}

	PSX_INLINE BOOL IResourcePoolItem::IsCreated( void ) const
	{
		return m_resFlags.TestBit( RESOURCE_FLAG_CREATED );
	}

	PSX_INLINE BOOL IResourcePoolItem::IsDisabled( void ) const
	{
		return m_resFlags.TestBit( RESOURCE_FLAG_DISABLED );
	}

	PSX_INLINE BOOL IResourcePoolItem::IsLoaded( void ) const
	{
		return m_resFlags.TestBit( RESOURCE_FLAG_LOADED );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagAltered( bool bAltered )
	{
		m_resFlags.SetBit( RESOURCE_FLAG_ALTERED, bAltered );
	}

	PSX_INLINE BOOL IResourcePoolItem::GetFlagAltered( void )
	{
		return m_resFlags.TestBit( RESOURCE_FLAG_ALTERED );
	}

	PSX_INLINE void IResourcePoolItem::SetResourceCode( ResourceRegisterCode code )
	{
		m_resCode = code;
	}

	PSX_INLINE void IResourcePoolItem::SetResourcePool( IResourcePool *pResPool )
	{
		m_pResPool = pResPool;
	}

	PSX_INLINE void IResourcePoolItem::SetResourceHandle( HRes hRes )
	{
		m_hRes = hRes;
	}

	PSX_INLINE void IResourcePoolItem::SetResourceFlag( DWORD flagBit, BOOL bOn )
	{
		m_resFlags.SetBit( flagBit, bOn );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagCreated( void )
	{
		SetResourceFlag( RESOURCE_FLAG_CREATED, TRUE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagDisabled( void )
	{
		SetResourceFlag( RESOURCE_FLAG_DISABLED, TRUE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagLoaded( void )
	{
		SetFlagAltered( FALSE );
		SetResourceFlag( RESOURCE_FLAG_LOADED, TRUE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagUnloaded( void )
	{
		SetResourceFlag( RESOURCE_FLAG_LOADED, FALSE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagRestored( void )
	{
		SetResourceFlag( RESOURCE_FLAG_DISABLED, FALSE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagDestroyed( void )
	{
		SetResourceFlag( RESOURCE_FLAG_CREATED, FALSE );
	}

	PSX_INLINE void IResourcePoolItem::SetFlagSaved( void )
	{
		SetFlagAltered( FALSE );
	}

}

#endif /* _PSX_RESOURCE_POOL_ITEM_H_ */