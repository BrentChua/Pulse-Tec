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

    return OUT;
}

PS_OUTPUT MainPS( VS_OUTPUT IN )
{
	PS_OUTPUT OUT;

	//OUT.color0 = tex2D( gTexture1, IN.texcoord0 ) * IN.color0; // Modulate texel color with vertex color
	//OUT.color0 = float4( IN.texcoord0, 1.0f, 1.0f );
	
	OUT.color0 = tex2D( gTexture1, IN.texcoord0 ); // Add texel color to vertex color
	OUT.color0.rgb = gFontColor.rgb;
	//OUT.color0.a *= gAlpha2D;
	
	//OUT.color0 = float4( 1.0f, 1.0f, 1.0f, 1.0f ); // Debug
	
	return OUT;
}
