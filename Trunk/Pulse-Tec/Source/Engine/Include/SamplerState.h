/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SamplerState.h
*
*	Description -	Sampler state.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/19/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_SAMPLER_STATE_H_
#define _PSX_SAMPLER_STATE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{
	struct SamplerStateDesc : public BaseResDesc
	{
		SamplerStateDesc( void );
		void SetDefault( void );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::SAMPLER_STATE; }
	
	public:

		EFilter::Type					filter;			// Filtering method to use when sampling a texture
		ETextureAddressingMode::Type	addressU;		// Method to use for resolving a u texture coordinate that is outside the 0 to 1 range
		ETextureAddressingMode::Type	addressV;		// Method to use for resolving a v texture coordinate that is outside the 0 to 1 range.
		ETextureAddressingMode::Type	addressW;		// Method to use for resolving a w texture coordinate that is outside the 0 to 1 range.
		FLOAT							mipLODBias;		// Offset from the calculated mipmap level. For example, if Direct3D calculates that a texture should be sampled at mipmap level 3 and MipLODBias is 2, then the texture will be sampled at mipmap level 5.
		UINT							maxAnisotropy;	// Clamping value used if D3D11_FILTER_ANISOTROPIC or D3D11_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values are between 1 and 16.
		EComparisonFunc::Type			comparisonFunc; // A function that compares sampled data against existing sampled data. The function options are listed in D3D11_COMPARISON_FUNC.
		FLOAT							borderColor[4]; // Border color to use if D3D11_TEXTURE_ADDRESS_BORDER is specified for AddressU, AddressV, or AddressW. Range must be between 0.0 and 1.0 inclusive.
		FLOAT							minLOD;			// Lower end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed.
		FLOAT							maxLOD;			// Upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to MinLOD. To have no upper limit on LOD set this to a large value such as D3D11_FLOAT32_MAX.
	};

	class SamplerState : public IResourcePoolItem
	{
	public:

		virtual ~SamplerState( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	
	
		void * GetNakedSamplerState( void ) { return (void*)m_pSamplerState; }

	private:

		PSX_MakeStoragePoolFriend( SamplerState );

		SamplerState( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		SamplerStateDesc m_desc;
		ID3D11SamplerState *m_pSamplerState;
	};
}

#endif /* _PSX_SAMPLER_STATE_H_ */