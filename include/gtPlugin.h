//	GOST

#pragma once
#ifndef __GT_PLUGIN_H__
#define __GT_PLUGIN_H__


/*
*/

namespace gost{

		//	тип плагина
	enum class gtPluginType{

		//	плагины должны быть правильно инициализированы
		unknown,

		//	рендер
		render,

		//	импортёр картинок
		import_image,

		//	импортёр моделей
		import_model

		// для начала хватит
	};

		//	общая информация о плагине
	struct gtPluginInfo{
		gtPluginInfo( void ):
			m_type( gtPluginType::unknown )
		{
				//	нельзя выделять память в одном модуле и освобождать в другом
				//	выход за пределы - на совести разработчика плагина
			m_name.reserve( 64u );
			m_author.reserve( 64u );
			m_url.reserve( 64u );
			m_email.reserve( 64u );
			m_company.reserve( 64u );
			m_description.reserve( 512u );
			m_GUID.reserve( 64u );
		}

		//	тип плагина
		gtPluginType m_type;

		//	название плагина
		gtString m_name;

		//	автор
		gtString m_author;

		//	ссылка на сайт
		gtString m_url;

		//	контактный имейл
		gtString m_email;

		//	имя компании если юр лицо
		gtString m_company;

		//	описание
		gtString m_description;

		//	уникальный ID. по сути любая строка.
		//	имя по аналогии с GUID. {41B20362-9FC0-4C40-9903-B8D2FF98CF88}
		gtString m_GUID;

	};

	/*	Информация о плагине(более близкая к самому модулю).
		Содержит путь к файлу
		хэндл,
		так же общая информация gtPluginInfo*/
	struct gtPluginInfoDL{

		gtPluginInfoDL( void ):
			m_handle( nullptr )
		{}

		~gtPluginInfoDL( void ){}

		//	путь к файлу
		gtString m_path;
		
		//	handle плагина
#ifdef GT_EXPORTS
		GT_LIBRARY_HANDLE
#else
		void*
#endif
			m_handle;

		//	общая информация
		gtPluginInfo m_info;
	};
	
	class gtPlugin : public gtRefObject{
	protected:

			//	информация
		gtPluginInfoDL	m_info;

			//	загружен ли плагин
		bool m_isLoad;

	public:

		gtPlugin( void ):
			m_isLoad( false )
		{}

		virtual ~gtPlugin( void ){	}

		bool isLoad( void ) const { return m_isLoad; }

			//	загрузить плагин
		virtual void load( void ) = 0;

			//	выгрузитьплагин
		virtual void unload( void ) = 0;

		virtual const gtPluginInfoDL&	getInfo( void ) = 0;

			//	проверит есть ли нужные функции в плагине
		virtual bool checkLibraryFunctions( 
#ifdef GT_EXPORTS
			GT_LIBRARY_HANDLE
#else
		void*
#endif
		) = 0;



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