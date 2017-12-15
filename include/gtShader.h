//	GOST

#pragma once
#ifndef __GT_SHADER_H__
#define __GT_SHADER_H__

/*
*/

namespace gost{

	struct gtShaderModel{

		enum class shaderModel{
			_1_1,		//	OpenGL + DirectX
			_1_2,		//	OpenGL
			_1_3,		//	OpenGL
			_1_4,		//	OpenGL
			_1_5,		//	OpenGL
			_2_0,		//	DirectX
			_3_0,		//	DirectX
			_3_3,		//	OpenGL
			_4_0,		//	OpenGL + DirectX
			_4_1,		//	OpenGL + DirectX
			_4_2,		//	OpenGL
			_4_3,		//	OpenGL
			_4_5,		//	OpenGL
			_5_0,		//	DirectX
			_6_0		//	DirectX
		}vertexShaderModel, pixelShaderModel;

	};

		//	Шейдер
	class gtShader : public gtRefObject {
	public:

		virtual gtShaderModel	getShaderModel( void ) = 0;

			//	В d3d11 создаёт константный буффер
		virtual bool	createShaderObject( u32 byteSize ) = 0;

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