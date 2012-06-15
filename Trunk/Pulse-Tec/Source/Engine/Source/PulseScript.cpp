/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseScript.cpp
*
*	Comments	-	See PulseScript.h
*
**************************************************************************************/

#include "../Include/PulseScript.h"
#include "../Include/FileIO.h"

namespace Pulse
{
	PSX_REGISTRY_OBJECT_START( PulseScript, ERegistryPriority::Normal, ERegistryPriority::Normal )
		PSX_REGISTRY_ON_TEST()
		{
			//PulseScript script( PSX_String("Demo\\SampleScript.ps") );

		}
	PSX_REGISTRY_OBJECT_END()

	PulseScript::Section::Section( IStream &in, const String &filename )
	{
		BuildInfo info( in, filename, 0, TRUE );
		m_bIsValid = LoadFromFile( &info ) == EErrorCode::NONE ? TRUE : FALSE;
	}

	PulseScript::Section::~Section( void )
	{
		SectionMap::Iterator iter1 = m_sections.IteratorBegin();
		SectionMap::Iterator iter1End = m_sections.IteratorEnd();
		SectionList::Iterator iter2;
		SectionList::Iterator iter2End;

		while ( iter1 != iter1End )
		{
			iter2 = iter1->second.IteratorBegin();
			iter2End = iter1->second.IteratorEnd();

			while ( iter2 != iter2End )
			{
				delete (*iter2);
				++iter2;
			}
			++iter1;
		}
	}

