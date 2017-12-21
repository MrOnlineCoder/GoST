//GoST

#include "stdafx.h"

gtDriverD3D11::gtDriverD3D11( gtMainSystem* System, gtDriverInfo params ):
	m_beginRender( false ),
	m_system( System ),
	m_D3DLibrary( nullptr ),
	m_SwapChain( nullptr ),
	m_d3d11Device( nullptr ),
	m_d3d11DevCon( nullptr ),
	m_MainTargetView( nullptr ),
	m_depthStencilBuffer( nullptr ),
	m_depthStencilStateEnabled( nullptr ),
	m_depthStencilStateDisabled( nullptr ),
	m_depthStencilView( nullptr ),
	m_RasterizerSolid( nullptr ),
	m_RasterizerSolidNoBackFaceCulling( nullptr ),
	m_RasterizerWireframe( nullptr ),
	m_RasterizerWireframeNoBackFaceCulling( nullptr ),
	m_shader2DStandart( nullptr )
{
	m_params =  params;
	
	if( params.m_outWindow ){
		m_currentWindowSize[ 0u ] = params.m_outWindow->getRect().getComponent( 2u );
		m_currentWindowSize[ 1u ] = params.m_outWindow->getRect().getComponent( 3u );
	}

#ifdef GT_DEBUG
	this->setDebugName( u"DriverD3D11" );
#endif
}


gtDriverD3D11::~gtDriverD3D11( void ){
	
	if( m_shader2DStandart )
		m_shader2DStandart->release();

	if( m_RasterizerWireframeNoBackFaceCulling )
		m_RasterizerWireframeNoBackFaceCulling->Release();

	if( m_RasterizerWireframe )
		m_RasterizerWireframe->Release();

	if( m_RasterizerSolidNoBackFaceCulling )
		m_RasterizerSolidNoBackFaceCulling->Release();

	if( m_RasterizerSolid )
		m_RasterizerSolid->Release();

	if( m_depthStencilView )
		m_depthStencilView->Release();

	if( m_depthStencilStateDisabled )
		m_depthStencilStateDisabled->Release();

	if( m_depthStencilStateEnabled )
		m_depthStencilStateEnabled->Release();

	if( m_depthStencilBuffer )
		m_depthStencilBuffer->Release();

	if( m_MainTargetView )
		m_MainTargetView->Release();

	if( m_d3d11DevCon )
		m_d3d11DevCon->Release();

	if( m_SwapChain )
		m_SwapChain->Release();
	
	if( m_d3d11Device )
		m_d3d11Device->Release();

	if( m_D3DLibrary )
		FreeLibrary( m_D3DLibrary );

}

HMODULE gtDriverD3D11::getD3DLibraryHandle( void ){
	return m_D3DLibrary;
}

ID3D11Device * gtDriverD3D11::getD3DDevice( void ){
	return m_d3d11Device;
}

void gtDriverD3D11::setActiveShader( gtShader* shader ){
	m_d3d11DevCon->IASetInputLayout( ((gtShaderImpl*)shader)->m_vLayout );
	m_d3d11DevCon->VSSetShader( ((gtShaderImpl*)shader)->m_vShader, 0, 0 );
	m_d3d11DevCon->PSSetShader( ((gtShaderImpl*)shader)->m_pShader, 0, 0 );
}

const gtDriverInfo&	gtDriverD3D11::getParams( void ) const {
		return  m_params;
}

