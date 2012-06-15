/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PTL.h
*
*	Description -	(P)ulse (T)emplate (L)ibrary.
*
*	Comments	-	Contains a collection of very helpful basic templated 
*					classes and functions.
*
*					TODO: Implement/Integrate a compound type functionality.
*						-	Function Types
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/1/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_TEMPLATE_LIBRARY_H_
#define _PSX_TEMPLATE_LIBRARY_H_

//#include "Platform.h"
#include "PulseSTD.h"

namespace Pulse
{
	template < typename T >
	class Array;

	namespace Private
	{
		template < typename T >
		class GetBottomT
		{
		public:

			typedef T BottomType;
		};

		#define PSX_DECL_GETBTM_SPEC( _spec )								\
			template < typename T >											\
			class GetBottomT< T##_spec >									\
			{																\
			public:															\
																			\
				typedef typename GetBottomT< T >::BottomType BottomType;	\
			};																												

		PSX_DECL_GETBTM_SPEC( * )
		PSX_DECL_GETBTM_SPEC( & )
		PSX_DECL_GETBTM_SPEC( [] )

		template < typename T, SIZE_T N >							
		class GetBottomT< T[N] >									
		{															
		public:														

			typedef typename GetBottomT< T >::BottomType BottomType;	
		};	

		template < typename T, typename C >
		class GetBottomT< T C::* >
		{
		public:
			typedef typename GetBottomT< T >::BottomType BottomType;
		};

		#undef PSX_DECL_GETBTM_SPEC
	};

	/* Template helper classes for determining a data type if its fundamental or compound */
	template < typename T >
	class DataType
	{
	public:
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 0, Reference = 0, Array = 0, Function = 0, PointerToMember = 0 };

