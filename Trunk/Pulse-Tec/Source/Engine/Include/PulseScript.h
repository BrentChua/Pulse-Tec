/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseScript.h
*
*	Description -	This is used to read Pulse scripts. Such scripts are used for meshes,
*						materials, particle effects, procedural textures, config files, etc.
*
*	Comments	-	I think it would be better if we can formalize all pulse srcipt with 
*						a ".ps" file extension format. "ps" stands for Pulse Script.
*					This implementation is crapadoodle... We need to rethink how to code this.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/07/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_MATERIAL_SCRIPT_H_
#define _PSX_MATERIAL_SCRIPT_H_

// So many includes!!!
#include "PulseSTD.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Array.h"
#include "Map.h"
#include "Stream.h"
#include "Optional.h"

namespace Pulse
{
	class PulseScript
	{
	public:

		class Section
		{
		public:

			// We could actually use a hashtable here for an even faster lookup.
			typedef Array< Section * >			SectionList;
			typedef Array<INT>					IntArray;
			typedef Array<FLOAT>				FloatArray;
			typedef Array<Vector2>				Vector2Array;
			typedef Array<Vector3>				Vector3Array;
			typedef Array<Vector4>				Vector4Array;
			typedef Map< String, String >		StringMap;
			typedef Map< String, INT >			IntMap;
			typedef Map< String, IntArray >		IntArrayMap;
			typedef Map< String, FLOAT >		FloatMap;
			typedef Map< String, FloatArray >	FloatArrayMap;
			typedef Map< String, BOOL >			BoolMap;
			typedef Map< String, Vector2 >		Vec2Map;
			typedef Map< String, Vector2Array >	Vec2ArrayMap;
			typedef Map< String, Vector3 >		Vec3Map;
			typedef Map< String, Vector3Array >	Vec3ArrayMap;
			typedef Map< String, Vector4 >		Vec4Map;
			typedef Map< String, Vector4Array >	Vec4ArrayMap;
			typedef Map< String, SectionList >	SectionMap;

			// Optionals
			typedef Optional< String * >		OPString;
			typedef Optional< INT * >			OPInt;
			typedef Optional< IntArray * >		OPIntArray;
			typedef Optional< FLOAT * >			OPFloat;
			typedef Optional< FloatArray * >	OPFloatArray;
			typedef Optional< BOOL * >			OPBoool;
			typedef Optional< Vector2 * >		OPVec2;
			typedef Optional< Vector2Array * >	OPVec2Array;
			typedef Optional< Vector3 * >		OPVec3;
			typedef Optional< Vector3Array * >	OPVec3Array;
			typedef Optional< Vector4 * >		OPVec4;
			typedef Optional< Vector4Array * >	OPVec4Array;
			typedef Optional< SectionList * >	OPSectionList;
			
			~Section( void );

			void Print( OStream &out, const String &prefix = PSX_String("") ) const;

			BOOL IsValid( void ) const;

			// NOTE: Check token functions
			BOOL ContainsString( const String & id )		const;
			BOOL ContainsInt( const String & id )			const;
			BOOL ContainsIntArray( const String & id )		const;
			BOOL ContainsFloat( const String & id )			const;
			BOOL ContainsFloatArray( const String & id )	const;
			BOOL ContainsBool( const String & id )			const;
			BOOL ContainsVec2( const String & id )			const;
			BOOL ContainsVec2Array( const String & id )		const;
			BOOL ContainsVec3( const String & id )			const;
			BOOL ContainsVec3Array( const String & id )		const;
			BOOL ContainsVec4( const String & id )			const;
			BOOL ContainsVec4Array( const String & id )		const;
			BOOL ContainsSection( const String & id )		const;

			// NOTE: Get token functions. Note that they are all references.
			OPString		GetString( const String &id )		const;
			OPInt			GetInt( const String &id )			const;
			OPIntArray		GetIntArray( const String &id )		const;
			OPFloat			GetFloat( const String &id )		const;
			OPFloatArray	GetFloatArray( const String &id )	const;
			OPBoool			GetBool( const String &id )			const;
			OPVec2 			GetVec2( const String &id ) 		const;
			OPVec2Array		GetVec2Array( const String &id ) 	const;
			OPVec3 			GetVec3( const String &id ) 		const;
			OPVec3Array		GetVec3Array( const String &id ) 	const;
			OPVec4 			GetVec4( const String &id ) 		const;
			OPVec4Array		GetVec4Array( const String &id ) 	const;
			OPSectionList 	GetSectionList( const String &id )	const;

