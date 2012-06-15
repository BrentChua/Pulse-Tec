/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	View.h
*
*	Description -	Various view classes.
*
*	Comments	-	This class communicates with the application logic to present the 
*						data to the observer.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/16/2011	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_VIEW_H_
#define _PSX_VIEW_H_

#include "PulseSTD.h"
#include "Optional.h"
#include "AppMsg.h"

namespace Pulse
{

	PSX_EnumBegin( ApplicationViewType )
		HUMAN,
		AI,
		RECORDER,
	PSX_EnumEnd()

	// Application View base class
	typedef UINT GameViewID;
	typedef UINT ProgramID; // TODO: Move this to the program header file

	class View
	{
	public:

		View( void ) { }
		virtual ~View( void ) { }

		//virtual EErrorCode::Type	OnRestore( void ) = 0;
		virtual EErrorCode::Type	OnRender( DOUBLE time, FLOAT deltaTime ) = 0;
		//virtual void				OnLostDevice( void ) = 0;
		virtual void				OnUpdate( DOUBLE time, FLOAT deltaTime ) = 0;
		virtual BOOL				OnMessageProc( AppMsg *pMsg ) = 0;

		virtual ApplicationViewType::Type	GetType( void ) const = 0;
		virtual GameViewID					GetID( void ) const = 0;
		virtual void						OnAttach( GameViewID viewID, Optional<ProgramID> programID ) = 0;

	protected:

	private:

		friend Logic;
		virtual EErrorCode::Type	Render( DOUBLE time, FLOAT deltaTime ) = 0;
		virtual void				Update( DOUBLE time, FLOAT deltaTime ) = 0;

	};

	class HumanView : public View
	{
	public:

		HumanView( void );
		virtual ~HumanView( void );

		//virtual EErrorCode::Type	OnRestore( void );
		virtual EErrorCode::Type	OnRender( DOUBLE time, FLOAT deltaTime );
		//virtual void				OnLostDevice( void );
		virtual void				OnUpdate( DOUBLE time, FLOAT deltaTime );
		virtual BOOL				OnMessageProc( AppMsg *pMsg );

		virtual ApplicationViewType::Type	GetType( void ) const;
		virtual GameViewID					GetID( void ) const;
		virtual void						OnAttach( GameViewID viewID, Optional<ProgramID> programID );

		//void AddScreen( Screen *pScreen );
		//void RemoveScreen( Screen *pScreen );
		
		void SetFullSpeedRendering( BOOL bEnable ) { m_bFullSpeedRendering = bEnable; }

	private:

		void Update( DOUBLE time, FLOAT deltaTime );
		virtual EErrorCode::Type	Render( DOUBLE time, FLOAT deltaTime );

	private:

		BOOL m_bFullSpeedRendering;
		FLOAT m_lastTime;

	};


}

#endif /* _PSX_VIEW_H_ */