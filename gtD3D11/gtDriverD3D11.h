﻿//GoST

#pragma once

#ifndef __GT_DRIVER_D3D11_H__
#define __GT_DRIVER_D3D11_H__

using gtD3D11CreateDevice_t = HRESULT(__stdcall*)(
	_In_opt_ IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	_In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	_Out_opt_ ID3D11Device** ppDevice,
	_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
	_Out_opt_ ID3D11DeviceContext** ppImmediateContext);

using gtD3D11CreateDeviceAndSwapChain_t = HRESULT(__stdcall*)(
	__in_opt IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	__in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	__in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	__out_opt IDXGISwapChain** ppSwapChain,
	__out_opt ID3D11Device** ppDevice,
	__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
	__out_opt ID3D11DeviceContext** ppImmediateContext );

//	D3dcompiler_47.dll
using gtD3DCompile_t = HRESULT (__stdcall*)(
	__in_bcount(SrcDataSize) LPCVOID pSrcData,
	__in SIZE_T SrcDataSize,
    __in_opt LPCSTR pSourceName,
    __in_xcount_opt(pDefines->Name != NULL) CONST D3D_SHADER_MACRO* pDefines,
    __in_opt ID3DInclude* pInclude,
    __in LPCSTR pEntrypoint,
    __in LPCSTR pTarget,
    __in UINT Flags1,
    __in UINT Flags2,
    __out ID3DBlob** ppCode,
    __out_opt ID3DBlob** ppErrorMsgs);

namespace gost{

	class gtDriverD3D11 GT_FINAL : public gtDriverCommon{

		gtMainSystem* m_system;

		HMODULE m_D3DLibrary;

		IDXGISwapChain*			m_SwapChain;
		ID3D11Device*			m_d3d11Device;
		ID3D11DeviceContext*	m_d3d11DevCon;
		ID3D11RenderTargetView* m_MainTargetView;
		ID3D11Texture2D*		m_depthStencilBuffer;
		ID3D11DepthStencilState*m_depthStencilStateEnabled;
		ID3D11DepthStencilState*m_depthStencilStateDisabled;
		D3D11_DEPTH_STENCIL_VIEW_DESC	m_depthStencilViewDesc;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState*	m_RasterizerSolid;
		ID3D11RasterizerState*	m_RasterizerSolidNoBackFaceCulling;
		ID3D11RasterizerState*	m_RasterizerWireframeNoBackFaceCulling;
		ID3D11RasterizerState*	m_RasterizerWireframe;
		ID3D11BlendState*		m_blendStateAlphaEnabled;
		ID3D11BlendState*		m_blendStateAlphaDisabled;

		void clearRenderTarget( const gtColor& );

		bool m_beginRender;

			//	стандартный шейдер для рисования 2Д элементов
		gtShader*			m_shader2DStandart;

		void	setActiveShader( gtShader* );

			//	непосредственно рисует картинку
		void	_draw2DImage( const v4f& rect, const v8f& region, const gtMaterial& );

		void	enableBlending( bool );

	public:
		gtDriverD3D11( gtMainSystem* System, gtDriverInfo params );
		~gtDriverD3D11( void );

		bool initialize( void );

		HMODULE getD3DLibraryHandle( void );
		ID3D11Device * getD3DDevice( void );


		const gtDriverInfo&	getParams( void ) const;
			//	
		void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) );
		void endRender( void );

			//	нарисует картинку
			//	rect - координаты левого верхнего и правого нижнего углов
		void draw2DImage( const v4i& rect, const gtMaterial& );

			//	Render 2d image using region of texture
			//	нарисует картинку с выбранной областью текстуры
			//	rect - координаты левого верхнего и правого нижнего углов
			//	region - координаты левого верхнего и правого нижнего углов области картинки которую нужно нарисовать
		void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& );

			//	компилировать либо получить ранее скомпилированный шейдер
		gtShader *	getShader( 
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
				//	тип вершины (должен быть массив)
			gtVertexType * vertexType
			);

			//	Создаёт текстуру из gtImage
		gtTexture*	createTexture( gtImage* );
	};

}

#endif


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