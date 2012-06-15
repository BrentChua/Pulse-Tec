/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SmartPointer.h
*
*	Description -	SmartPointer is a template class that provides automatic 
*						garbage collection for the heap allocated data.
*
*	Comments	-	NEVER EVER pass a dynamically allocated object that's managed by 
*						a SmartPointer to another SmartPointer.
*					Ex. TestClass *pObj = new TestClass;
*						SmartPointer< TestClass > smart1(pObj);
*						SmartPointer< TestClass > smart2(pObj); // Will crash!
*
*				-	Never let two SmartPointers point to each other.
*					TODO: Evaluate the need of SmartPointerArr. And if the methods
*						really needs to be virtuals...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/10/2009	-	File creation and base implementation.
*			MrCodeSushi	-	10/11/2009	-	Array support ( SmartPointerArr<T> ).
*			MrCodeSushi	-	05/22/2010	-	Now using AtomicInt for m_refCount(Multi-Threading).
**************************************************************************************/

#ifndef _PSX_SMARTPOINTER_H_
#define _PSX_SMARTPOINTER_H_

#include "PulseSTD.h"
//#include "IRefCounter.h"
#include "AtomicInt.h"

namespace Pulse
{

	template < typename T >
	class SmartPointer
	{
	public:

		typedef T * PointerType;

		SmartPointer( void );
		SmartPointer( const SmartPointer< T > &smartPointer );
		SmartPointer( const T *pData );
		virtual ~SmartPointer();

		const T & operator * ( void ) const;
		T & operator * ( void );
		T * operator -> ( void ) const;

		operator T * ( void ) const;
		operator T & ( void ) const;

		BOOL operator == ( const SmartPointer< T > &rhs ) const;

		SmartPointer & operator = ( const SmartPointer< T > &rhs );
		virtual SmartPointer & operator = ( const T *pData );

		void Reset( const T *pData );
		void Reset( void );

		const SIZE_T GetRefCount( void );

		const BOOL IsNull( void ) const;

	protected:

		void Release();
		void AddRef();

		//class ISmartData : public IRefCounter
		//{
		//public:
		//	virtual void AddRef( void ) = 0;
		//	virtual void Release( void ) = 0;

		//	virtual T * GetData( void ) = 0;
		//	virtual const T * GetData() const = 0;
		//	virtual void SetData( T *pData ) = 0;

		//	//virtual ~ISmartData( void ) { }; // Eveything should be handled by Release()...
		//};

		class SmartData /*: public ISmartData*/
		{
		public:

			SmartData();
			SmartData( const T* pData );

			/*virtual*/ void AddRef( void );
			/*virtual*/ void Release( void );

			/*virtual*/ T * GetData( void );
			/*virtual*/ const T * GetData( void ) const;
			/*virtual*/ void SetData( T *pData );

		protected:

			friend SmartPointer;

			T			*m_pData;
			AtomicInt	m_refCount;
		};

		SmartData *m_pSmartObj;
	};

	// SmartPointer for handling arrays
	template < typename T >
	class SmartPointerArr : public SmartPointer< T >
	{
	public:

		SmartPointerArr( void );
		SmartPointerArr( SmartPointerArr< T > &smartPointerArr );
		explicit SmartPointerArr( T *pData );
		virtual ~SmartPointerArr();

		virtual SmartPointerArr & operator = ( const T *pData );
		SmartPointerArr & operator = ( const SmartPointerArr< T > &rhs );

	private:

		class SmartDataArr : public SmartData
		{
		public:
			SmartDataArr( void );
			SmartDataArr( const T* pData );

			virtual void Release( void );
		};
	};
}

#include "SmartPointer.inl"

#endif _PSX_SMARTPOINTER_H_
