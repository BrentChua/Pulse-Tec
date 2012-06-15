#include "Include/Pulse.h"
#include "Include/Matrix4x4.h"
#include "Include/RandomTraverse.h"

#pragma once 
using namespace Pulse;

#if defined( PSX_PLATFORM_WINDOWS_CONSOLE )

class SampleConsole : public ConsoleApplication
{
private:

	Pulse::INT Main( Pulse::INT argNum, Pulse::CHAR **ppArgs );
};

PSX_REGISTER_APPLICATION_CREATE( SampleConsole );
PSX_REGISTER_APPLICATION_RELEASE();

#else

struct Vertex
{
	Vector3 pos;
	Color color;
};

class SampleWindow : public WindowApplication
{
private:

	virtual Pulse::BOOL OnInitialize();
	virtual Pulse::BOOL OnStart();
	virtual void OnUpdate( Pulse::DOUBLE time, Pulse::FLOAT dt, void *pContext );
	virtual void OnRender( void );
	virtual void OnShutDown( void );

	ID3DX10Font *pFont;

	ProcessManager procMan;


	/////////////////////////////////
	ID3D10Device *m_pDevice;

	ID3D10Effect *m_pFX;
	ID3D10EffectTechnique *m_pTech;
	ID3D10EffectMatrixVariable *m_pVarWVP;
	
	ID3D10InputLayout *m_pVertexLayout;

	Matrix4x4 m_matView;
	Matrix4x4 m_matProj;
	Matrix4x4 m_matWVP;

	Pulse::FLOAT m_phi;
	Pulse::FLOAT m_theta;

	// Var members for defining a box
	Pulse::SIZE_T m_numVertices;
	Pulse::SIZE_T m_numIndices;
	ID3D10Buffer *m_pVB;
	ID3D10Buffer *m_pIB;

};

PSX_REGISTER_APPLICATION_CREATE( SampleWindow );
PSX_REGISTER_APPLICATION_RELEASE();

class ProcessWait : Process
{
public:

	ProcessWait( Pulse::FLOAT seconds = 5.0f )
		: Process( 0 )
	{
		wait = seconds;
		elapsedTime = 0.0f;
	}

	void OnUpdate( Pulse::FLOAT dt )
	{
		if ( elapsedTime >= wait )
		{
			MessageBox( NULL, PSX_String("POOP!"), PSX_String("DONE"), MB_OK );
			Terminate();
		}

		elapsedTime += dt;
	}

private:

	Pulse::FLOAT elapsedTime;
	Pulse::FLOAT wait;

};

#endif /* defined( PSX_PLATFORM_WINDOWS_CONSOLE ) */