		typedef T BaseType;
		typedef T BottomType;
		typedef DataType< void > ClassType;
	};

	template < typename T >
	class DataType< T * >
	{
	public:
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 1, Reference = 0, Array = 0, Function = 0, PointerToMember = 0 };

		typedef T BaseType;
		typedef typename Private::GetBottomT< T >::BottomType BottomType;
		typedef DataType< void > ClassType;
	};

	template < typename T >
	class DataType< T & >
	{
	public:
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 0, Reference = 1, Array = 0, Function = 0, PointerToMember = 0 };

		typedef T BaseType;
		typedef typename Private::GetBottomT< T >::BottomType BottomType;
		typedef DataType< void > ClassType;
	};

	template < typename T >
	class DataType< T[] >
	{
	public:
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 0, Reference = 0, Array = 1, Function = 0, PointerToMember = 0 };

		typedef T BaseType;
		typedef typename Private::GetBottomT< T >::BottomType BottomType;
		typedef DataType< void > ClassType;
	};

	template < typename T, SIZE_T N >
	class DataType< T[ N ] >
	{
	public:
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 0, Reference = 0, Array = 1, Function = 0, PointerToMember = 0 };

		typedef T BaseType;
		typedef typename Private::GetBottomT< T >::BottomType BottomType;
		typedef DataType< void > ClassType;
	};

	template < typename T, typename C >
	class DataType< T C::* >
	{
		enum { Fundamental = 0, Compound = 1 };
		enum { Pointer = 0, Reference = 0, Array = 0, Function = 0, PointerToMember = 1 };

		typedef T BaseType;
		typedef typename Private::GetBottomT< T >::BottomType BottomType;
		typedef C ClassType;
	};

	#define _PSX_DECL_FUNDA_T( T )																\
		template < >																			\
		class DataType< T >																		\
		{																						\
		public:																					\
			enum { Fundamental = 1, Compound = 0 };												\
			enum { Pointer = 0, Reference = 0, Array = 0, Function = 0, PointerToMember = 0 };	\
		};

	_PSX_DECL_FUNDA_T(	CHAR		)
	#if !defined( PSX_UNICODE )
	_PSX_DECL_FUNDA_T(	UCHAR		)
	#endif /* PSX_UNICODE  */
	#if defined( PSX_UNICODE )
		_PSX_DECL_FUNDA_T(	Char	)
	#endif /* PSX_UNICODE  */
	_PSX_DECL_FUNDA_T(	INT			)
	_PSX_DECL_FUNDA_T(	UINT		)
	_PSX_DECL_FUNDA_T(	SHORT		)
	_PSX_DECL_FUNDA_T(	USHORT		)
	_PSX_DECL_FUNDA_T(	LONG		)
	_PSX_DECL_FUNDA_T(	ULONG		)
	_PSX_DECL_FUNDA_T(	LONGLONG	)
	_PSX_DECL_FUNDA_T(	ULONGLONG	)
	_PSX_DECL_FUNDA_T(	FLOAT		)
	_PSX_DECL_FUNDA_T(	DOUBLE		)

	#undef _PSX_DECL_FUNDA_T

	template < typename T >
	class GenericElement
	{
	public:

		PSX_INLINE GenericElement( void ) { }
		PSX_INLINE GenericElement( T &elem ) : m_elem( elem ) { }
		PSX_INLINE GenericElement( T elem ) : m_elem( elem ) { }

		PSX_INLINE T & GetRef( void ) 
		{
			return m_elem; 
		}
		PSX_INLINE T * GetPtr( void ) 
		{
			return &m_elem; 
		}

		PSX_INLINE T & operator * ( void )  { return GetRef(); }
		PSX_INLINE T * operator -> ( void ) { return GetPtr(); }

		PSX_INLINE BOOL operator == ( const GenericElement &rhs ) { return m_elem == rhs.m_elem; }
		PSX_INLINE BOOL operator == ( const T &rhs ) { return m_elem == rhs; }

		PSX_INLINE T & operator = ( T &rhs ) { m_elem = rhs; return m_elem; }

		T m_elem;
	};

	template < typename T >
	class GenericElement< T * >
	{
	public:

		typedef T* Type;

		PSX_INLINE GenericElement( void ) { }
		PSX_INLINE GenericElement( const Type &elem ) : m_elem( elem ) { }

		PSX_INLINE Type & GetRef( void ) { return m_elem; }
		PSX_INLINE Type GetPtr( void ) { return m_elem; }

		PSX_INLINE Type & operator * ( void )  { return GetRef(); }
		PSX_INLINE Type operator -> ( void ) { return GetPtr(); }

		PSX_INLINE BOOL operator == ( const GenericElement &rhs ) { return m_elem == rhs.m_elem; }
		PSX_INLINE BOOL operator == ( const T *rhs ) { return m_elem == rhs; }

		PSX_INLINE T * operator = ( T *rhs ) { m_elem = rhs; return m_elem; }

		T *m_elem;
	};

	template < typename T >
	PSX_INLINE T & GetRefGeneric( T &data )
	{
		return data;
	}

	template < typename T >
	PSX_INLINE T *& GetRefGeneric( T *data )
	{
		return data;
	}

	template < typename T >
	PSX_INLINE T * GetPtrGeneric( T &data )
	{
		return &data;
	}
	
	template < typename T >
	PSX_INLINE T * GetPtrGeneric( T *data )
	{
		return data;
	}

	/* UTILITY FUNCTIONS STARTS HERE */
	#define PSX_Destroy( _pObj ) ((T*)_pObj)->~T()

	template < typename T >
	PSX_INLINE void PSX_NoDestroy( T * pObj )
	{

	}

	template < typename T >
	PSX_INLINE void PSX_Swap( T &lhs, T &rhs )
	{
		// NOTE: Could we do a butterfuly swap?

		T temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	template < typename T >
	PSX_FORCE_INLINE T PSX_Abs( T val )
	{
		return (val < 0 ? -val : val);
	}

	template < typename T, typename Trait >
	PSX_INLINE const T & PSX_Max( const T &lhs, const T &rhs, Trait comp )
	{
		return comp( lhs, rhs ) ? lhs : rhs;
	}

	template < typename T >
	PSX_INLINE const T & PSX_Max( const T &lhs, const T &rhs )
	{
		return rhs < lhs ? lhs : rhs;
	}

	template < typename T, typename Trait >
	PSX_INLINE const T & PSX_Min( const T &lhs, const T&rhs, Trait comp )
	{
		return comp( lsh, rhs ) ? lhs : rhs;
	}

	template < typename T >
	PSX_INLINE const T & PSX_Min( const T &lhs, const T&rhs )
	{
		return rhs < lhs ? rhs : lhs;
	}

	/* FUNCTORS STARTS HERE */
	template < typename T >
	class PSX_LessThan
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs < rhs;
		}
	};

	template < typename T >
	class PSX_GreaterThan
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs > rhs;
		}
	};

	template < typename T >
	class PSX_Equal
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs == rhs;
		}
	};

	template < typename T >
	class PSX_NotEqual
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs != rhs;
		}
	};

	template < typename T >
	class PSX_GreaterEqual
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs >= rhs;
		}
	};

	template < typename T >
	class PSX_LessEqual
	{
	public:
		BOOL operator () ( const T &lhs, const T &rhs ) const
		{
			return lhs <= rhs;
		}
	};

	template < typename KVPair, typename Trait >
	class PSX_LessKV
	{
	public:
		bool operator () ( const KVPair &lhs, const KVPair &rhs ) const
		{
			return m_compare( lhs.first, rhs.first );
		}

	private:
		Trait m_compare;
	};


	/* SORTING ALGORITHM STARTS HERE */

	/* Insertion sort for simple data array */
	template < typename T >
	void PSX_InsertionSort( T *data, SIZE_T size )
	{
		T temp;
		SIZE_T j;
		
		for ( SIZE_T i = 1; i < size; ++i )
		{
			temp = data[i];

			for ( j = i; j > 0 && temp < data[j - 1]; j-- )
				data[j] = data[j - 1];
			data[j] = temp;
		}
	}

	template < typename T >
	PSX_INLINE void PSX_InsertionSort( T *pData, SIZE_T start, SIZE_T end )
	{
		PSX_InsertionSort( pData + start, end - start + 1 );
	}

	/* Insertion sort Array<T> support */
	template < typename  T >
	PSX_INLINE void PSX_InsertionSort( Array<T> &arr )
	{
		PSX_InsertionSort( &arr[0], arr.GetSize() );
	}

	template < typename T >
	PSX_INLINE void PSX_InsertionSort( Array<T> &arr, SIZE_T start, SIZE_T end )
	{
		PSX_InsertionSort( &arr[start], end - start + 1 );
	}

	template < typename T >
	void PSX_QuickSort( T *pData, const SIZE_T start, const SIZE_T end )
	{
		#define CUTOFF 10
		
		if ( start + CUTOFF > end )
			PSX_InsertionSort( pData, start, end );
		else
		{
			// Median of three partitioning
			size_t middle = ( start + end ) / 2;

			if ( pData[ middle ] < pData[ start ] )
				PSX_Swap( pData[ start ], pData[ middle ] );
			if ( pData[ end ] < pData[ start ] )
				PSX_Swap( pData[ start ], pData[ end ] );
			if ( pData[ end ] < pData[ middle ] )
				PSX_Swap( pData[ middle ], pData[ end ] );

			// Switch the pivot at second to the last
			T pivot = pData[ middle ];
			PSX_Swap( pData[ middle ], pData[ end - 1 ] );

			// Partition
			size_t i, j;

			for ( i = start, j = end - 1; ; )
			{
				// Look for a high(i) and low(j) number
				while ( pData[ ++i ] < pivot );
				while ( pivot < pData[ --j ] );

				// Swap if they haven't crossed
				if ( i < j )
					PSX_Swap( pData[ i ], pData[ j ] );
				else
					break;
			}

			// Restore pivot back
			PSX_Swap( pData[ i ], pData[ end - 1 ] );

			// Sort the two partitioned elements
			PSX_QuickSort( pData, start, i - 1 );
			PSX_QuickSort( pData, i + 1, end );
		}
		#undef CUTOFF
	}

	template < typename T >
	PSX_INLINE void PSX_QuickSort( Array< T > &arr )
	{
		PSX_QuickSort( &arr[0], 0, arr.GetSize() - 1 );
	}

	template < typename T >
	PSX_INLINE void PSX_QuickSort( Array< T > &arr, const SIZE_T start, const SIZE_T end )
	{
		PSX_QuickSort( &arr[ start ], 0, end - start );
	}

	/* SEARCHING ALGORITHMS STARTS HERE */
	template < typename Iter, typename Obj, typename Comp >
	Iter PSX_LowerBound( const Iter &start, const Iter &end, const Obj &obj,  Comp lessThan )
	{
		Iter low = start;
		Iter mid;
		Iter high = end;

		while ( low < high )
		{
			mid = low + ( high - low ) / 2;

			if ( lessThan( *mid, obj ) )
				low = mid + 1;
			else
				high = mid;
		}

		return low;
	}

	template < typename Iter, typename Obj >
	PSX_INLINE Iter PSX_LowerBound( const Iter &start, const Iter &end, const Obj &obj )
	{
		return PSX_LowerBound( start, end, obj, PSX_LessThan< Obj >() );
	}

	template < typename T, typename Obj >
	PSX_INLINE INT PSX_BinarySearch( T &pData, const SIZE_T size, const Obj &obj)
	{
		T *temp = PSX_LowerBound( pData, pData + size, obj );
		return *temp == obj ? temp - pData : -1; // TODO: Change -1 to some defined error code
	}

	template < typename T, typename Obj >
	PSX_INLINE INT PSX_BinarySearch( T &pStart, T &pEnd, const Obj &obj)
	{
		T *temp = PSX_LowerBound( pStart, pEnd, obj );
		return *temp == obj ? temp - pStart : -1; // TODO: Change -1 to some defined error code
	}

	/* Utility Classes */
	template < typename T1, typename T2 >
	class PSX_Pair
	{
	public:
		T1 first;
		T2 second;

		PSX_Pair( const T1 &_first = T1(), const T2 &_second = T2() ) 
			: first( _first ), second( _second ) 
		{ }
	};

	template < typename T >
	class PSX_CRef
	{
	public:

		PSX_CRef( const T &obj ) : m_pObj( &obj )
		{ }

		PSX_CRef( const T *pObj = NULL ) : m_pObj( pObj )
		{ }

		const T & Get( void )
		{
			PSX_Assert( !IsNull(), "Object reference is NULL." );
			return *m_pObj;
		}

		BOOL IsNull( void ) const
		{
			return m_pObj == NULL;
		}

	private:

		const T *m_pObj;
	};

};

#endif /* _PSX_TEMPLATE_LIBRARY_H_ */