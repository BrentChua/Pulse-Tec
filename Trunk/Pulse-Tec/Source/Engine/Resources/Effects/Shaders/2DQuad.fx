#include "Resources/Shaders/Include.sh"

VSOutputQuad MainVS( VSInputQuad input )
{
	VSOutputQuad output;
	
	output.hPosition = float4( input.position.xy, 0.0f, 1.0f );
	output.texCoord0.x = 0.5f * ( 1 + input.position.x - gViewportSizeInv.x );
	output.texCoord0.y = 0.5f * ( 1 - input.position.y - gViewportSizeInv.y );
	
    return output;
}

PS_OUTPUT MainPS( PSInputQuad input )
{
	PS_OUTPUT output;

	output.color0 = tex2D( gTexture1, input.texCoord0 );

	return output;
}