		private:

			friend PulseScript;

			struct BuildInfo
			{
				BuildInfo( IStream &in, const String &filename, INT lineNumber, BOOL global )
					: m_in( in ), m_fileName( filename ), m_lineNumber( lineNumber ), m_bGlobal( global )
				{
				}

				IStream &m_in;
				const String &m_fileName;
				INT m_lineNumber;
				BOOL m_bGlobal;
			};

			Section( void );
			Section( IStream &in, const String &filename );

			// Private Helper Functions
			static BOOL IsAlphaNumeric( const CHAR c );
			static EErrorCode::Type GetLine( BuildInfo *pInfo, String &lineStr, BOOL bIncludeComments = FALSE );

			EErrorCode::Type LoadFromFile( BuildInfo *pInfo );
			EErrorCode::Type ReadSection( BuildInfo *pInfo, const String &id );
			EErrorCode::Type ReadValue( BuildInfo *pInfo, const String &keyword, BOOL bIsArray, String &lineStr, SIZE_T index );

		private:
			StringMap		m_strings;
			IntMap			m_ints;
			IntArrayMap		m_intArrays;
			FloatMap		m_floats;
			FloatArrayMap	m_floatArrays;
			BoolMap			m_bools;
			Vec2Map			m_vec2s;
			Vec2ArrayMap	m_vec2Arrays;
			Vec3Map			m_vec3s;
			Vec3ArrayMap	m_vec3Arrays;
			Vec4Map			m_vec4s;
			Vec4ArrayMap	m_vec4Arrays;
			SectionMap		m_sections;
			BOOL			m_bIsValid;
		};

		typedef Section::SectionList SectionList;

		typedef Section::OPString		OPString;
		typedef Section::OPInt			OPInt;
		typedef Section::OPIntArray		OPIntArray;
		typedef Section::OPFloat		OPFloat;
		typedef Section::OPFloatArray	OPFloatArray;
		typedef Section::OPBoool		OPBoool;
		typedef Section::OPVec2			OPVec2;
		typedef Section::OPVec2Array	OPVec2Array;
		typedef Section::OPVec3			OPVec3;
		typedef Section::OPVec3Array	OPVec3Array;
		typedef Section::OPVec4			OPVec4;
		typedef Section::OPVec4Array	OPVec4Array;
		typedef Section::OPSectionList	OPSectionList;

		// Public interface
		PulseScript( void );
		PulseScript( const String &filename );
		~PulseScript( void );

		EErrorCode::Type LoadScript( const String &filename );
		void UnloadScript( void );

		const CHAR * GetName( void ) const;
		void Print( OStream &out ) const;

		BOOL IsValid( void ) const;

		// NOTE: Check token functions
		BOOL ContainsString( const String & id )		const;
		BOOL ContainsInt( const String & id )			const;
		BOOL ContainsIntArray( const String & id )		const;
		BOOL ContainsFloat( const String & id )			const;
		BOOL ContainsFloatArray( const String & id )	const;
		BOOL ContainsBool( const String & id )			const;
		BOOL ContainsVec2( const String & id )			const;
		BOOL ContainsVec2Array( const String & id )		const;
		BOOL ContainsVec3( const String & id )			const;
		BOOL ContainsVec3Array( const String & id )		const;
		BOOL ContainsVec4( const String & id )			const;
		BOOL ContainsVec4Array( const String & id )		const;
		BOOL ContainsSection( const String & id )		const;

		// NOTE: Get token functions. Note that they are all references.
		OPString		GetString( const String &id )		const;
		OPInt			GetInt( const String &id )			const;
		OPIntArray		GetIntArray( const String &id )		const;
		OPFloat			GetFloat( const String &id )		const;
		OPFloatArray	GetFloatArray( const String &id )	const;
		OPBoool			GetBool( const String &id )			const;
		OPVec2 			GetVec2( const String &id ) 		const;
		OPVec2Array		GetVec2Array( const String &id ) 	const;
		OPVec3 			GetVec3( const String &id ) 		const;
		OPVec3Array		GetVec3Array( const String &id ) 	const;
		OPVec4 			GetVec4( const String &id ) 		const;
		OPVec4Array		GetVec4Array( const String &id ) 	const;
		OPSectionList 	GetSectionList( const String &id )	const;

	private:

		friend class RegistryObjectPulseScript;