	// NOTE: Check token functions
	BOOL PulseScript::Section::ContainsString( const String & id ) const
	{
		return m_strings.Find( id ) != m_strings.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsInt( const String & id ) const
	{
		return m_ints.Find( id ) != m_ints.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsIntArray( const String & id ) const
	{
		return m_intArrays.Find( id ) != m_intArrays.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsFloat( const String & id ) const
	{
		return m_floats.Find( id ) != m_floats.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsFloatArray( const String & id ) const
	{
		return m_floatArrays.Find( id ) != m_floatArrays.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsBool( const String & id ) const
	{
		return m_bools.Find( id ) != m_bools.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec2( const String & id ) const
	{
		return m_vec2s.Find( id ) != m_vec2s.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec2Array( const String & id ) const
	{
		return m_vec2Arrays.Find( id ) != m_vec2Arrays.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec3( const String & id ) const
	{
		return m_vec3s.Find( id ) != m_vec3s.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec3Array( const String & id ) const
	{
		return m_vec3Arrays.Find( id ) != m_vec3Arrays.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec4( const String & id ) const
	{
		return m_vec4s.Find( id ) != m_vec4s.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsVec4Array( const String & id ) const
	{
		return m_vec4Arrays.Find( id ) != m_vec4Arrays.IteratorEnd();
	}

	BOOL PulseScript::Section::ContainsSection( const String & id ) const
	{
		return m_sections.Find( id ) != m_sections.IteratorEnd();
	}

	PulseScript::Section::OPString PulseScript::Section::GetString( const String &id ) const
	{
		StringMap::Iterator iter = m_strings.Find( id );
		
		if ( iter != m_strings.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPInt PulseScript::Section::GetInt( const String &id ) const
	{
		IntMap::Iterator iter = m_ints.Find( id );
		
		if ( iter != m_ints.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPIntArray PulseScript::Section::GetIntArray( const String &id ) const
	{
		IntArrayMap::Iterator iter = m_intArrays.Find( id );
		
		if ( iter != m_intArrays.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPFloat PulseScript::Section::GetFloat( const String &id ) const
	{
		FloatMap::Iterator iter = m_floats.Find( id );

		if ( iter != m_floats.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPFloatArray PulseScript::Section::GetFloatArray( const String &id ) const
	{
		FloatArrayMap::Iterator iter = m_floatArrays.Find( id );

		if ( iter != m_floatArrays.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPBoool PulseScript::Section::GetBool( const String &id ) const
	{
		BoolMap::Iterator iter = m_bools.Find( id );

		if ( iter != m_bools.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec2 PulseScript::Section::GetVec2( const String &id ) const
	{
		Vec2Map::Iterator iter = m_vec2s.Find( id );

		if ( iter != m_vec2s.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec2Array PulseScript::Section::GetVec2Array( const String &id ) const
	{
		Vec2ArrayMap::Iterator iter = m_vec2Arrays.Find( id );
		
		if ( iter != m_vec2Arrays.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec3 PulseScript::Section::GetVec3( const String &id ) const
	{
		Vec3Map::Iterator iter = m_vec3s.Find( id );
		
		if ( iter != m_vec3s.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec3Array PulseScript::Section::GetVec3Array( const String &id ) const
	{
		Vec3ArrayMap::Iterator iter = m_vec3Arrays.Find( id );

		if ( iter != m_vec3Arrays.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec4 PulseScript::Section::GetVec4( const String &id ) const
	{
		Vec4Map::Iterator iter = m_vec4s.Find( id );

		if ( iter != m_vec4s.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPVec4Array PulseScript::Section::GetVec4Array( const String &id ) const
	{
		Vec4ArrayMap::Iterator iter = m_vec4Arrays.Find( id );

		if ( iter != m_vec4Arrays.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	PulseScript::Section::OPSectionList PulseScript::Section::GetSectionList( const String &id )const
	{
		SectionMap::Iterator iter = m_sections.Find( id );

		if ( iter != m_sections.IteratorEnd() )
			return &iter->second;
		else
			return OptionalEmpty();
	}

	EErrorCode::Type PulseScript::Section::GetLine( BuildInfo *pInfo, String &lineStr, BOOL bIncludeComments )
	{
		IStream &in = pInfo->m_in;
		SIZE_T MLStart = PSX_NPOS;	// Multi-line comment index start
		SIZE_T MLEnd;				// Multi-line comment index end
		SIZE_T index;

		while ( TRUE )
		{
			if ( in.IsEOF() )
			{
				if ( pInfo->m_bGlobal )
				{
					lineStr.Clear();
					return EErrorCode::NONE;
				}else
				{
					String str;
					PSX_PushError( "PulseScript Unexpected end of file." );
					return EErrorCode::SCRIPT;
				}
			}

			++pInfo->m_lineNumber;
			in.GetLine( lineStr );
			
			if ( lineStr.GetLength() == 0 )
				continue;

			index = lineStr.FindFirstNotOf( PSX_String(" \t\n") );
			
			if ( index == PSX_NPOS && MLStart == PSX_NPOS )
				continue;

			//if ( bIncludeComments )
			//	break;

			// Detect if there's a multi-line comment (i.e. /* ... */ ). Or a single line partial/section comment.
			if ( MLStart == PSX_NPOS )
				MLStart = lineStr.Find( PSX_String("/*"), index );

			if ( MLStart != PSX_NPOS )
			{
				MLEnd = lineStr.Find( PSX_String("*/"), MLStart );
				if ( MLEnd == PSX_NPOS )
				{
					//MLStart = PSX_NPOS;
					continue;
				}

				// Skip '*' and '/'
				MLEnd += 2;

				if ( index == MLStart )
					index = MLEnd;
			}

			// If the first character of the line is not a comment then we have a legit line of code
			if ( !lineStr.IsEmpty() && (index < lineStr.GetLength()) && lineStr[index] != PSX_String('/')  && lineStr[index] != PSX_String('\n') )
				break;
			else
			{
				MLStart = MLEnd = PSX_NPOS;
			}
		}

		// Strip out all available section comments.
		while ( TRUE )
		{
			if ( MLStart != PSX_NPOS )
			{
				lineStr = lineStr.SubString( 0, MLStart ) + lineStr.SubString( MLEnd );
				MLStart = MLEnd = PSX_NPOS;
				index = lineStr.FindFirstNotOf( PSX_String(" \t\n") );
			}
			else 
				break;

			// Detect if there's a multi-line comment (i.e. /* ... */ ). Or a single line partial/section comment.
			if ( MLStart == PSX_NPOS )
				MLStart = lineStr.Find( PSX_String("/*"), index );

			if ( MLStart != PSX_NPOS )
			{
				MLEnd = lineStr.Find( PSX_String("*/"), MLStart );
				if ( MLEnd == PSX_NPOS )
				{
					MLStart = PSX_NPOS;
					continue;
				}

				// Skip '*' and '/'
				MLEnd += 2;
			}

		}

		// We also want to remove the comment and or the newline at the end of the line if it's there
		SIZE_T endIndex;
		SIZE_T tempIndex = index;
		do
		{
			endIndex = lineStr.FindFirstOf( PSX_String("/\n"), tempIndex );

			// If the next character is also a forward-slash then it's a comment
			if ( endIndex != PSX_NPOS )
			{
				if ( lineStr[endIndex] == PSX_String('\n') )
					break;
				else if ( endIndex < (lineStr.GetLength() - 1) )
				{
					if ( lineStr[endIndex + 1] == PSX_String('/') )
						break;
					else if ( lineStr[endIndex - 1] == PSX_String('<') )
						tempIndex = endIndex + 1;
				}
			}

		} while( endIndex != PSX_NPOS );

		if ( endIndex == PSX_NPOS )
			endIndex = lineStr.GetLength();
		else
		{
			// Remove excess spaces or tabs
			while ( lineStr[endIndex - 1] == PSX_String(' ') || lineStr[endIndex - 1] == PSX_String('\t') )
				--endIndex;
		}

		lineStr = lineStr.SubString( index, endIndex - index );

		return EErrorCode::NONE;
	}

	EErrorCode::Type PulseScript::Section::LoadFromFile( BuildInfo *pInfo )
	{
		String lineStr;
		EErrorCode::Type err;
		BOOL bStop, bEndSection, bDeclaringArray;
		SIZE_T index;

		while ( (err = GetLine( pInfo, lineStr )) == EErrorCode::NONE )
		{
			if ( lineStr.IsEmpty() )
				break;

			index = 0;
			bStop = FALSE;
			bEndSection = FALSE;
			bDeclaringArray = FALSE;
			
			// Get the id
			while ( !bStop && index < lineStr.GetLength() )
			{
				switch( lineStr[index] )
				{
				case PSX_String('}'):
					{
						// Make sure the section is ended with a semi-colon
						SIZE_T SCIndex = lineStr.FindFirstOf( PSX_String(';') );
						if ( SCIndex != PSX_NPOS && lineStr[SCIndex] == PSX_String(';') )
							bEndSection = TRUE;
						else
						{
							PSX_PushError( "PulseScript: Expecting ';' to end section." );
							return EErrorCode::SCRIPT;
						}
					}
				case PSX_String('='):
				case PSX_String(' '):
				case PSX_String('\t'):
				//case PSX_String('\n'):
					bStop = TRUE;
					break;
				// These are used for declaring arrays.
				case PSX_String('['):
					if ( bDeclaringArray == FALSE )
						bDeclaringArray = TRUE;
					else
					{
						PSX_PushError( "PulseScript: Can't use another '[' inside a '[' in declaration." );
						return EErrorCode::SCRIPT;
					}
				case PSX_String(']'):
					++index;
					break;
				default:
					PSX_Assert( IsAlphaNumeric( lineStr[index] ), "Invalid ID name." );
					++index;
				}

			}

			if ( bEndSection )
				break;

			if ( bDeclaringArray )
			{
				if ( index == 0 )
				{
					PSX_PushError( "PulseScript: Can't start with '[' in a declaration." );
					return EErrorCode::SCRIPT;
				}
				else if ( lineStr[index - 1] != PSX_String(']') )
				{
					PSX_PushError( "PulseScript: Expecting ']' to close array declaration." );
					return EErrorCode::SCRIPT;
				}
			}

			// Check if it's a section
			String id = lineStr.SubString( 0, index );
			BOOL bIsSection = id.Find( PSX_String("Section" ) ) == 0;

			// Get the section or value
			if ( bIsSection )
			{
				// Extract id name
				SIZE_T idStart = lineStr.FindFirstNotOf( PSX_String(" \t\n"), index );

				if ( idStart == PSX_NPOS )
				{
					PSX_PushError( "PulseScript: Expecting identifier after 'Section' declaration" );
					return EErrorCode::SCRIPT;
				}

				SIZE_T idEnd = lineStr.FindFirstOf( PSX_String(" \t\n"), idStart );

				if ( idEnd == PSX_NPOS )
					idEnd = lineStr.GetLength();

				id = lineStr.SubString( idStart, idEnd - idStart );
				if ( (err = ReadSection( pInfo, id )) != EErrorCode::NONE )
					return err;
			}
			else
			{
				// In here, id is the keyword (i.e. int bool, float, Vector..., etc.)
				if ( (err = ReadValue( pInfo, id, bDeclaringArray, lineStr, index )) != EErrorCode::NONE )
				{
					LogBuffer buff = PSX_PopLog();
					return err;
				}
			}
		}

		return err;
	}

	EErrorCode::Type PulseScript::Section::ReadSection( BuildInfo *pInfo, const String &id )
	{
		String lineStr;
		BuildInfo subInfo( pInfo->m_in, pInfo->m_fileName, pInfo->m_lineNumber, false );
		SIZE_T index = id.FindFirstNotOf( PSX_String("{") );
		EErrorCode::Type error = GetLine( pInfo, lineStr );

		if ( error != EErrorCode::NONE )
			return error;

		if ( index != 0 )
		{
			PSX_PushError( "PulseScript: New Section expecting Section identifier." );
			return EErrorCode::SCRIPT;
		}

		if ( (index + 1) < lineStr.GetLength() && lineStr.FindFirstNotOf( PSX_String(" \t"), index + 1 ) != PSX_NPOS )
		{
			PSX_PushError( "PulseScript: Declarations not allowed on the same line of opening Section '{'." );
			return EErrorCode::SCRIPT;
		}

		//Create sub-section
		Section *pNewSection = new Section;

		if ( (error = pNewSection->LoadFromFile( &subInfo )) != EErrorCode::NONE )
			return error;

		pInfo->m_lineNumber = subInfo.m_lineNumber;
		m_sections[id].PushBack( pNewSection );

		return EErrorCode::NONE;
	}

	EErrorCode::Type PulseScript::Section::ReadValue( BuildInfo *pInfo, const String &keyword, BOOL bIsArray, String &lineStr, SIZE_T index )
	{
		SIZE_T	start = lineStr.FindFirstNotOf( PSX_String(" \t"), index );
		SIZE_T	end = lineStr.FindFirstOf( PSX_String(" \t="), start );
		BOOL	bIgnoreRedundantCases = FALSE;
		
		// Get the value
		if ( index == PSX_NPOS )
		{
			PSX_PushError( "PulseScript: Expecting an identifier after type declaration." );
			return EErrorCode::SCRIPT;
		}
		else if ( end == start )
		{
			PSX_PushError( "PulseScript: No identifier found." );
			return EErrorCode::SCRIPT;
		}

		String id = lineStr.SubString( start, end - start );

		start = lineStr.FindFirstNotOf( PSX_String( " \t=" ), end );
		end = lineStr.FindFirstOf( PSX_String( ";" ), start );

		if ( end == PSX_NPOS )
		{
			// If it's not a semi-colon, it could be a back-slash for multi-line strings
			// Another version of string is using tags to avoid \ for every line
			if ( lineStr[ lineStr.GetLength() - 1 ] == PSX_String( '\\' ) || 
				(lineStr.SubString( start, 8 ) == PSX_String("<String>")) )
			{
				bIgnoreRedundantCases = TRUE;
				end = lineStr.GetLength();
			}
			else
			{
				PSX_PushError( "PulseScript: No ';' found." );
				return EErrorCode::SCRIPT;
			}
		}
		else if ( keyword == PSX_String( "String" ) )
		{
			bIgnoreRedundantCases = TRUE;
			end = lineStr.GetLength();
		}

		// We want to remove reduntant characters such as ; and spaces
		if (  bIgnoreRedundantCases == FALSE )
			while ( (lineStr[end-1] == PSX_String(' ') || lineStr[end-1] == PSX_String('\t')) )
				--end;

		String value = lineStr.SubString( start, end - start );

		if ( bIsArray == FALSE )
		{
			// bool
			if ( keyword == PSX_String( "Bool" ) )
			{
				if ( value == PSX_String("true") )
					m_bools[id] = true;
				else if ( value == PSX_String("false") )
					m_bools[id] = false;
			}

			// string
			else if ( keyword == PSX_String( "String" ) )
			{
				String str;
				SIZE_T excessIndex = PSX_NPOS;
				SIZE_T tempExcess = 0;
				EErrorCode::Type err;
				BOOL bIsMultiLine = FALSE;
				BOOL bUsingStringTag = value.SubString( 0, 8 ) == PSX_String( "<String>" );

				// This could be a multi-line string. So we need to loop
				while( true )
				{
					if ( bIsMultiLine )
					{
						if ( bUsingStringTag )
						{
							// If this contains the end tag
							if ( value.GetLength() >= 10 && value.SubString( value.GetLength() - 10 ) == PSX_String( "</String>;" ) )
							{
								if ( value.GetLength() - 10 )
									str += value.SubString( 0, value.GetLength() - 10);
								
								break;
							}
							else
								str += value.SubString( 0 );
						}
						else
							str += value.SubString( 0, value.GetLength() - 1 );
					}
					else
					{
						if ( bUsingStringTag )
						{
							// If this contains the end tag
							if ( value.GetLength() >= 10 && value.SubString( value.GetLength() - 10 ) == PSX_String( "</String>;" ) )
							{
								if ( value.GetLength() - 8 - 10 )
									str += value.SubString( 8, value.GetLength() - 8 - 10 );
								
								break;
							}
							else
								str += value.SubString( 8 );
						}
						else
							str += value.SubString( 1, value.GetLength() - 2 );
					}

					if ( !bUsingStringTag )
					{
						// If no backslash that means it's the end of the line
						if ( value[value.GetLength() - 1] == PSX_String(';') )
						{
							break;
						}
						else if ( value[value.GetLength() - 1] != PSX_String('\\') )
						{
							PSX_PushError( "PulseScript: Expecting '\' to read next line or '\"' to end string." );
							return EErrorCode::SCRIPT;
						}
					}

					bIsMultiLine = TRUE;

					if ( (err = GetLine( pInfo, value, TRUE )) != EErrorCode::NONE )
						return err;

					// If we find a semicolon then that's the end of the string
					if ( !bUsingStringTag )
					{
						excessIndex = value.FindLastNotOf( PSX_String(" \t;") );
						if ( (excessIndex + 1) != value.GetLength() && excessIndex != PSX_NPOS )
						{
							str += value.SubString( 0, value.GetLength() - (value.GetLength() - excessIndex) );
							break;
						}
					}

					// We're adding another line so add newline to str
					str += PSX_String( "\n" );
				}

				m_strings[id] = str;
			}

			// Vectors
			else if ( keyword.Find( PSX_String("Vector") ) != PSX_NPOS )
			{
				FLOAT vec[4];
				SIZE_T start;
				SIZE_T end = 0;
				INT i = 0;

				for ( ; i < 4; ++i )
				{
					start = value.FindFirstNotOf( PSX_String( " \t)" ), end );

					if ( start == PSX_NPOS )
						break;

					if ( i != 0 && value[start] != PSX_String(',') )
					{
						PSX_PushError( "PulseScript: Expecting ','." );
						return EErrorCode::SCRIPT;
					}

					start = value.FindFirstNotOf( PSX_String(" \t"), start + 1 );

					if ( start == PSX_NPOS && value.FindFirstOf( PSX_String(','), end ) != PSX_NPOS )
					{
						PSX_PushError( "PulseScript: Vector value ending expecting ')'." );
						return EErrorCode::SCRIPT;
					}

					end = value.FindFirstOf( PSX_String(" \t,)"), start );
					if ( end == PSX_NPOS )
						break;

					String val = value.SubString( start, end - start );

					// Remove f at the end of the value if found
					if ( val[val.GetLength() - 1] == PSX_String('f') )
						val[val.GetLength() - 1] = PSX_NULL;

					if ( !PSX_StringToFLOAT( val, vec[i] ) )
					{
						PSX_PushError( "PulseScript: Excpecting value [float]." );
						return EErrorCode::SCRIPT;
					}
				}

				switch( i )
				{
				case 2:
					m_vec2s[id] = Vector2( vec[0], vec[1] );
					break;
				case 3:
					m_vec3s[id] = Vector3( vec[0], vec[1], vec[2] );
					break;
				case 4:
					m_vec4s[id] = Vector4( vec[0], vec[1], vec[2], vec[3] );
					break;
				default:
					PSX_PushError( "PulseScript: Reading in unknown vector type." );
					return EErrorCode::SCRIPT;
				}
				
			}

			// Float
			else if ( keyword == PSX_String( "Float" ) )
			{
				FLOAT floatVal;

				if ( PSX_StringToFLOAT( value, floatVal ) )
					m_floats[id] = floatVal;
				else
				{
					PSX_PushError( "PulseScript: Unknown value type. Value is not int or a float value type." );
					return EErrorCode::SCRIPT;
				}
			}

			// Int
			else if ( keyword == PSX_String("Int" ) )
			{
				INT intVal;

				if ( PSX_StringToINT( value, intVal ) )
					m_ints[id] = intVal;
				else
				{
					if ( value.Find( PSX_String('.') ) == PSX_NPOS )
					{
						PSX_PushError( "PulseScript: Unknown value type. Value is not int or a float value type." );
						return EErrorCode::SCRIPT;
					}
				}
			}

			// Unknown keyword data type
			else
			{
				PSX_PushError( "PulseScript: Unknow or unsupported data type." );
				return EErrorCode::SCRIPT;
			}
		}
		else
		{
			String newKeyword;
			String var;
			SIZE_T arraySize = 0;
			SIZE_T start;
			SIZE_T end;
			EErrorCode::Type err = EErrorCode::NONE;

			// Extract arraySize and strip it out. Take note that it is expected that we have '[' and ']' from
			//	the check above
			start = keyword.FindFirstOf( PSX_String('[') ) + 1;
			end = keyword.FindFirstOf( PSX_String(']'), start );

			if ( start != end && !PSX_StringToUINT( keyword.SubString( start, end - start ), arraySize ) )
			{
				PSX_PushError( "PulseScript: Invalid array size." );
				return EErrorCode::SCRIPT;
			}

			newKeyword = keyword.SubString( 0, start - 1 );
			
			// Int and Float arrays
			BOOL bIsFloatArray = newKeyword == PSX_String("Float");
			BOOL bIsIntArray = newKeyword == PSX_String("Int");
			if ( bIsFloatArray || bIsIntArray )
			{
				IntArray *pIntArr		= PSX_NULL;
				FloatArray *pFloatArr	= PSX_NULL;

				if ( bIsFloatArray )
					pFloatArr = &m_floatArrays[id];
				else
					pIntArr = &m_intArrays[id];

				// Two versions, the script could explicity declare the size or PulseScript could figure it out)Not implemented yet
				if ( arraySize )
				{
					start = end = 0;

					if ( bIsFloatArray )
						pFloatArr->Resize( arraySize );
					else
						pIntArr->Resize( arraySize );

					// Read in each value
					for ( SIZE_T i = 0; i < arraySize; ++i )
					{
						//value
						start = value.FindFirstNotOf( PSX_String(" \t,{"), end );

						if ( start == PSX_NPOS )
						{
							PSX_PushError( "PulsceScript: Unexpected value define of Int array. Expecting '}'." );
							return EErrorCode::SCRIPT;
						}

						if ( value[start] == PSX_String('\\') )
						{
							if ( (err = GetLine( pInfo, value )) != EErrorCode::NONE )
								return EErrorCode::SCRIPT;

							// We haven't read a value so revert back once and reset indices
							--i;	
							start = end = 0;
							continue;
						}

						end = value.FindFirstOf( PSX_String( " \t,}" ), start );

						if ( end == PSX_NPOS )
						{
							PSX_PushError( "PulsceScript: Not enough values defined in the array." );
							return EErrorCode::SCRIPT;
						}

						var = value.SubString( start, end - start );

						if ( bIsFloatArray )
						{
							if ( !PSX_StringToFLOAT( var, (*pFloatArr)[i] ) )
							{
								PSX_PushError( "PulseScript: None int value found." );
								return EErrorCode::SCRIPT;
							}
						}
						else
						{
							if ( !PSX_StringToINT( var, (*pIntArr)[i] ) )
							{
								PSX_PushError( "PulseScript: None int value found." );
								return EErrorCode::SCRIPT;
							}
						}
					}

					// Once done we have to make sure it closes it with a '}' and ';'
					while( true )
					{
						start = value.FindFirstOf( PSX_String('}'), end );
						if ( start == PSX_NPOS )
						{
							if ( (err = GetLine( pInfo, value )) != EErrorCode::NONE )
								return err;

							start = end = 0;
							continue;
						}

						end = value.FindFirstNotOf( PSX_String(" \t}"), start );

						if ( !bIgnoreRedundantCases || value[end] == PSX_String(';') )
							break;

						PSX_PushError( "PulseScript: Unexpected ending of array definition. Expecting ';'" );
						return EErrorCode::SCRIPT;
					}
				}
			}

			// Vector arrays
			else if ( newKeyword.FindFirstOf( PSX_String( "Vector" ) ) != PSX_NPOS )
			{
				FLOAT vec[4];
				SIZE_T vectorType;
				void *pArr; // Generic pointer to any vector array 

				if ( !PSX_StringToUINT( newKeyword.SubString( 6, 1), vectorType ) )
				{
					PSX_PushError( "PulseScript: Unidentified vector type." );
					return EErrorCode::SCRIPT;
				}

				// Two versions, the script could explicity declare the size or PulseScript could figure it out)Not implemented yet
				if ( arraySize )
				{
					switch ( vectorType )
					{
					case 2:
						pArr = (void*)(&m_vec2Arrays[id]);
						((Vector2Array*)pArr)->Resize(arraySize);
						break;
					case 3:
						pArr = (void*)(&m_vec3Arrays[id]);
						((Vector3Array*)pArr)->Resize(arraySize);
						break;
					case 4:
						pArr = (void*)(&m_vec4Arrays[id]);
						((Vector4Array*)pArr)->Resize(arraySize);
						break;
					default:
						pArr = PSX_NULL;
						PSX_PushError( "PulseScript: Unknown vector type." );
						return EErrorCode::SCRIPT;
					}

					start = end = 0;

					for ( SIZE_T i = 0; i < arraySize; ++i )
					{
						if ( end != 0 )
							++end;

						end = value.FindFirstNotOf( PSX_String(" \t,{"), end );

						if ( end == PSX_NPOS )
						{
							PSX_PushError( "PulseScript: Unexpected end of array definition." );
							return EErrorCode::SCRIPT;
						}

						if ( end == (value.GetLength() - 1) && value[end] == PSX_String('\\') )
						{
							if ( (err = GetLine( pInfo, value )) != EErrorCode::NONE )
								return err;

							start = end = 0;
							--i;
							continue;
						}

						for ( SIZE_T j = 0; j < 4; ++j )
						{
							start = value.FindFirstNotOf( PSX_String( " \t" ), end );

							if ( start != 0 && (start == PSX_NPOS || value[start] == PSX_String(')')) )
								break;

							if ( j != 0 && value[start] != PSX_String(',') )
							{
								PSX_PushError( "PulseScript: Expecting ','." );
								return EErrorCode::SCRIPT;
							}

							start = value.FindFirstNotOf( PSX_String(" \t"), start + 1 );

							if ( start == PSX_NPOS && value.FindFirstOf( PSX_String(','), end ) != PSX_NPOS )
							{
								PSX_PushError( "PulseScript: Vector value ending expecting ')'." );
								return EErrorCode::SCRIPT;
							}

							end = value.FindFirstOf( PSX_String(" \t,)"), start );
							if ( end == PSX_NPOS )
								break;

							// Check if we're still reading in values for the current vector 
							//	and it's over the vector type
							if ( j >= vectorType )
							{
								PSX_PushError( "PulseScript: Defining mroe values than the vector type should have." );
								return EErrorCode::SCRIPT;
							}

							String val = value.SubString( start, end - start );

							// Remove f at the end of the value if found
							if ( val[val.GetLength() - 1] == PSX_String('f') )
								val[val.GetLength() - 1] = PSX_NULL;

							if ( !PSX_StringToFLOAT( val, vec[i] ) )
							{
								PSX_PushError( "PulseScript: Excpecting value [float]." );
								return EErrorCode::SCRIPT;
							}
						}

						// Now store in the values
						switch ( vectorType )
						{
						case 2:
							((Vector2Array*)pArr)->operator [](i) = Vector2( vec[0], vec[1] );
							break;
						case 3:
							((Vector3Array*)pArr)->operator [](i) = Vector3( vec[0], vec[1], vec[2] );
							break;
						case 4:
							((Vector4Array*)pArr)->operator [](i) = Vector4( vec[0], vec[1], vec[2], vec[3] );
							break;
						default:
							pArr = PSX_NULL;
							PSX_PushError( "PulseScript: Unknown vector type." );
							return EErrorCode::SCRIPT;
						}
					}

					// Once done we have to make sure it closes it with a '}' and ';'
					while( true )
					{
						start = value.FindFirstOf( PSX_String('}'), end );
						if ( start == PSX_NPOS )
						{
							if ( (err = GetLine( pInfo, value )) != EErrorCode::NONE )
								return err;

							start = end = 0;
							continue;
						}

						end = value.FindFirstNotOf( PSX_String(" \t}"), start );

						if ( !bIgnoreRedundantCases || value[end] == PSX_String(';') )
							break;

						PSX_PushError( "PulseScript: Unexpected ending of array definition. Expecting ';'" );
						return EErrorCode::SCRIPT;
					}

				}
				else
				{
					PSX_PushError("PulseScript: Implicit declaration of array size is not implemented." );
					return EErrorCode::SCRIPT;
				}

			}

			else
			{
				PSX_PushError( "PulseScript: Unknown or unsupported data array type." );
				return EErrorCode::SCRIPT;
			}
		}

		return EErrorCode::NONE;
	}

	PulseScript::PulseScript( void )
		: m_pGlobal( PSX_NULL )
	{

	}

	PulseScript::PulseScript( const String &filename )
		: m_pGlobal( PSX_NULL ), m_name( filename )
	{
		LoadScript( filename );
	}

	PulseScript::~PulseScript( void )
	{
		UnloadScript();
	}

	EErrorCode::Type PulseScript::LoadScript( const String &filename )
	{
		UnloadScript();

		FileIO file( filename.GetBuffer(), FileIO::FILEOP_READ );

		if ( file.IsOpen() )
			m_pGlobal = new Section( file, filename );

		if ( IsValid() )
			return EErrorCode::NONE;
		else
			return EErrorCode::SCRIPT;
	}

	void PulseScript::UnloadScript( void )
	{
		PSX_SafeDelete( m_pGlobal );
	}

	BOOL PulseScript::Section::IsAlphaNumeric( const CHAR c )
	{
		return ( c >= PSX_String('a') && c <= PSX_String('z') ) ||
			( c >= PSX_String('A') && c <= PSX_String('Z') ) ||
			( c >= PSX_String('0') && c <= PSX_String('9') ) ||
			c == PSX_String( '_' );
	}

}