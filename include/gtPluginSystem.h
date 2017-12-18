//	GOST

#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__


/*
*/

namespace gost{
	
	class gtMainSystem;


	//	указывается при создании gtDriver
	const gtString GT_GUID_RENDER_D3D11( u"{41B20362-9FC0-4C40-9903-B8D2FF98CF88}" );
	
	//	указывается при точном выборе плагина загрузки картинок
	const gtString GT_GUID_IMPORT_IMAGE_BMP( u"{B0904D7B-5AA3-4023-BC99-ECA2232E1EBE}" );

	//	для загрузки функций из плагинов
	using gtGetPluginInfo = void(*)(gtPluginInfo&);
	using gtLoadGPUDriver_t = gtDriver*(*)(gtMainSystem*,gtDriverInfo);
	using gtPluginGetExtCount_t = u32(*)( void );
	using gtPluginGetExtension_t = s8*(*)( u32 id );
	using gtPluginLoadImage_t = bool(*)(gtImage*,gtString*);

		//	Интерфейс для работы с плагинами
	class gtPluginSystem : public gtRefObject{
	public:

			//	получить количество плагинов в папке plugins
		virtual u32	getNumOfPlugins( void ) = 0;


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