bool gtDriverD3D11::initialize( void ){

	if( !m_params.m_outWindow ){
		gtLogWriter::printError( u"No render out window." );
		return false;
	}

	HWND outWindow = (HWND)m_params.m_outWindow->getHandle();

	//	Describes a display mode.
	DXGI_MODE_DESC	bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	//	A value that describes the resolution width
	bufferDesc.Width	=	m_params.m_backBufferSize[0u];
	//	A value describing the resolution height
	bufferDesc.Height	=	m_params.m_backBufferSize[1u];
	//	refresh rate in hertz
	if( m_params.m_vSync )
		bufferDesc.RefreshRate.Numerator	=	60;
	else bufferDesc.RefreshRate.Numerator	=	0;
	bufferDesc.RefreshRate.Denominator	=	1;
	//	display format
	bufferDesc.Format	=	DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering	=	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling	=	DXGI_MODE_SCALING_UNSPECIFIED;
	
	DXGI_SWAP_CHAIN_DESC	swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferDesc	=	bufferDesc;
	swapChainDesc.BufferUsage	=	DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow	=	outWindow;
	swapChainDesc.SampleDesc.Count	=	1;
	swapChainDesc.SampleDesc.Quality	=	0;
	swapChainDesc.BufferCount	=	1;
	swapChainDesc.Windowed	=	true;
	swapChainDesc.SwapEffect	=	DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags	=	0;

	gtString d3dlib_str = gtFileSystem::getSystemPath();
	d3dlib_str += u"d3d11.dll";

	m_D3DLibrary = LoadLibrary( (wchar_t*)d3dlib_str.data() );
	if( !m_D3DLibrary ){
		gtLogWriter::printError( u"Could not load d3d11.dll" );
		return false;
	}

	gtD3D11CreateDevice_t D3D11CreateDevice = (gtD3D11CreateDevice_t)GetProcAddress(m_D3DLibrary, "D3D11CreateDevice");
	if( !D3D11CreateDevice ){
		gtLogWriter::printError( u"Could not get proc adress of D3D11CreateDevice");
		return false;
	}

	gtD3D11CreateDeviceAndSwapChain_t D3D11CreateDeviceAndSwapChain =
		(gtD3D11CreateDeviceAndSwapChain_t)GetProcAddress(m_D3DLibrary, "D3D11CreateDeviceAndSwapChain");
	if( !D3D11CreateDeviceAndSwapChain ){
		gtLogWriter::printError( u"Could not get proc adress of D3D11CreateDeviceAndSwapChain");
		return false;
	}

	D3D_FEATURE_LEVEL FeatureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1
    };
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
	if( FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&FeatureLevels[0],
		ARRAYSIZE( FeatureLevels ),
		D3D11_SDK_VERSION,
		nullptr,
		&featureLevel,
		nullptr ))){
			gtLogWriter::printError( u"Can not get D3D Feature Level");
		return false;
	}

	if( featureLevel == D3D_FEATURE_LEVEL_11_0 ){
		gtLogWriter::printInfo( u"D3D feature level 11.0" );
	}else if( featureLevel == D3D_FEATURE_LEVEL_11_1 ){
		gtLogWriter::printInfo( u"D3D feature level 11.1" );
	}

	if( FAILED( D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr,
		0, 
		&featureLevel, 
		1,
		D3D11_SDK_VERSION, 
		&swapChainDesc,
		&m_SwapChain,
		&m_d3d11Device, 
		nullptr, 
		&m_d3d11DevCon ) ) ){
			gtLogWriter::printError( u"Can't create Direct3D 11 Device" );
		return false;
	}

	ID3D11Texture2D* BackBuffer;
	if( FAILED( m_SwapChain->GetBuffer( 
		0,
		__uuidof( ID3D11Texture2D ), 
		(void**)&BackBuffer ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 back buffer" );
		return false;
	}


	if( FAILED( this->m_d3d11Device->CreateRenderTargetView( 
		BackBuffer, 0, &m_MainTargetView ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 render target" );
		if( BackBuffer ) BackBuffer->Release();
		return false;
	}
	
	if( BackBuffer ) BackBuffer->Release();

	D3D11_TEXTURE2D_DESC	DSD;
	ZeroMemory( &DSD, sizeof(DSD) );
	DSD.Width	=	m_params.m_backBufferSize[0u];
	DSD.Height	=	m_params.m_backBufferSize[1u];
	DSD.MipLevels	=	1;
	DSD.ArraySize	=	1;
	DSD.Format	=	DXGI_FORMAT_D32_FLOAT;
	DSD.SampleDesc.Count	=	1;
	DSD.SampleDesc.Quality	=	0;
	DSD.Usage	=	D3D11_USAGE_DEFAULT;
	DSD.BindFlags	=	D3D11_BIND_DEPTH_STENCIL;
	DSD.CPUAccessFlags	=	0;
	DSD.MiscFlags	=	0;
	if( FAILED( m_d3d11Device->CreateTexture2D( &DSD, 0, &m_depthStencilBuffer ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil buffer" );
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof(depthStencilDesc) );
	depthStencilDesc.DepthEnable	=	true;
	depthStencilDesc.DepthWriteMask	=	D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc		=	D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable	=	true;
	depthStencilDesc.StencilReadMask=	0xFF;
	depthStencilDesc.StencilWriteMask=	0xFF;
	depthStencilDesc.FrontFace.StencilFailOp	=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp=	D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp	=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc		=	D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp		=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp=	D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp		=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc		=	D3D11_COMPARISON_ALWAYS;

	if( FAILED( m_d3d11Device->CreateDepthStencilState( &depthStencilDesc, &m_depthStencilStateEnabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil state" );
		return false;
	}

	m_d3d11DevCon->OMSetDepthStencilState( this->m_depthStencilStateEnabled, 0 );

	depthStencilDesc.DepthEnable	=	false;
	if( FAILED( m_d3d11Device->CreateDepthStencilState( &depthStencilDesc, &this->m_depthStencilStateDisabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil state" );
		return false;
	}

	ZeroMemory( &m_depthStencilViewDesc, sizeof( m_depthStencilViewDesc ) );
	m_depthStencilViewDesc.Format		=	DXGI_FORMAT_D32_FLOAT;
	m_depthStencilViewDesc.ViewDimension=	D3D11_DSV_DIMENSION_TEXTURE2D;
	m_depthStencilViewDesc.Texture2D.MipSlice	=	0;
	if( FAILED( m_d3d11Device->CreateDepthStencilView( m_depthStencilBuffer, &m_depthStencilViewDesc, &m_depthStencilView ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil view" );
		return false;
	}

	m_d3d11DevCon->OMSetRenderTargets( 1, &m_MainTargetView, m_depthStencilView );

	D3D11_RASTERIZER_DESC	rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if( FAILED( m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerSolid ))){
		gtLogWriter::printError( u"Can not create rasterizer state" );
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerSolidNoBackFaceCulling );
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerWireframeNoBackFaceCulling );
	rasterDesc.CullMode = D3D11_CULL_BACK;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerWireframe );

	m_d3d11DevCon->RSSetState( m_RasterizerSolid );

	D3D11_VIEWPORT viewport;
	viewport.Width		=	(f32)m_params.m_backBufferSize[0u];
	viewport.Height		=	(f32)m_params.m_backBufferSize[1u];
	viewport.MinDepth	=	0.0f;
	viewport.MaxDepth	=	1.0f;
	viewport.TopLeftX	=	0.0f;
	viewport.TopLeftY	=	0.0f;
	m_d3d11DevCon->RSSetViewports( 1, &viewport );

	//	в будущем стандартные шейдеры нужно убрать внутрь плагина
	gtShaderModel shaderModel;
	shaderModel.pixelShaderModel = gtShaderModel::shaderModel::_5_0;
	shaderModel.vertexShaderModel = gtShaderModel::shaderModel::_5_0;

	gtVertexType vertexType[] = 
	{
		{ gtVertexType::position },
		{ gtVertexType::end }
	};

	m_shader2DStandart = getShader( 
		u"../shaders/2d_basic.hlsl",
		"VSMain",
		u"../shaders/2d_basic.hlsl",
		"PSMain",
		shaderModel,
		vertexType
		);
	if( m_shader2DStandart ){

		//	создание константного буффера.
		if( !m_shader2DStandart->createShaderObject( 96u ) ) return false;
	}

	return true;
}

void gtDriverD3D11::clearRenderTarget( const gtColor& color ){
	m_d3d11DevCon->ClearRenderTargetView( m_MainTargetView, color.getData() );
	m_d3d11DevCon->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void gtDriverD3D11::beginRender( bool _clearRenderTarget, const gtColor& color ){
	if( !m_beginRender ){
		m_beginRender = true;

		if( _clearRenderTarget )
			clearRenderTarget( color );

	}
}

void gtDriverD3D11::endRender( void ){
	if( m_beginRender ){
		
		if( m_params.m_vSync )
			this->m_SwapChain->Present( 1, 0 );
		else
			this->m_SwapChain->Present( 0, 0 );

		m_beginRender = false;
	}
}

	//	нарисует картинку
	//	rect - координаты левого верхнего и правого нижнего углов
void gtDriverD3D11::draw2DImage( const v4i& rect, const gtMaterial& m ){
	draw2DImage( rect, v4i(), m );
}
	//	Render 2d image using region of texture
	//	нарисует картинку с выбранной областью текстуры
	//	rect - координаты левого верхнего и правого нижнего углов
	//	region - координаты левого верхнего и правого нижнего углов области картинки которую нужно нарисовать
void gtDriverD3D11::draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& m ){

	v2i center( { m_currentWindowSize[ 0u ] / 2, m_currentWindowSize[ 1u ] / 2 } );

	v4f realRect;
	realRect[ 0u ] = f32(rect[ 0u ] - center[ 0u ] ) / (f32)center[ 0u ];
	realRect[ 1u ] = (f32(rect[ 1u ] - center[ 1u ] ) * -1.f )/(f32)center[ 1u ];
	realRect[ 2u ] = (rect[ 2u ] - center[ 0u ] ) / (f32)center[ 0u ];
	realRect[ 3u ] = (f32(rect[ 3u ] - center[ 1u] ) * -1.f )/(f32)center[ 1u ];


	/*
	2-------3
	|		|
	|		|
	1-------4
	*/

	
	v2f lt, rb;

	if( v4i() == region ){ // сравнение с пустым вектором. если пустой то координаты по умолчанию.
		lt[ 0u ] = 0.f;
		lt[ 1u ] = 0.f;
		rb[ 0u ] = 1.f;
		rb[ 1u ] = 1.f;

	}else{
		GT_ASSERT2( m.textureLayer[ 0u ].texture, "texture != nullptr" );

		u32 width = m.textureLayer[ 0u ].texture->getWidth();
		u32 height = m.textureLayer[ 0u ].texture->getHeight();

		f32 mulX = 1.f / (f32)width;
		f32 mulY = 1.f / (f32)height;

		lt[ 0u ] = region[ 0u ] * mulX;
		lt[ 1u ] = region[ 1u ] * mulY;
		rb[ 0u ] = region[ 2u ] * mulX;
		rb[ 1u ] = region[ 3u ] * mulY;

	
	}

	v8f uvs;
	uvs[ 0u ] = lt[ 0u ]; // 1
	uvs[ 1u ] = rb[ 1u ];

	uvs[ 2u ] = lt[ 0u ]; // 2
	uvs[ 3u ] = lt[ 1u ];

	uvs[ 4u ] = rb[ 0u ]; // 3
	uvs[ 5u ] = lt[ 1u ];

	uvs[ 6u ] = rb[ 0u ]; // 4
	uvs[ 7u ] = rb[ 1u ];
	
	_draw2DImage( realRect, uvs, m );
}

	//	непосредственно рисует картинку
void gtDriverD3D11::_draw2DImage( const v4f& rect, const v8f& region, const gtMaterial& material ){

	gtShader * shader = material.shader;
	if( !shader ){
		shader = m_shader2DStandart;
	}

	setActiveShader( shader );

	struct cbVerts{
		v4f v1;
		v4f v2;
		v4f v3;
		v4f v4;
		v2f t1;
		v2f t2;
		v2f t3;
		v2f t4;
	}cb;

	cb.v1[ 0 ] = rect[ 0 ];	//x		
	cb.v1[ 1 ] = rect[ 3 ];	//y		
	cb.v1[ 2 ] = 0.5f;	//z		*
	cb.v1[ 3 ] = 1.f;
	cb.t1[ 0 ] = region[ 0u ];	//u
	cb.t1[ 1 ] = region[ 1u ];	//v

	cb.v2[ 0 ] = rect[ 0 ];	//x		*
	cb.v2[ 1 ] = rect[ 1 ];	//y		|
	cb.v2[ 2 ] = 0.5f;	//z			*
	cb.v2[ 3 ] = 1.f;
	cb.t2[ 0 ] = region[ 2u ];	//u
	cb.t2[ 1 ] = region[ 3u ];	//v

	cb.v3[ 0 ] = rect[ 2 ];	//x		*-----*
	cb.v3[ 1 ] = rect[ 1 ];	//y		|	/
	cb.v3[ 2 ] = 0.5f;	//z			*/
	cb.v3[ 3 ] = 1.f;
	cb.t3[ 0 ] = region[ 4u ];	//u
	cb.t3[ 1 ] = region[ 5u ];	//v

	cb.v4[ 0 ] = rect[ 2 ];	//x		*-----*
	cb.v4[ 1 ] = rect[ 3 ];	//y		|	/
	cb.v4[ 2 ] = 0.5f;		//z		*/    *
	cb.v4[ 3 ] = 1.f;
	cb.t4[ 0 ] = region[ 6u ];	//u
	cb.t4[ 1 ] = region[ 7u ];	//v

	m_d3d11DevCon->IASetInputLayout( 0 );
	m_d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_d3d11DevCon->Map(
		((gtShaderImpl*)shader)->m_constantBuffers[ 0 ],
		0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	D3D11_BUFFER_DESC d;
	((gtShaderImpl*)shader)->m_constantBuffers[ 0 ]->GetDesc( &d );

	memcpy( mappedResource.pData, &cb, d.ByteWidth );
	m_d3d11DevCon->Unmap( ((gtShaderImpl*)shader)->m_constantBuffers[ 0 ], 0 );

	for( u32 i = 0u; i < 16u; ++i ){
		if( !material.textureLayer[ i ].texture ) break;

		gtTextureD3D11* texture = (gtTextureD3D11*)material.textureLayer[ i ].texture;

		m_d3d11DevCon->PSSetShaderResources( i, 1, texture->getResourceView() );
		m_d3d11DevCon->PSSetSamplers( i, 1, texture->getSamplerState() );
	}

	m_d3d11DevCon->VSSetConstantBuffers( 0, 1, &((gtShaderImpl*)shader)->m_constantBuffers[ 0 ] );
	m_d3d11DevCon->Draw( 6, 0 );
}

	//	компилировать либо получить ранее скомпилированный шейдер
gtShader *	gtDriverD3D11::getShader( 
		//	путь к файлу хранящем вершинный шейдер
	const gtString& vertexShader,
		//	главная функция вершинного шейдера, точка входа
	const gtStringA& vertexShaderMain,
		//	путь к файлу хранящем пиксельный/фрагментный шейдер
	const gtString& pixelShader,
		//	главная функция пиксельного/фрагментного шейдера, точка входа
	const gtStringA& pixelShaderMain,
		//	тип шейдерного языка
	gtShaderModel shaderModel,

	gtVertexType * vertexType
){
	//	для хранения текста шейдера
	std::unique_ptr<s8[]> vertexBuffer;
	std::unique_ptr<s8[]> pixelBuffer;

	//	если указан файл то читаем его
	if( gtFileSystem::existFile( vertexShader ) ){

		gtFile_t file = util::openFileForReadText( vertexShader );

		u32 sz = file->size();
		
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", vertexShader.data() );
			return nullptr;
		}

		vertexBuffer.reset( new s8[ sz+1u ] );
		vertexBuffer.get()[sz] = 0;

		file->read( (u8*)vertexBuffer.get(), sz );


	}else{
		//	если указан не файл, то скорее всего текст шейдера.
		u32 sz = vertexShader.size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", vertexShader.data() );
			return nullptr;
		}

		vertexBuffer.reset( new s8[ sz+1u ] );
		vertexBuffer.get()[sz] = 0;
		auto * data = vertexShader.data();

		for( u32 i = 0u; i < sz; ++i ){
			auto * b = vertexBuffer.get();
			b[ i ] = (s8)data[ i ];
		}
	}

	if( gtFileSystem::existFile( pixelShader ) ){
		gtFile_t file = util::openFileForReadText( pixelShader );
		u32 sz = file->size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", pixelShader.data() );
			return nullptr;
		}
		pixelBuffer.reset( new s8[ sz+1 ] );
		pixelBuffer.get()[sz] = 0;

		file->read( (u8*)pixelBuffer.get(), sz );
	}else{
		u32 sz = pixelShader.size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", pixelShader.data() );
			return nullptr;
		}
		pixelBuffer.reset( new s8[ sz+1 ] );
		pixelBuffer.get()[sz] = 0;
		auto * data = pixelShader.data();
		for( u32 i = 0u; i < sz; ++i ){
			auto * b = pixelBuffer.get();
			b[ i ] = (s8)data[ i ];
		}
	}

	gtStringA v_target;

	switch( shaderModel.vertexShaderModel ){
	case gtShaderModel::shaderModel::_1_1:
	case gtShaderModel::shaderModel::_1_2:
	case gtShaderModel::shaderModel::_1_3:
	case gtShaderModel::shaderModel::_1_4:
	case gtShaderModel::shaderModel::_1_5:
		v_target = "vs_1_1";
		break;
	case gtShaderModel::shaderModel::_2_0:
		v_target = "vs_2_0";
		break;
	case gtShaderModel::shaderModel::_3_0:
	case gtShaderModel::shaderModel::_3_3:
		v_target = "vs_3_0";
		break;
	case gtShaderModel::shaderModel::_4_0:
		v_target = "vs_4_0";
		break;
	case gtShaderModel::shaderModel::_4_1:
	case gtShaderModel::shaderModel::_4_2:
	case gtShaderModel::shaderModel::_4_3:
	case gtShaderModel::shaderModel::_4_5:
		v_target = "vs_4_1";
		break;
	case gtShaderModel::shaderModel::_5_0:
		v_target = "vs_5_0";
		break;
	case gtShaderModel::shaderModel::_6_0:
		v_target = "vs_6_0";
		break;
	}

	gtStringA p_target;

	switch( shaderModel.pixelShaderModel ){
	case gtShaderModel::shaderModel::_1_1:
	case gtShaderModel::shaderModel::_1_2:
	case gtShaderModel::shaderModel::_1_3:
	case gtShaderModel::shaderModel::_1_4:
	case gtShaderModel::shaderModel::_1_5:
		p_target = "ps_1_1";
		break;
	case gtShaderModel::shaderModel::_2_0:
		p_target = "ps_2_0";
		break;
	case gtShaderModel::shaderModel::_3_0:
	case gtShaderModel::shaderModel::_3_3:
		p_target = "ps_3_0";
		break;
	case gtShaderModel::shaderModel::_4_0:
		p_target = "ps_4_0";
		break;
	case gtShaderModel::shaderModel::_4_1:
	case gtShaderModel::shaderModel::_4_2:
	case gtShaderModel::shaderModel::_4_3:
	case gtShaderModel::shaderModel::_4_5:
		p_target = "ps_4_1";
		break;
	case gtShaderModel::shaderModel::_5_0:
		p_target = "ps_5_0";
		break;
	case gtShaderModel::shaderModel::_6_0:
		p_target = "ps_6_0";
		break;
	}

	gtPtr< gtShaderImpl > shader = gtPtrNew< gtShaderImpl >( new gtShaderImpl(this) );

	if( !shader->compileShader( 
		v_target, 
		p_target,
		vertexBuffer.get(),
		pixelBuffer.get(),
		(s8*)vertexShaderMain.data(),
		(s8*)pixelShaderMain.data(),
		vertexType ) ){
		return nullptr;
	}

	shader->addRef();
	return shader.data();
}

	//	Создаёт текстуру из gtImage
gtTexture*	gtDriverD3D11::createTexture( gtImage* image ){
	GT_ASSERT2( image, "image!=nullptr" );

	gtPtr<gtTextureD3D11> texture = gtPtrNew<gtTextureD3D11>( new gtTextureD3D11( this ) );

	if( !texture->init( image ) ){
		gtLogWriter::printWarning( u"Can not init D3D11 texture" );
		return nullptr;
	}

	texture->addRef();

	return texture.data();
}

/*
Copyright (c) 2017 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/