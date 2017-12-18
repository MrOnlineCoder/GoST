//	GOST

#pragma once
#ifndef __GT_PLUGIN_RENDER_H__
#define __GT_PLUGIN_RENDER_H__


namespace gost{

	/*
		Ёто обЄртка к render плагину.
		’ранит информацию.
		Ѕлагодор€ этому можно грузить и выгружать плагин 
		использу€ методы load unload
	*/
	class gtPluginRender : public gtPlugin{
	public:

		gtPluginRender( gtPluginInfoDL* info );

		~gtPluginRender( void );
		
		struct Driver{

			Driver( void (*f)(), gtDriver* d ):
				loadPlugin( f ),
				driver( d )
			{}

			//	указатель на функцию
			//	render плагин - указывает на функцию запуска.
			void (*loadPlugin)();

				//	видео драйвера данного плагина
			gtDriver* driver;

		};

		gtArray<Driver> m_driver;

			//	загрузить плагин
		void loadDriver( const gtDriverInfo& params );

			//	выгрузить драйвер под номером id
		void unloadDriver( u32 id );

			//	загрузить плагин
		void load( void );

			//	выгрузитьплагин
		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

			//	проверит есть ли нужные функции в плагине
		bool checkLibraryFunctions( GT_LIBRARY_HANDLE );
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