#if 0
#include "CubeDemo.h"

#if defined( PSX_PLATFORM_WINDOWS_CONSOLE )

#include <iostream>
#include <string>

using std::cout;
using std::endl;

class test
{
public:
	test( void ) { cout << "hello test." << endl; }
};

// NOTE: Callback that uses FILTER_TYPE_ZLIB_TEST compression/decompresson on all files
FileSystem::FILTER_TYPE SelectFilter( const Pulse::CHAR *pFileName, const Pulse::CHAR *pFilePath )
{
	// Use deflate on all files
	return FileSystem::FILTER_TYPE_ZLIB_TEST;
}

class TestBase
{
public:

	void Release( void )
	{
		OnRelease();
		delete this;
	}

	virtual void OnRelease( void ) = 0;

	virtual ~TestBase( void ) {

		cout << "BaseDerived destructor" << endl;

	}
};

class TestDerived : public TestBase
{
public:

	virtual void OnRelease( void )
	{
		int x = 0;
	}

	virtual ~TestDerived( void ) {
		cout << "TestDerived destructor" << endl;
	}
};


Pulse::INT SampleConsole::Main( Pulse::INT argNum, Pulse::CHAR **ppArgs )
{
	((TestBase*)(new TestDerived))->Release();

	HIDManager::GetInstance();

	 //#define SAMPLE_RESOURCE_CACHE
	 #ifdef SAMPLE_RESOURCE_CACHE
	ResourceCache resCache( 10 );

	ResourceName samplePic("SampleResources/Galaxy Music Radio.png");
	HResource *pHRes = resCache.Load( &samplePic );

	if ( pHRes->IsValid() )
	{
		FileIO fWrite("Poop.png", FileIO::FILEOP_WRITE | FileIO::FILEOP_BINARY );
		if ( fWrite.IsOpen() )
		{
			if ( fWrite.Write( pHRes->GetBuffer(), pHRes->GetSize() ) )
			{
				fWrite.Close();
				cout << "File succesfully written." << endl;
			}
			else
				cout << "Failed to write file." << endl;
		}

		pHRes->Release();
	}
	 #endif /*SAMPLE_RESOURCE_CACHE*/

//#define SAMPLE_PACKED_ALLOCATOR
#ifdef SAMPLE_PACKED_ALLOCATOR
	PackedAllocator PA( PSX_MEGABYTE );

	do
	{
		PackedAllocator::HPointer< Pulse::INT >		hPtr1	= PA.Alloc(sizeof(Pulse::INT));
		PackedAllocator::HPointer< Pulse::FLOAT >	hPtr2	= PA.Alloc(sizeof(Pulse::FLOAT));
		PackedAllocator::HPointer< Pulse::DOUBLE >	hPtr3	= PA.Alloc(sizeof(Pulse::DOUBLE));
		PackedAllocator::HPointer< Pulse::String >	hPtr4	= PA.Alloc(sizeof(Pulse::String));
		PackedAllocator::HPointer< Pulse::INT >		hPtr5	= PA.Alloc(sizeof(Pulse::INT));
		PackedAllocator::HPointer< Pulse::INT >		hPtr6	= PA.Alloc(sizeof(Pulse::INT));

		new (hPtr4.GetTempPtr()) String();

		*hPtr1.GetTempPtr() = 5;
		*hPtr2.GetTempPtr() = 3.15128f;
		*hPtr3.GetTempPtr() = 69.696969;
		*hPtr4.GetTempPtr() = PSX_String("Hello World");
		*hPtr5.GetTempPtr() = 1;
		*hPtr6.GetTempPtr() = 2;

		cout << "Original memory content." << endl;

		cout << *hPtr1.GetTempPtr() << endl;
		cout << *hPtr2.GetTempPtr() << endl;
		cout << *hPtr3.GetTempPtr() << endl;
		cout << hPtr4.GetTempPtr()->GetCString() << endl;
		cout << *hPtr5.GetTempPtr() << endl;
		cout << *hPtr6.GetTempPtr() << endl;

		cout << endl;

		PA.Free( hPtr1 );

		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );
		PA.Defrag( 1 );

		cout << "Defraged memory content." << endl;

		//cout << *hPtr1.GetTempPtr() << endl;
		cout << *hPtr2.GetTempPtr() << endl;
		cout << *hPtr3.GetTempPtr() << endl;
		cout << hPtr4.GetTempPtr()->GetCString() << endl;
		cout << *hPtr5.GetTempPtr() << endl;
		cout << *hPtr6.GetTempPtr() << endl;

		hPtr4.GetTempPtr()->~String();

		PA.Free( hPtr2 );
		PA.Free( hPtr3 );
		PA.Free( hPtr4 );
		PA.Free( hPtr5 );
		PA.Free( hPtr6 );

	} while ( true );

