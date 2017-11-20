//	GOST

#pragma once
#ifndef __GT_LOGER_H__
#define __GT_LOGER_H__

/*
	описание 
*/

namespace gost{

		///	выводит сообщения в окно вывода
	class gtLoger : public gtRefObject{
	public:

			///	можно будет отключить показ общей информации
			///	предупреждений или ошибок
		enum class level{
			error,
			warning,
			info
		};

			///	напечатает форматированную строку
			///	%f - float
			///	%i - int
			///	%u - unsigned
			///	%s - char16_t*
			///	%c - char16_t
		virtual void print( level, char16_t* str, ... ) = 0;

			///	установка окна, в которое будет выводится текст
		virtual void setOutputWindow( gtOutputWindow* ) = 0;

			///	info - будут все сообщения
			///	warning - warning и error
			///	error - только error
		virtual void setInfoLevel( level = level::info ) = 0;


	};

		///	Для более простого вывода сообщений
	class gtLogWriter{
	public:
			///	Вывести сообщение об ошибке
		GT_API static	void printError( char16_t* str, ... );
			///	Вывести предупреждение
		GT_API static	void printWarning( char16_t* str, ... );
			///	Вывести информационное сообщение
		GT_API static	void printInfo( char16_t* str, ... );
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