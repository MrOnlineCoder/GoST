//GoST

#pragma once

#ifndef __GT_DRIVER_D3D11_H__
#define __GT_DRIVER_D3D11_H__

using gtD3D11CreateDevice_t = HRESULT(__stdcall*)(_In_opt_ IDXGIAdapter* pAdapter,D3D_DRIVER_TYPE DriverType,HMODULE Software,UINT Flags,_In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,UINT FeatureLevels,UINT SDKVersion,_Out_opt_ ID3D11Device** ppDevice,_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,_Out_opt_ ID3D11DeviceContext** ppImmediateContext);
using gtD3D11CreateDeviceAndSwapChain_t = HRESULT(__stdcall*)(__in_opt IDXGIAdapter* pAdapter,D3D_DRIVER_TYPE DriverType,HMODULE Software,UINT Flags,__in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,UINT FeatureLevels,UINT SDKVersion,__in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,__out_opt IDXGISwapChain** ppSwapChain,__out_opt ID3D11Device** ppDevice,__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,__out_opt ID3D11DeviceContext** ppImmediateContext );

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
		ID3D11RasterizerState*	m_RasterizerWireframe;
		ID3D11RasterizerState*	m_RasterizerWireframeNoBackFaceCulling;

		void clearRenderTarget( const gtColor& );

		bool m_beginRender;

	public:
		gtDriverD3D11( gtMainSystem* System, gtDriverInfo params );
		~gtDriverD3D11( void );
		bool initialize( void );



		const gtDriverInfo&	getParams( void ) const;
			//	
		void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) );
		void endRender( void );
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