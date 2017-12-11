//	GOST

#pragma once
#ifndef __GT_LIST_H__
#define __GT_LIST_H__

/*
	В gost.h стоит ниже #include <gtError.h>
*/

namespace gost{

		//	простой список
	template< typename Type >
	class gtList{

			//	контейнер хранит вот эти объекты
		struct node{
			node(Type i):m_instance(i),m_next(nullptr){	}
			
				//	объект для хранения
			Type m_instance;

				//	указатель на следующий объект в списке
			node * m_next;

		};

			//	самый первый объект
		node * m_first;

			//	самый последний
		node * m_last;

			//	размер
		u32 m_size;

	public:

		gtList(void):m_first(nullptr),m_last(nullptr),m_size(0u){}
		~gtList(void){
			clear();
		}

			//	добавляет новый элемент
		void add( Type v ){
			node * new_data = new node(v);
			if( !m_first ){	// первый элемент
				m_first = new_data;
			}else{ // последующие
				m_last->m_next = new_data;
			}
			m_last = new_data;
			m_size++;
		}

			//	возвратит элемент 
		Type	get( u32 id ){

			GT_ASSERT( id < m_size, "Bad argument", "id < m_size", id, m_size );

			Type ret;

			node * tmp = m_first;
			u32 _id = id+1u;
			for( u32 i = 0u; i < _id; ++i ){
				ret = tmp->m_instance;
				tmp = tmp->m_next;
			}

			return ret;
		}

			//	возвратит последний элемент
		Type	getLast( void ){
			if( m_last ){
				return m_last->m_instance;
			}
			return nullptr;
		}

			//	возвратит количество объектов хранящихся в контейнере
		u32		size( void ) const {
			return m_size;
		}

			//	удаляет все элементы
		void	clear( void ){
			if( m_size ){
				node * tmp = m_first;
				m_first = nullptr;
				m_last = nullptr;
				for( u32 i = 0u; i < m_size; ++i ){
					node * next = tmp->m_next;
					if( tmp )
						delete tmp;
					tmp = next;
				}
				m_size = 0u;
			}
		}
			
			//	удалит элемент по id
		void	remove( u32 id ){

			GT_ASSERT( id < m_size, "Bad argument", "id < m_size", id, m_size );

			node * node_to_delete = nullptr;

			if( id == 0u ){
				node_to_delete = m_first;
				m_first = node_to_delete->m_next;
			}else{
				node * tmp = m_first;
				node * tmp2 = nullptr;	//элемент который стоит перед элементом которого нужно удалить
				u32 _id = id;
				for( u32 i = 0u; i < _id; ++i ){
					tmp2 = tmp;
					tmp = tmp->m_next;
				}
				node_to_delete = tmp;
				tmp = node_to_delete->m_next;
				tmp2->m_next = tmp;
			}

			delete node_to_delete;	node_to_delete = nullptr;

			m_size--;
			if( m_size == 0u ) m_last = nullptr;
		}

			//	удалит все элементы хранящих указанное значение
		void	remove( Type val ){
			if( m_size ){
				u32 C = findCount( val );
				
				for( u32 i = 0u; i < C; ++i ){
					remove( findFirst( val ) );
				}
			}
		}

			//	Найдёт количество эементов с указанным значением
		u32		findCount( Type val ){
			u32 C = 0u;

			if( m_size ){
				node * tmp = m_first;
				for( u32 i = 0u; i < m_size; ++i ){
					if( tmp->m_instance == val ) C++;
					tmp = tmp->m_next;
				}
			}

			return C;
		}

			//	Найдёт первое совпадение
			//	Если элемента нет то возврат размера контейнера
		u32		findFirst( Type val ){
			if( m_size ){
				node * tmp = m_first;
				for( u32 i = 0u; i < m_size; ++i ){
					if( tmp->m_instance == val ) return i;
					tmp = tmp->m_next;
				}
			}
			return m_size;
		}

			//	Найдёт последнее совпадение
			//	Если элемента нет то возврат размера контейнера
		u32		findLast( Type val ){
			if( m_size ){
				node * tmp = m_first;
				bool find = false;
				u32 id = 0u;
				for( u32 i = 0u; i < m_size; ++i ){
					if( tmp->m_instance == val ){ id = i; find = true; }
					tmp = tmp->m_next;
				}
				if( find )return id;
			}
			return m_size;
		}
		
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