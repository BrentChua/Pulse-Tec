/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseSTD.cpp
*
*	Comments	-	See PulseBase.h
*
**************************************************************************************/

#include "../Include/PulseSTD.h"
namespace Pulse
{
	UINT PSX_SuperFastHash( CONST CHAR *pData, UINT len )
	{
		
		#define PSX_Get16Bit( d ) ((((U32)(((const U8 *)(d))[1])) << 8) + (U32)(((const U8 *)(d))[0]) )

		/* Mr. Paul Hsieh's awesome fast hash function */
		/* http://www.azillionmonkeys.com/qed/hash.html */

		#if defined( PSX_PLATFORM_WINDOWS )
	
			UINT hash = 0;
			if ( pData == NULL || len == 0 ) return 0;

			__asm 
			{
				xor   ebx, ebx
					mov   esi, pData
					mov   edi, len
					mov   eax, edi
					mov   ecx, edi
					and   edi, 3
					shr   ecx, 2
					jz    L2

			L1:
				movzx ebx, word ptr [esi]   ; 0
					add   eax, ebx              ; 1
					movzx ebx, word ptr [esi+2] ; 0

					shl   ebx, 11               ; 1
					xor   ebx, eax              ; 2
					shl   eax, 16               ; 2

					add   esi, 4                ; 0
					xor   eax, ebx              ; 3
					mov   edx, eax              ; 4

					shr   eax, 11               ; 4
					add   eax, edx              ; 5
					dec   ecx                   ; 2
					jnz   L1

			L2:

				mov   ecx, edi
					cmp   ecx, 1
					jz    lcase1
					cmp   ecx, 2
					jz    lcase2
					cmp   ecx, 3
					jnz   L3

					mov    bx, word ptr [esi]
				add   eax, ebx
					xor   ebx, ebx
					mov   edx, eax
					shl   eax, 16
					xor   eax, edx
					movsx ebx, byte ptr [esi+2]
				shl   ebx, 18
					xor   eax, ebx
					mov   edx, eax
					shr   eax, 11
					add   eax, edx
					jmp   L3

			lcase2:
				mov    bx, word ptr [esi]
				add   eax, ebx
					mov   edx, eax
					shl   eax, 11
					xor   eax, edx
					mov   edx, eax
					shr   eax, 17
					add   eax, edx
					jmp   L3

			lcase1:
				movsx ebx, byte ptr [esi]
				add   eax, ebx
					mov   edx, eax
					shl   eax, 10
					xor   eax, edx
					mov   edx, eax
					shr   eax, 1
					add   eax, edx

			L3:

				mov   ebx, eax
					shl   eax, 3
					xor   eax, ebx
					mov   ebx, eax
					shr   eax, 5
					add   eax, ebx
					mov   ebx, eax
					shl   eax, 4
					xor   eax, ebx
					mov   ebx, eax
					shr   eax, 17
					add   eax, ebx
					mov   ebx, eax
					shl   eax, 25
					xor   eax, ebx
					mov   ebx, eax
					shr   eax, 6
					add   eax, ebx

					mov   hash, eax
			}

			return hash;
		#else
			UINT hash = len;
			UINT tmp;
			int rem;

			if ( len <= 0 || pData == NULL ) return 0;

			rem = len & 3;
			len >>= 2;

			for ( ; len > 0; --len )
			{
				hash += PSX_Get16Bit( pData );
				tmp = (PSX_Get16Bit( pData + 2 ) << 11) ^ hash;
				hash = (hash << 16) ^ tmp;
				pData += 2 * sizeof( U16 );
				hash += hash >> 11;
			}

			/* Handle end cases */
			switch (rem) 
			{
			case 3: hash += PSX_Get16Bit(pData);
				hash ^= hash << 16;
				hash ^= pData[sizeof(U16)] << 18;
				hash += hash >> 11;
				break;
			case 2: hash += PSX_Get16Bit(pData);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
			case 1: hash += *pData;
				hash ^= hash << 10;
				hash += hash >> 1;
			}

			/* Force "avalanching" of final 127 bits */
			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;

			return hash;
		#endif

		#undef PSX_Get16Bit
	};

	BOOL PSX_IsPrime( UINT n )
	{
		if( n == 2 || n == 3 )
			return true;

		if( n == 1 || n % 2 == 0 )
			return false;

		for( UINT i = 3; i * i <= n; i += 2 )
			if( n % i == 0 )
				return false;

		return true;
	}

	UINT PSX_NextPrime( UINT n )
	{
		if( n % 2 == 0 )
			n++;

		for( ; !PSX_IsPrime( n ); n += 2 );

		return n;
	}

	void PSX_StringToLower( const CHAR *pIn, CHAR *pOut )
	{
		while ( *pIn )							
		{	
			#if defined( PSX_UNICODE )
				*pOut = towlower( *pIn );			
			#else
				*pOut = tolower( *pIn );			
			#endif
			++pOut;								
			++pIn;								
		}
	}

	void PSX_StringToUpper( const CHAR *pIn, CHAR *pOut )
	{
		while ( *pIn )							
		{	
			#if defined( PSX_UNICODE )
				*pOut = towupper( *pIn );			
			#else
				*pOut = toupper( *pIn );			
			#endif
			++pOut;								
			++pIn;								
		}

		// Make sure to copy the null too
		*pOut = *pIn;
	}

	BOOL PSX_StringContains( const CHAR *pString, const CHAR *pToken, BOOL bCaseSensitive )
	{
		BOOL bReturn = FALSE;

		if ( bCaseSensitive )
		{
			#if defined( PSX_UNICODE )
				bReturn = (wcsstr ( pString, pToken ) != NULL);
			#else
				bReturn = (strstr( pString, pToken ) != NULL);
			#endif
		}
		else
		{
			const UINT tokenLen = PSX_StrLen( pToken );

			while ( PSX_StrLen( pString ) >= tokenLen )
			{
			#if defined( PSX_UNICODE )
				if ( wcsnicmp( pString, pToken, tokenLen ) )
			#else
				if ( strnicmp( pString, pToken, tokenLen ) )
			#endif
				{
					//bReturn = TRUE;
					return TRUE;
				}

				++pString;
			}
		}

		return bReturn;

	}
};