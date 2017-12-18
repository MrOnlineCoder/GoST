//	GOST

#pragma once
#ifndef __GT_MAIN_SYSTEM_H__
#define __GT_MAIN_SYSTEM_H__

/*
	Главный интерфейс 
*/

namespace gost{

	class gtOutputWindow;
	class gtLoger;
	class gtWindow;
	struct gtWindowInfo;
	class gtDriver;
	struct gtDriverInfo;
	struct gtImage;
	
		//	Перечисление ОС
	enum class gtDeviceType{
		android,
		ios,
        linux,
		osx,
		windows,	//	конечно же будет реализован пока только он
		xbox,
		playstation,
		wii
		//хватит
	};

		//	В этой структуре будут находится параметры для запуска главной системы
	struct gtDeviceCreationParameters{
		gtDeviceCreationParameters( void ){
#if defined(GT_PLATFORM_WIN32)
			m_device_type	=	gtDeviceType::windows;
#else
#error "Эта ОС не поддерживается"
#endif
			m_outputWindow = nullptr;
		}
		~gtDeviceCreationParameters( void ){}

			//	Тип Операционной системы на которой будет работать программа
		gtDeviceType		m_device_type;

			//	если nullptr то будет создано стандартное окно вывода
		gtOutputWindow*		m_outputWindow;

	};

		//	Основной класс движка
	class gtMainSystem : public gtRefObject{
	public:

			//	Для начала эта функция подойдёт.
			//	Как раз пригодится в будущем.
		virtual gtOutputWindow* getOutputWindow( void ) = 0;

			//	Получить логер
		virtual gtLoger*		getLoger( void ) = 0;

			//	Используется для главного цикла. Возвращает true если всё впорядке, или не был послан сигнал о завершении работы
		virtual	bool	update( void ) = 0;
			
			//	Создаёт окно, которое можно использовать для рисования 3D сцены
		virtual gtWindow*	createSystemWindow( const gtWindowInfo& ) = 0;

			//	Инициализирует видео драйвер
		virtual gtDriver*	createVideoDriver( const gtDriverInfo& ) = 0;

			//	Завершает работу видео драйвера, выгружает .dll и удаляет из коллекции
			//	возможно и не нужно.
			//	можно вызвать driver->release();
			//	но библиотека будетвсё ещё загружена. По этому есть эта функция.
		virtual void		removeVideoDriver( gtDriver** ) = 0;
		
			//	Выделяет память размером size. Для освобождения нужно вызвать freeMemory
		virtual bool		allocateMemory( void** data, u32 size ) = 0;

			//	Освобождает память, выделенную с помощью allocateMemory
		virtual void		freeMemory( void** data ) = 0;

			//	Загрузит gtImage, если расширение поддерживается хоть каким-то плагином
		virtual gtImage*	loadImage( const gtString& fileName ) = 0;

			//	Загрузит gtImage плагином имеющим указанный код
		virtual gtImage*	loadImage( const gtString& fileName, const gtString& pluginGUID ) = 0;

			//	Удаляет картинку из памяти
		virtual void		removeImage( gtImage* ) = 0;

			//	возвратит указатель на gtMainSystem
			//	альтернатива this так как this не работает в статических методах
		GT_API static gtMainSystem*	getInstance( void );
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