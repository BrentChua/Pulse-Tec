/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SmartPointer.inl
*
*	Comments	-	See SmartPointer.h
*
**************************************************************************************/

namespace Pulse
{

	/* SmartData Implementation */

	template < typename T >
	PSX_FORCE_INLINE SmartPointer< T >::SmartData::SmartData() 
		: m_pData( 0 ), m_refCount( 0 ) 
	{
	
	}

	template < typename T >
	PSX_FORCE_INLINE SmartPointer< T >::SmartData::SmartData( const T *pData ) 
		: m_pData( (T*)pData ), m_refCount( 1 ) 
	{

	}

	template < typename T >
	PSX_FORCE_INLINE void SmartPointer< T >::SmartData::AddRef()
	{
		++m_refCount;
	}

	template < typename T >
	void SmartPointer< T >::SmartData::Release()
	{
		if ( --m_refCount == 0 )
		{
			delete ((T*)m_pData);
			delete this;
		}
	}

	template < typename T >
	PSX_FORCE_INLINE T * SmartPointer< T >::SmartData::GetData()
	{
		return m_pData;
	}

	template < typename T >
	PSX_FORCE_INLINE const T * SmartPointer< T >::SmartData::GetData() const
	{
		return m_pData;
	}

	template < typename T >
	PSX_FORCE_INLINE void SmartPointer< T >::SmartData::SetData( T *pData )
	{
		m_pData = pData;
	}


	/* SmartPointer Implementation */

	template < typename T >
	PSX_FORCE_INLINE SmartPointer< T >::SmartPointer( void ) 
		: m_pSmartObj(0) 
	{
	
	}

	template < typename T >
	SmartPointer< T >::SmartPointer( const SmartPointer< T > &smartPointer )
	{
		m_pSmartObj = smartPointer.m_pSmartObj;
		if ( smartPointer.IsNull() == FALSE )
			AddRef();
	}

	template < typename T >
	PSX_FORCE_INLINE SmartPointer< T >::SmartPointer( const T *pData )
	{
		// Only create smart data if pData actually points to something
		//	other than NULL.
		if ( pData )
			m_pSmartObj = new SmartData( pData ); // auto sets ref to one
		else
			m_pSmartObj = NULL;
	}

	template < typename T >
	PSX_FORCE_INLINE SmartPointer< T >::~SmartPointer()
	{
		Release();
	}

	template < typename T >
	void SmartPointer< T >::Release()
	{
		if ( m_pSmartObj )
		{
			m_pSmartObj->Release(); 
			m_pSmartObj = NULL;
		}
	}

	template < typename T >
	PSX_INLINE void SmartPointer< T >::AddRef()
	{
		if ( m_pSmartObj )
			m_pSmartObj->AddRef();
	}
	
	template < typename T >
	PSX_INLINE const T & SmartPointer< T >::operator * ( void ) const
	{
		return *m_pSmartObj->GetData();
	}

	template < typename T >
	PSX_INLINE T & SmartPointer< T >::operator * ( void )
	{
		return *m_pSmartObj->GetData();
	}

	template < typename T >
	PSX_INLINE T * SmartPointer< T >::operator -> ( void ) const
	{
		return m_pSmartObj->GetData();
	}

	template < typename T >
	PSX_INLINE SmartPointer< T >::operator T * ( void ) const
	{
		return m_pSmartObj ? m_pSmartObj->GetData() : PSX_NULL;
	}

	template < typename T >
	PSX_INLINE SmartPointer< T >::operator T & ( void ) const
	{
		return *m_pSmartObj->GetData();
	}

	template < typename T >
	PSX_INLINE BOOL SmartPointer< T >::operator == ( const SmartPointer< T > &rhs ) const
	{
		return m_pSmartObj == rhs.m_pSmartObj;
	}

	template < typename T >
	SmartPointer< T > & SmartPointer< T >::operator = ( const SmartPointer< T > &rhs )
	{
		if ( this == &rhs ) return *this;

		Release();
		m_pSmartObj = rhs.m_pSmartObj;
		AddRef();
		return *this;
	}

	template < typename T >
	SmartPointer< T > & SmartPointer< T >::operator = ( const T *pData )
	{
		if ( m_pSmartObj && m_pSmartObj->GetData() == pData ) return *this;

		Release();
		m_pSmartObj = new SmartData( pData ); // ref autmatically set to 1
		return *this;
	}

	template < typename T >
	PSX_FORCE_INLINE void SmartPointer< T >::Reset(  const T *pData )
	{
		*this = pData;
	}

	template < typename T >
	PSX_FORCE_INLINE void SmartPointer< T >::Reset( void )
	{
		Release();
	}

	template < typename T >
	PSX_FORCE_INLINE const SIZE_T SmartPointer< T >::GetRefCount( void )
	{
		return m_pSmartObj->m_refCount;
	}

	template < typename T >
	PSX_INLINE const BOOL SmartPointer< T >::IsNull( void ) const
	{
		return m_pSmartObj == NULL;
	}

	/* SmartDataArr Implementation */

	template < typename T >
	PSX_FORCE_INLINE SmartPointerArr< T >::SmartDataArr::SmartDataArr( void ) : SmartData() { }

	template < typename T >
	PSX_FORCE_INLINE SmartPointerArr< T >::SmartDataArr::SmartDataArr( const T *pData ) : SmartData( pData ) { }

	template < typename T >
	void SmartPointerArr< T >::SmartDataArr::Release()
	{
		if ( m_pData && --m_refCount == 0)
		{
			delete [] m_pData;
			delete this;
		}
	}

	/* SmartPointerArr Implementation */

	template < typename T >
	PSX_FORCE_INLINE SmartPointerArr< T >::SmartPointerArr( void ) : SmartPointer() { }

	template < typename T >
	SmartPointerArr< T >::SmartPointerArr( SmartPointerArr< T > &smartPointerArr ) : SmartPointer( smartPointerArr )
	{
		Release();
		m_pSmartObj = smartPointerArr;
		AddRef();
	}

	template < typename T >
	SmartPointerArr< T >::SmartPointerArr( T *pData ) : SmartPointer()
	{
		Release();
		m_pSmartObj = new SmartDataArr( pData );
	}

	template < typename T >
	PSX_FORCE_INLINE SmartPointerArr< T >::~SmartPointerArr()
	{
		Release();
	}

	template < typename T >
	SmartPointerArr< T > & SmartPointerArr< T >::operator = ( const T *pData )
	{
		if ( m_pSmartObj && m_pSmartObj->GetData() == pData ) return *this;

		Release();
		m_pSmartObj = new SmartDataArr( pData );
		
		return *this;
	}

	template < typename T >
	SmartPointerArr< T > & SmartPointerArr< T >::operator = ( const SmartPointerArr< T > &rhs )
	{
		if ( &rhs == this ) return *this;

		Release();
		m_pSmartObj = rhs.m_pSmartObj;
		AddRef();

		return *this;
	}

	
}