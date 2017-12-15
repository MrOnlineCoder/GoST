//	GOST

#pragma once
#ifndef __GT_IMAGE_H__
#define __GT_IMAGE_H__

/*
	Software картинка
*/

namespace gost{


	struct gtImage{

		gtImage( void ):
			format( format::FMT_R8G8B8A8 ),
			width( 0u ),
			height( 0u ),
			bits( 32u ),
			mipCount( 1u ),
			data( nullptr ),
			dataSize( 0u ),
			pluginId( -1 )
		{}

		~gtImage( void ){
			if( data ){
				delete []data;
				data = nullptr;
			}
		}


		enum format{

			FMT_R8G8B8A8,

			FMT_R8G8B8,


			//	OpenGL
			FMT_COMPRESSED_RGBA_S3TC_DXT1 = 0x83F1,
			FMT_COMPRESSED_RGBA_S3TC_DXT3 = 0x83F2,
			FMT_COMPRESSED_RGBA_S3TC_DXT5 = 0x83F3

		}format;

			//	ширина
		u32		width;

			//	высота
		u32		height;

			//	биты на пиксель
		u32		bits;

			//	количество mipMap. 1 - основная картинка
		u32		mipCount;

		u8	*	data;
		u32		dataSize;

			//	id плагина который загрузил картинку
			//	нужно чтобы потом этот плагин её выгрузил
			//	По умолчанию -1, можно создавать и уничтожать вручную где вздумается
		s32		pluginId;


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