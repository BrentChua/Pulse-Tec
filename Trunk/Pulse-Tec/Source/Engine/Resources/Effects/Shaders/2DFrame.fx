#include "Resources/Shaders/Include.sh"

VSOutput2DFrame MainVS( VSInput2DFrame In )
{
	VSOutput2DFrame OUT;

	float4 v = float4( In.position.x * (gSize2D.x * 0.5f) * gScale,
		               In.position.y * (gSize2D.y * 0.5f) * gScale,
					   In.position.z,
					   1.0f );

    OUT.hposition = mul( v, gMatWorldViewProj2D );
    //OUT.hposition = v;
    OUT.color0    = In.color0;
    OUT.pos0 = float3( v.x + gMatWorld._41, v.y + gMatWorld._42, v.z + gMatWorld._43 );

    return OUT;
}

PS_OUTPUT MainPS( PSInput2DFrame In )
{
	PS_OUTPUT OUT;

	//OUT.color0 = tex2D( gTexture1, In.texcoord0 ) * IN.color0; // Modulate texel color with vertex color
	//OUT.color0 = float4( In.texcoord0, 1.0f, 1.0f );
	
	//OUT.color0 = tex2D( gTexture1, in.texcoord0 ); // Add texel color to vertex color
	//OUT.color0 = float4( In.color0.x, In.color0.y, In.color0.z, 1.0f );
	
	//float glass = saturate(In.color0.x + sin( (In.pos0.y + In.pos0.x) / 100.0f));
	//float glass = (In.color0.x + sin( (In.pos0.y + In.pos0.x) / 50.0f));

	//OUT.color0 = float4( In.pos0, 1.0f );
	//OUT.color0 = float4(In.pos0, 1.0f );
	//OUT.color0 = float4(1.0f, 1.0f, 1.0f, 1.0f );
	
	float diagonalControl = 0.5f;
	float repetition = 0.02f;
	float xplusy = In.pos0.x + In.pos0.y;
	float reflectance = 0.50f;
	float glass = saturate(sin( (In.pos0.y * diagonalControl + In.pos0.x) * repetition)) * reflectance;
	
	OUT.color0 = float4( gFrameColor.xyz + glass , 1.0f );
	OUT.color0.a = gAlpha2D;

	return OUT;
}