#endif /*SAMPLE_PACKED_ALLOCATOR*/


	#define PFS_ACTION 0
#if PFS_ACTION != 0
	FileSystem pulseFile;

	#if (PFS_ACTION == 1) // NOTE: Pack a specified directory
		pulseFile.Create( "C:\\Program Files\\Adobe", "C:\\New Folder\\TestPAK.pfs", 
			SelectFilter, FileSystem::FILTER_TYPE_ZLIB_TEST );
	#elif (PFS_ACTION == 2) // NOTE: Extract all the contenst of a pack file
		pulseFile.Unpack( "C:\\New Folder\\TestPAK.pfs", "C:\\New Folder\\Extracted Data\\" );
	#elif ( PFS_ACTION == 3) // NOTE: Extract a file then save on the disk from a pack file
		if ( pulseFile.Open( PSX_String("C:\\New Folder\\TestPAK.pfs") ) )
		{
			cout << "Successfully opened pulse files." << endl;
			FileSystem::OHFile ohFile = pulseFile.FindFile( "Sample Pictures\\Sample Music\\Kalimba.mp3" );
			if ( ohFile.IsValid() )
			{
				Pulse::SIZE_T size = pulseFile.GetFileSize( &ohFile );
				Pulse::BYTE *pData = new Pulse::BYTE [ size ];
				pulseFile.ReadFile( &ohFile, pData );
				FileIO file( "C:\\music.mp3", FileIO::FILEOP_BINARY | FileIO::FILEOP_WRITE );
				file.Write( pData, size );
				file.Close();
				cout << "File found." << endl;
			}
		}
	#endif
#endif /*PFS_ACTION != 0*/
		return 0;
}

#else

////////////////////////////

IKeyboardDevice *pKB;
IMouseDevice *pMouse;

Pulse::BOOL SampleWindow::OnInitialize()
{
	ShowFPS( TRUE );

	return TRUE;
}

