/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	EventManager.cpp
*
*	Comments	-	See EventManager.h
*
**************************************************************************************/

#include "../Include/EventManager.h"
#include "../Include/System.h"

namespace Pulse
{
	BOOL EventManager::AddListener( const EventType &eventType, const EventListenerPtr &listener )
	{
		ListenerList *pListeners = &m_listeners[eventType.GetID()];

		ListenerList::Iterator iter = pListeners->IteratorBegin();
		ListenerList::Iterator iterEnd = pListeners->IteratorEnd();

		// Make sure listener only exist once in the list.
		// We don't want to double trigger the event on the same listener.
		while ( iter != iterEnd )
		{
			if ( **iter == *listener ) // Already in
				return FALSE;
			++iter;
		}

		pListeners->PushBack( listener );

		return TRUE;
	}

	BOOL EventManager::RemoveListener( const EventType &eventType, const EventListenerPtr &listener )
	{
		ListenerMap::Iterator findIter = m_listeners.Find( eventType.GetID() );

		if ( findIter == m_listeners.IteratorEnd() )
			return FALSE;

		ListenerList *pListeners = &(*findIter).second;

		pListeners->Remove( listener );

		return TRUE;
	}

	void EventManager::TriggerEvent( const Event &event )
	{
		ListenerMap::Iterator findIter = m_listeners.Find( event.GetEventType().GetID() );

		if ( findIter == m_listeners.IteratorEnd() )
			return;

		ListenerList *pListeners = &(*findIter).second;
		ListenerList::Iterator iter = pListeners->IteratorBegin();
		ListenerList::Iterator iterEnd = pListeners->IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->HandleEvent( event );
			++iter;
		}
	}

	void EventManager::ProcessEvents( FLOAT maxTime /*= (FLOAT)INFINITY*/ )
	{
		ULONG currTick = System::GetTickCount();
		ULONG maxMillis = INFINITY;

		// If using maxTime, we'll need to convert it to ticks.
		if ( maxTime != INFINITY )
			maxMillis = currTick + static_cast<ULONG>(1000 * maxTime);
		
		// Swap event queue
		EventQueue *pQueueToProcess = &m_eventQueues[m_activeQueue];
		m_activeQueue = ( m_activeQueue + 1 ) % NUM_QUEUES;

		EventPtr				eventToProcess;
		ListenerList			*pListeners;
		ListenerList::Iterator	listenerIter;
		ListenerList::Iterator	listenerIterEnd;
		ListenerMap::Iterator	findIter;

		// For each event
		while ( pQueueToProcess->IsEmpty() == FALSE )
		{
			eventToProcess = pQueueToProcess->Dequeue();

			findIter = m_listeners.Find( eventToProcess->GetEventType().GetID() );

			PSX_Assert( findIter != m_listeners.IteratorEnd(), "No registered listeners found." );

			if ( findIter != m_listeners.IteratorEnd() )
			{
				pListeners = &(*findIter).second;

				listenerIter = pListeners->IteratorBegin();
				listenerIterEnd = pListeners->IteratorEnd();

				// For-each registered listener of this event
				while ( listenerIter != listenerIterEnd )
				{
					(*listenerIter)->HandleEvent( *eventToProcess );
					++listenerIter;
				}
			}

			// Check time if we have passed the limit
			if ( maxMillis != INFINITY && System::GetTickCount() > maxMillis )
			{
				// Transfer the remaining events to the next queue to be processed next frame.
				while ( pQueueToProcess->IsEmpty() == FALSE )
					PostEvent( pQueueToProcess->Dequeue() );

				return;
			}

		}
	}

}