#include "Resources/Shaders/Include.sh"

VSOutput2DLine MainVS( VSInput2DFrame input )
{
	VSOutput2DLine output;

	/*
	float4 v = float4( input.position.x * (gSize2D.x * 0.5f) * gScale,
		               input.position.y * (gSize2D.y * 0.5f) * gScale,
					   input.position.z,
					   1.0f );
	*/		
			   
	float4 v = float4( input.position.x * gScale,
						input.position.y * gScale,
						input.position.z, 1.0f );

    output.hPosition = mul( v, gMatWorldViewProj2D );
    output.color0    = input.color0;

    return output;
}

PS_OUTPUT MainPS( PSInput2DLine input )
{
	PS_OUTPUT output;

	output.color0 = input.color0;
	output.color0.a *= gAlpha2D;
	
	return output;
}