Pulse::BOOL SampleWindow::OnStart()
{
	//procMan.AddProcess( Process::ProcessPointer(new ProcessWait( 5.0f )) );


	// Init D3D
	m_GD.Initialize();

	GRAPHICS_DEVICE_DESC desc;
	desc.mode.width = 800;
	desc.mode.height = 600;
	desc.mode.refreshRate = 60;
	desc.windowed = TRUE;
	m_GD.FindClosestDisplayMode( &desc.mode, &desc.mode );
	m_GD.CreateDevice( &desc );

	m_pDevice = m_GD.m_pDevice;
	D3DX10CreateFont( m_GD.m_pDevice, 25, 10, 10, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman", &pFont );
	
	// Create DInput devices
	pKB = (IKeyboardDevice*)m_pInputManager->GetDeviceInterface( HIDManager::HID_KEYBOARD, 
		HIDManager::FLAG_FOREGROUND | HIDManager::FLAG_NONEXCLUSIVE );

	pMouse = (IMouseDevice*)m_pInputManager->GetDeviceInterface( HIDManager::HID_MOUSE,
		HIDManager::FLAG_FOREGROUND | HIDManager::FLAG_NONEXCLUSIVE );

	// Init Box
	Vertex pVertices[] = {
		{ Vector3( -1.0f, -1.0f, -1.0f ), WHITE },
		{ Vector3( -1.0f, +1.0f, -1.0f ), RED },
		{ Vector3( +1.0f, +1.0f, -1.0f ), BLACK },
		{ Vector3( +1.0f, -1.0f, -1.0f ), BLUE },

		{ Vector3( -1.0f, -1.0f, +1.0f ), BLUE },
		{ Vector3( -1.0f, +1.0f, +1.0f ), MAGENTA },
		{ Vector3( +1.0f, +1.0f, +1.0f ), YELLOW },
		{ Vector3( +1.0f, -1.0f, +1.0f ), CYAN },
	};
	m_numVertices = sizeof(pVertices) / sizeof(Vertex);

	for ( Pulse::SIZE_T i = 0; i < m_numVertices; ++i )
	{
		//pVertices[i].pos *= 0.5f;
	}


	D3D10_BUFFER_DESC vbd;
	vbd.Usage			= D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth		= sizeof( Vertex ) * m_numVertices;
	vbd.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags	= 0;
	vbd.MiscFlags		= 0;

	D3D10_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem			= pVertices;
	vInitData.SysMemPitch		= 0;
	vInitData.SysMemSlicePitch	= 0;

	HRESULT hr;
	hr = m_pDevice->CreateBuffer( &vbd, &vInitData, &m_pVB );
	PSX_Assert( !FAILED( hr ), "Failed to create vertex buffer." );

	Pulse::DWORD pIndices[] = {
		// Front
		0, 1, 2,
		0, 2, 3,

		// Back
		4, 6, 5,
		4, 7, 6,

		// Left
		4, 5, 1,
		4, 1, 0,

		// Right
		3, 2, 6,
		3, 6, 7,

		// Top
		1, 5, 6,
		1, 6, 2,

		// Bottom
		4, 0, 3,
		4, 3, 7
	};

	m_numIndices = sizeof( pIndices ) / sizeof( Pulse::DWORD );

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(Pulse::DWORD) * m_numIndices;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = pIndices;
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	hr = m_pDevice->CreateBuffer( &ibd, &iInitData, &m_pIB );
	PSX_Assert( !FAILED( hr ), "Failed to create index buffer." );

	Pulse::DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	//shaderFlags |= D3D10_SHADER_DEBUG;
	//shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;

	ID3D10Blob *pCompileErrors;
	hr = D3DX10CreateEffectFromFile( "color.fx", NULL, NULL, "fx_4_0", shaderFlags, 0, m_pDevice, NULL, NULL, &m_pFX, &pCompileErrors, NULL );
	if ( FAILED( hr ) )
	{
		MessageBox( NULL, (Char*)pCompileErrors->GetBufferPointer(), "Shader compile error.", MB_OK );
	}

	m_pTech		= m_pFX->GetTechniqueByName( "ColorTech" );
	m_pVarWVP	= m_pFX->GetVariableByName( "gWVP" )->AsMatrix();

	// Build vertex layout
	D3D10_INPUT_ELEMENT_DESC vertexDesc [] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA , 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA , 0 },
	};

	// Create Input layout
	D3D10_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc( &passDesc );
	m_pDevice->CreateInputLayout( vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pVertexLayout );

	m_theta = 0.0f;
	m_phi = PSX_PI*0.25f;

	float aspect = (float)GetWidth() / (float)GetHeight();
	//D3DXMatrixPerspectiveFovLH( &m_matProj, 0.25f * PSX_PI, aspect, 1.0f, 1000.0f );
	m_matProj.BuildPerspectiveProjection( 0.25f * PSX_PI, aspect, 1.0f, 1000.0f );

	return TRUE;
}

