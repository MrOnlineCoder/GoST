﻿//	GOST

#pragma once
#ifndef __GT_WINDOW_H__
#define __GT_WINDOW_H__

/*
	Всё что связано с системным окном
*/

namespace gost{
		
		//	Параметры окна
	struct gtWindowInfo{

		gtWindowInfo( void ){
			m_title.assign( u"GoST window" );
			m_rect.set({ 0, 0, 800, 600 });
			m_style = 0u;
		}
		
			//	заголовок
		gtString	m_title;

			//	координаты левого верхнего и правого нижнего углов
		v4i			m_rect;
		
			//	стиль окна
			//	если ничего не указано то будет стандартное окно - рамка, без изменения размера и без кнопки maximize
		enum style_{
			_standart,
				//	если это, то будет только окно без рамки
			_popup = 1u,
				//	это и далее работает без popup
				//	можно изменять размер окна
			_resize = 2u,
				//	есть кнопка maximize
			_maximize = 4u
		};

		u32 m_style;

		bool	operator==( const gtWindowInfo& wi ){
			if( m_title == wi.m_title )
				if( m_rect == wi.m_rect )
					if( m_style == m_style )
						return true;

			return false;
		}
	};
	
		//	Системное окно
	class gtWindow : public gtRefObject{
	public:

			//	Установит заголовок окна
		virtual void	setWindowTitle( const gtString& ) = 0;

		virtual void*	getHandle( void ) = 0;

			//	Получить координаты левого верхнего и правого нижнего углов
		virtual const v4i&		getRect( void ) = 0;

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