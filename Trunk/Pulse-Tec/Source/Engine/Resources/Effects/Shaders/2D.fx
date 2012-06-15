#include "Resources/Shaders/Include.sh"

VS_OUTPUT MainVS( VS_INPUT IN )
{
	VS_OUTPUT OUT;

	float4 v = float4( IN.position.x * (gSize2D.x * 0.5f) * gScale,
		               IN.position.y * (gSize2D.y * 0.5f) * gScale,
					   IN.position.z,
					   1.0f );

    OUT.hposition = mul( v, gMatWorldViewProj2D );
    //OUT.hposition = v;
    OUT.color0    = IN.color0;
    OUT.texcoord0 = IN.texcoord0;
    
    // texture render rect
    OUT.texcoord0 = float2( OUT.texcoord0.x * gTextureRect.z, OUT.texcoord0.y * gTextureRect.w );
    OUT.texcoord0 += float2( gTextureRect.x, gTextureRect.y );

    return OUT;
}

PS_OUTPUT MainPS( VS_OUTPUT IN )
{
	PS_OUTPUT OUT;

	//OUT.color0 = tex2D( gTexture1, IN.texcoord0 ) * IN.color0; // Modulate texel color with vertex color
	//OUT.color0 = float4( IN.texcoord0, 1.0f, 1.0f );
	
	OUT.color0 = tex2D( gTexture1, IN.texcoord0 ); // Add texel color to vertex color
	OUT.color0.a *= gAlpha2D;
	
	//OUT.color0 = float4( 1.0f, 1.0f, 1.0f, 1.0f ); // Debug
	
	return OUT;
}