void SampleWindow::OnUpdate( Pulse::DOUBLE time, Pulse::FLOAT dt, void *pContext )
{
	if ( pKB->IsKeyDown( PIK_RALT ) && pKB->IsBufferedKeyDown( PIK_RETURN ) )
		ToggleFullscreen();

	//if ( pKB->IsBufferedKeyDown( PIK_SPACE ) )
	//	procMan.AddProcess( Process::ProcessPointer( new ProcessWait( 2.0f ) ));

	if( pKB->IsBufferedKeyDown( PIK_K ) )
		int x = 0;

	if ( pMouse->IsButtonDown( PIM_LBUTTON ) )
		int x = 0;
	else if ( pMouse->IsButtonDown( PIM_MBUTTON ) )
		int x = 0;
	else if ( pMouse->IsButtonDown( PIM_RBUTTON ) )
		int x = 0;

	if ( pKB->IsKeyDown( PIK_A ) )
		m_theta -= 2.0f * dt;
	if ( pKB->IsKeyDown( PIK_D ) )
		m_theta += 2.0f * dt;
	if ( pKB->IsKeyDown( PIK_W ) )
		m_phi -= 2.0f * dt;
	if ( pKB->IsKeyDown( PIK_S ) )
		m_phi += 2.0f * dt;

	if ( m_phi < 0.1f ) m_phi = 0.1f;
	if ( m_phi > PSX_PI - 0.1f ) m_phi = PSX_PI - 0.1f;

	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.
	float x =  5.0f*sinf(m_phi)*sinf(m_theta);
	float z = -5.0f*sinf(m_phi)*cosf(m_theta);
	float y =  5.0f*cosf(m_phi);

	// Build view matrix
	Vector3 pos( x, y, z );
	Vector3 target( 0.0f, 0.0f, 0.0f );
	Vector3 up( 0.0f, 1.0f, 0.0f );
	// D3DXMatrixLookAtLH( &m_matView, &pos, &target, &up );
	m_matView.BuildLookAt( pos, target, up );

	procMan.Update( dt );
}

void SampleWindow::OnRender( void )
{
	Color color((Pulse::UINT)0);
	m_GD.m_pDevice->ClearRenderTargetView( m_GD.m_pRenderTargetView, color );
	m_GD.m_pDevice->ClearDepthStencilView( m_GD.m_pDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

	RECT r = { 0, 0, 0, 0 };
	pFont->DrawText( NULL, "HELLO CUBE!!!", -1, &r, DT_NOCLIP, *((D3DXCOLOR*)&RED) );

	m_pDevice->OMSetDepthStencilState( 0, 0 );
	Pulse::FLOAT blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDevice->OMSetBlendState( NULL, blendFactors, 0xffffffff );

	m_pDevice->IASetInputLayout( m_pVertexLayout );
	m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Set constants
	m_matWVP = m_matView * m_matProj;
	m_pVarWVP->SetMatrix( (float*)&m_matWVP._11 );

	D3D10_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc( &techDesc );

	for ( Pulse::SIZE_T i = 0; i < techDesc.Passes; ++i )
	{
		m_pTech->GetPassByIndex( i )->Apply( 0 );

		Pulse::UINT stride = sizeof(Vertex);
		Pulse::UINT offset = 0;
		m_pDevice->IASetVertexBuffers( 0, 1, &m_pVB, &stride,  &offset );
		m_pDevice->IASetIndexBuffer( m_pIB, DXGI_FORMAT_R32_UINT, 0 );
		m_pDevice->DrawIndexed( m_numIndices, 0, 0 );
	}

	m_GD.m_pSwapchain->Present( 0, 0 );
}

void SampleWindow::OnShutDown( void )
{
	pFont->Release();
	pKB->Release();
}

#endif /* defined( PSX_PLATFORM_WINDOWS_CONSOLE ) */
#endif