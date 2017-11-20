//	GOST

#pragma once
#ifndef __GT_REF_OBJECT_H__
#define __GT_REF_OBJECT_H__

/*
	Подсчёт ссылок
*/

namespace gost{
	 
		///	Все объекты наследуют этот класс.
	class gtRefObject : public gtBaseObject{
	
			///	счётчик
		mutable u32 m_count;
	
	public:

			///	конструктор
		gtRefObject( void ) : m_count( 1U ){}

			///	деструктор
		virtual ~gtRefObject( void ){}

			///	получить количество ссылающихся на этот объект
		u32 getReferenceCount( void ) const {
			return m_count;
		}

			///	Когда ещё 1 объект получает адрес, вызывается эта функция
			///	Просто означает что адрес этого объект а есть ещё у кого-то
			///	Нужно учитывать это создавая новые производные классы
			///	И не нужно забывать про Release()
		void addRef( void ) const {
			++m_count;
		}

			///	В конструкторе как видно, счётчик инициализируется значением 1
			///	Когда вызывается Release, счётчик уменьшается на 1
			///	Если счётчик равен 0 то происходит удаление объекта
		void release( void ) const {
			--m_count;
			if( !m_count ){
				delete this;
			}
		}
	};

}

#endif

/*
Copyright (c) 2017

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