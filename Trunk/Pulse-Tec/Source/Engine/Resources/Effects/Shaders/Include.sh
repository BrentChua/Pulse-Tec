// Textures
sampler gTexture1;

// Common
float gScale = 1.0f;

typedef struct VS_OUTPUT
{
	float4 hposition : POSITION;
	float4 color0    : COLOR0;
	float2 texcoord0 : TEXCOORD0;
} PS_INPUT;

typedef struct VSOutputQuad
{
	float4 hPosition : POSITION;
	float2 texCoord0 : TEXCOORD0;
} PSInputQuad;

typedef struct VSOutput2DFrame
{
	float4 hposition : POSITION;
	float4 color0    : COLOR0;
	float3 pos0		 : TEXCOORD0;
} PSInput2DFrame;

typedef struct VSOutput2DLine
{
	float4 hPosition : POSITION;
	float4 color0	 : COLOR0;
} PSInput2DLine;

// 2D
float2 gViewportSizeInv;
float4 gFrameColor = float4( 0.87f, 0.87f, 0.87f, 1.0f );
float gAlpha2D;
float4x4 gMatWorldViewProj2D;
float2 gSize2D = float2( 1.0f, 1.0f );
float4 gTextureRect = float4( 0.0f, 0.0f, 1.0f, 1.0f ); // xy are offset. zw are width and height
float4 gFontColor = float4( 1.0f, 1.0f, 1.0f, 1.0f );

struct PS_OUTPUT
{
	float4 color0 : COLOR0;
};

// 3D
float4x4 gMatWorld;
float4x4 gMatView;
float4x4 gMatProj;
float4x4 gMatWorldViewProj;

struct VS_INPUT
{
	float3 position  : POSITION;
	float4 color0    : COLOR0;
	float2 texcoord0 : TEXCOORD0;
};

struct VSInputQuad
{
	float3 position  : POSITION;
};

typedef struct VSInput2DFrame
{
	float3 position  : POSITION;
	float4 color0    : COLOR0;
} VSInput2DLine;