//	GOST

#pragma once
#ifndef __GT_MAIN_SYSTEM_COMMON_H__
#define __GT_MAIN_SYSTEM_COMMON_H__

/*
	Общий класс 
*/

namespace gost{

	
		///	Общий класс для конкретных реализаций gtMainSystem
	class gtLogerImpl;
	class gtMainSystemCommon : public gtMainSystem{
	protected:

		gtList< gtPtr< gtWindow > > m_windowCache;


			///	параметры главной системы
		gtDeviceCreationParameters	m_params;

			///	она возвращается в методе update
		bool	m_isRun;
		
			///	объект для вывода стека вызова функций
		gtStackTrace* m_stackTracer;
		
			///	по сути это this
		static gtMainSystemCommon*	s_instance;

		u32 m_systemWindowCount;

		gtPtr<gtPluginSystemImpl> m_pluginSystem;

	public:

			///	конструктор
		gtMainSystemCommon( void );

			///	деструктор
		virtual ~gtMainSystemCommon( void );

		static gtPtr<gtLogerImpl> s_loger;

		static gtFileSystemCommon* s_fileSystem;
		

		gtLoger*		getLoger( void );

			///	так как трейсеру нужно окно вывода, а это окно создаётся чуть позже выделения памяти для него
			///	то, после создания окна, вызывается эта функция, чтобы можно было дать трейсеру
			///	инициализированный объект окна
		void initStackTracer( void );

			///	возвратит указатель на gtMainSystem
			///	альтернатива this так как this не работает в статических методах
		static gtMainSystemCommon*	getInstance( void );
			
			///	возвратит StackTracer
		gtStackTrace*	getStackTracer( void );

			//	Инициализирует видео драйвер
		gtDriver* createVideoDriver( const gtDriverInfo& );

			//	Завершает работу видео драйвера
		void		removeVideoDriver( gtDriver** );
	};

#define gtLog gtMainSystemCommon::s_loger


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