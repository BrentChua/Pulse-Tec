/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	EventManager.h
*
*	Description -	Event manager using the listener and dispatcher design pattern
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	03/07/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_EVENT_SYSTEM_H_
#define _PSX_EVENT_SYSTEM_H_

#include "PulseSTD.h"
#include "SmartPointer.h"
#include "StringID.h"
#include "Queue.h"
#include "List.h"
#include "Map.h"

namespace Pulse
{

	typedef StringID EventType;

	class Event
	{
	public:

		typedef SmartPointer< Event > EventPtr;

		explicit Event( FLOAT timeStamp = 0.0f );
		virtual ~Event( void ) { }

		virtual const EventType & GetEventType( void ) const =0;
		const FLOAT GetTimeStamp( void ) const;

		// TODO : Add serialization

	protected:

		const FLOAT m_timeStamp;
	};

	PSX_INLINE Event::Event( FLOAT timeStamp )
		: m_timeStamp( timeStamp )
	{

	}

	PSX_INLINE const FLOAT Event::GetTimeStamp( void ) const
	{
		return m_timeStamp;
	}

	class EventListener
	{
	public:

		typedef SmartPointer< EventListener > EventListenerPtr;

		explicit EventListener( void );
		virtual ~EventListener( void );

		virtual const CHAR * GetName( void ) = 0;

		virtual void HandleEvent( const Event &event ) = 0;

		BOOL operator == ( const EventListener &rhs ) const;

	};

	PSX_INLINE EventListener::EventListener( void )
	{

	}

	PSX_INLINE EventListener::~EventListener( void )
	{

	}

	PSX_FORCE_INLINE BOOL EventListener::operator == ( const EventListener &rhs ) const
	{
		return this == &rhs;
	}

	class EventManager
	{
	public:

		typedef Event::EventPtr EventPtr;
		typedef EventListener::EventListenerPtr EventListenerPtr;

		enum CONSTANTS { INFINITY = 0xFFFFFFFF };

		explicit EventManager( void );
		~EventManager( void );

		BOOL AddListener( const EventType &eventType, const EventListenerPtr &listener );
		BOOL RemoveListener( const EventType &eventType, const EventListenerPtr &listener );

		// NOTE: Execute event immediately
		void TriggerEvent( const Event &event );

		// NOTE: Queue event object
		void PostEvent( const EventPtr &event );

		void ProcessEvents( FLOAT maxTime = (FLOAT)INFINITY );

	private:

		enum PRIVATE_CONSTANTS { NUM_QUEUES = 2 };

		typedef ULONG									EventTypeID;
		typedef Queue< EventPtr >						EventQueue;
		typedef List< EventListenerPtr >				ListenerList;
		typedef PSX_Pair< EventTypeID, ListenerList >	TypeEventPair;
		typedef Map< EventTypeID, ListenerList >		ListenerMap;

		ListenerMap m_listeners;

		// We employ a double-buffered event queue
		EventQueue	m_eventQueues[NUM_QUEUES];
		INT			m_activeQueue;

	};

	PSX_INLINE EventManager::EventManager( void )
		: m_activeQueue(0)
	{

	}

	PSX_INLINE EventManager::~EventManager( void )
	{

	}

	PSX_INLINE void EventManager::PostEvent( const EventPtr &event )
	{
		m_eventQueues[m_activeQueue].Enqueue( event );
	}

}

#endif /* _PSX_EVENT_SYSTEM_H_ */