		// Private members
		Section *m_pGlobal;
		String m_name;

	};

	PSX_INLINE PulseScript::Section::Section( void )
	{

	}

	PSX_INLINE BOOL PulseScript::Section::IsValid( void ) const
	{
		return m_bIsValid;
	}

	PSX_INLINE const CHAR * PulseScript::GetName( void ) const
	{
		return m_name.GetBuffer();
	}

	PSX_INLINE void PulseScript::Print( OStream &out ) const
	{
		return m_pGlobal->Print( out );
	}

	PSX_INLINE BOOL PulseScript::IsValid( void ) const
	{
		if ( !m_pGlobal ) 
			return FALSE;

		return m_pGlobal->IsValid();
	}

	// NOTE: Check token functions
	PSX_INLINE BOOL PulseScript::ContainsString( const String & id ) const
	{
		return m_pGlobal->ContainsString( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsInt( const String & id ) const
	{
		return m_pGlobal->ContainsInt( id );
	}
	PSX_INLINE BOOL PulseScript::ContainsIntArray( const String & id ) const
	{
		return m_pGlobal->ContainsIntArray( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsFloat( const String & id )	const
	{
		return m_pGlobal->ContainsFloat( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsFloatArray( const String & id ) const
	{
		return m_pGlobal->ContainsFloatArray( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsBool( const String & id ) const
	{
		return m_pGlobal->ContainsBool( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec2( const String & id ) const
	{
		return m_pGlobal->ContainsVec2( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec2Array( const String & id ) const
	{
		return m_pGlobal->ContainsVec2Array( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec3( const String & id ) const
	{
		return m_pGlobal->ContainsVec3( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec3Array( const String & id ) const
	{
		return m_pGlobal->ContainsVec3Array( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec4( const String & id ) const
	{
		return m_pGlobal->ContainsVec4( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsVec4Array( const String & id ) const
	{
		return m_pGlobal->ContainsVec4Array( id );
	}

	PSX_INLINE BOOL PulseScript::ContainsSection( const String & id ) const
	{
		return m_pGlobal->ContainsSection( id );
	}

	// NOTE: Get token functions. Note that they are all references.
	PSX_INLINE PulseScript::OPString PulseScript::GetString( const String &id ) const
	{
		return m_pGlobal->GetString( id );
	}

	PSX_INLINE PulseScript::OPInt PulseScript::GetInt( const String &id ) const
	{
		return m_pGlobal->GetInt( id );
	}

	PSX_INLINE PulseScript::OPIntArray PulseScript::GetIntArray( const String &id ) const
	{
		return m_pGlobal->GetIntArray( id );
	}

	PSX_INLINE PulseScript::OPFloat PulseScript::GetFloat( const String &id ) const
	{
		return m_pGlobal->GetFloat( id );
	}

	PSX_INLINE PulseScript::OPFloatArray	PulseScript::GetFloatArray( const String &id ) const
	{
		return m_pGlobal->GetFloatArray( id );
	}

	PSX_INLINE PulseScript::OPBoool PulseScript::GetBool( const String &id ) const
	{
		return m_pGlobal->GetBool( id );
	}

	PSX_INLINE PulseScript::OPVec2 PulseScript::GetVec2( const String &id ) const
	{
		return m_pGlobal->GetVec2( id );
	}

	PSX_INLINE PulseScript::OPVec2Array PulseScript::GetVec2Array( const String &id ) const
	{
		return m_pGlobal->GetVec2Array( id );
	}

	PSX_INLINE PulseScript::OPVec3 PulseScript::GetVec3( const String &id ) const
	{
		return m_pGlobal->GetVec3( id );
	}

	PSX_INLINE PulseScript::OPVec3Array PulseScript::GetVec3Array( const String &id ) const
	{
		return m_pGlobal->GetVec3Array( id );
	}

	PSX_INLINE PulseScript::OPVec4 PulseScript::GetVec4( const String &id ) const
	{
		return m_pGlobal->GetVec4( id );
	}

	PSX_INLINE PulseScript::OPVec4Array PulseScript::GetVec4Array( const String &id ) const
	{
		return m_pGlobal->GetVec4Array( id );
	}

	PSX_INLINE PulseScript::OPSectionList PulseScript::GetSectionList( const String &id ) const
	{
		return m_pGlobal->GetSectionList( id );
	}

}

#endif /* _PSX_MATERIAL_SCRIPT_H_ */