//	GOST

#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__


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
		{}

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
	};


	using gtGetPlugunInfo = gtPluginInfo&(*)();

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