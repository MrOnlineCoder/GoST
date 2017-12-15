//	GOST

#pragma once
#ifndef __GT_PLUGIN_SYSTEM_IMPL_H__
#define __GT_PLUGIN_SYSTEM_IMPL_H__

namespace gost{

	/*	Информация о плагине. Содержит путь к файлу
		указатель на фукнцию запуска,
		хэндл,
		так же общая информация gtPluginInfo*/
	struct gtPluginInfoDL{

		gtPluginInfoDL( void ):
			m_loadPlugin( nullptr ),
			m_handle( nullptr )
		{}

		~gtPluginInfoDL( void ){}

		//	путь к файлу
		gtString m_path;

		//	указатель на функцию записи
		void (*m_loadPlugin)();
		
		//	handle плагина
		GT_LIBRARY_HANDLE m_handle;

		//	общая информация
		gtPluginInfo m_info;
	};

	/*
		Это обёртка к render плагину.
		Хранит информацию.
		Благодоря этому можно грузить и выгружать плагин 
		используя методы load unload
	*/
	class gtPluginRender : public gtRefObject{

			//	загружен ли плагин
		bool m_isLoad;

			//	информация
		gtPluginInfoDL	m_info;

	public:

		gtPluginRender( gtPluginInfoDL* info ):
			m_isLoad( false )
		{
			m_info = *info;
		}

		~gtPluginRender( void ){
			unload();
		}

			//	видео драйвер
		gtDriver* m_driver;

			//	загрузить плагин
		void load( const gtDriverInfo& params );

			//	выгрузитьплагин
		void unload( void );

	};

	class gtPluginSystemImpl GT_FINAL : public gtPluginSystem{

		void scanFolder( const gtString& );

		u32 m_numOfPlugins;

		//	доступные для загрузки
		gtArray<gtPluginInfoDL> m_renderPluginCache;
		
		//	загруженные плагины
		gtList<gtPtr<gtPluginRender>>	m_renderPlugins;

	public:
		gtPluginSystemImpl( void );
		virtual ~gtPluginSystemImpl( void );

		bool init( void );

			//	получить количество плагинов в папке plugins
		u32	getNumOfPlugins( void );

			//	загружает видео плагин
		gtDriver*	loadRenderPlugin( const gtDriverInfo& params );

			//	выгружает и удаляет из коллекции
		void 		unloadRenderPlugin( gtDriver* );

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