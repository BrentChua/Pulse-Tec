/**************************************************************************************
*
*	Copyright (C) 2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ScreenElement.h
*
*	Description -	Base interface class for all renderable classes.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	07/05/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDERABLE_H_
#define _PSX_RENDERABLE_H_

#include "PulseSTD.h"
#include "Vector3.h"
#include "List.h"

namespace Pulse
{

	class ScreenElement
	{
	public:

		ScreenElement( void );

		virtual ~ScreenElement( void ) { }

		Vector3 * GetPosition( void ) { return &m_pos; }
		
		Vector3 GetGlobalPosition( void );

		ScreenElement * GetParent( void ) { return m_pParent; }

		void SetParent( ScreenElement *pParent ) { if ( pParent != this ) m_pParent = pParent; }
		
		void RemoveParent( void ) { SetParent( PSX_NULL ); }

		void Attach( ScreenElement *pParent );

		void RemoveAttachment( void ) { RemoveParent(); }

		void SetPosition( Vector3 *pos ) { m_pos = *pos; }

		void SetPosition( FLOAT x, FLOAT y, FLOAT z ) { SetPosition( &Vector3( x, y, z ) );	}

		FLOAT GetYaw( void );
		void SetYaw( FLOAT deg );

		FLOAT GetPitch( void );
		void SetPitch( FLOAT deg );

		FLOAT GetRoll( void );
		void SetRoll( FLOAT deg );

		Vector3 * GetRotation( void );

		Vector3 GetGlobalRotation( void );
		
		FLOAT GetScale( void ) { return m_scale; }

		void SetScale( FLOAT scale ) { m_scale = scale; }

		Material * GetMaterial( void ) { return m_pMaterial; }

		void SetMaterial( Material *pMat ) { m_pMaterial = pMat; }

		virtual void Render( void );

		//virtual ERenderType::Type GetRenderType( void ) = 0;

	protected:

		void AddChild( ScreenElement *pObj );

		void ScreenElement::RemoveChild( ScreenElement *pObj );

		void FixZOrdering( void );

		friend RenderStage;

		// Used by the renderer
		virtual void Bind( void ) = 0;

		// Used by the renderer
		virtual void Unbind( void ) = 0;

	protected:

		typedef List<ScreenElement*> ChildList;

		ScreenElement	*m_pParent;
		ChildList	m_children;

		Vector3 m_pos;
		Vector3 m_rotation;
		FLOAT	m_scale;
		Material *m_pMaterial;

	};

	PSX_INLINE FLOAT ScreenElement::GetYaw( void ) 
	{ 
		return Math::RadToDeg(m_rotation.y);
	}

	PSX_INLINE void ScreenElement::SetYaw( FLOAT deg )
	{
		m_rotation.y = Math::DegToRad( deg );
	}

	PSX_INLINE FLOAT ScreenElement:: GetPitch( void ) 
	{
		return Math::RadToDeg(m_rotation.x);
	}
	
	PSX_INLINE void ScreenElement::SetPitch( FLOAT deg )
	{
		m_rotation.x = Math::DegToRad( deg );
	}

	PSX_INLINE FLOAT ScreenElement::GetRoll( void )
	{
		return Math::RadToDeg(m_rotation.z);
	}

	PSX_INLINE void ScreenElement::SetRoll( FLOAT deg )
	{
		m_rotation.z = Math::DegToRad( deg );
	}

	PSX_INLINE Vector3 * ScreenElement::GetRotation( void )
	{
		return &m_rotation;
	}
}

#endif /* _PSX_RENDERABLE_H_ */