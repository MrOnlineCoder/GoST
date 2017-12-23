//	GOST

#pragma once
#ifndef __GT_VECTOR_H__
#define __GT_VECTOR_H__

/*
	Вектор
*/

namespace gost{
		
#define x_ at(0u)
#define y_ at(1u)
#define z_ at(2u)
#define w_ at(3u)
#define a_ at(4u)
#define b_ at(5u)
#define c_ at(6u)
#define d_ at(7u)

	template<typename T, u32 i >
	class gtVector{
	
			//	массив с данными
		T	m_data[ i ];

	public:

			//	ctor по умолчанию
		gtVector( void ){
			GT_STATIC_ASSERT( i > 1u, gtError::GT_VECTOR_BAD_SIZE );
			fill( 0u );
		}

			//	такая инициализация
			//	vector<f32, 4> v = {1.f,2.1f,1.3f,5.14f};
			//	или vector<f32, 4>{21.f,32.1f,41.3f,55.14f}
		gtVector( const std::initializer_list<T>& l ){
			set( l );
		}

			//	установит каждый компонент равным 0
		void zero( void ){
			fill( 0u );
		}

			//	заполнит вектор указанным значением
		void fill( T val ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] = static_cast<T>( val );
		}

			//	установить значение компонента
		void setComponent( u32 id, T value ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			m_data[ id ] = value;
		}

			//	даёт доступ к компоненту вектора
		T&	operator()( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			//	даёт доступ к компоненту вектора. только чтение
		const T&	operator()( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			//	даёт доступ к компоненту вектора
		T&	operator[]( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			//	даёт доступ к компоненту вектора. только чтение
		const T&	operator[]( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			//	даёт доступ к компоненту вектора
		T&	at( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			//	даёт доступ к компоненту вектора. только чтение
		const T&	at( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}
			//	получить значение компонента
		const T	getComponent( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}
		
			//	установка компонентов
			//	v.set( {3.14f, 6.28f, 9.81f} );
		void	set( const std::initializer_list<T>& l ){
			GT_ASSERT2( l.size() <= i, "l.size() <= i" );
			auto * p = &m_data[0u];
			for each( auto var in l ){
				*p++ = var;
			}
		}

			//	возвратит размер вектора
		u32	getSize( void ){
			return i;
		}

		bool	operator==( const gtVector<T, i>& v ){
			for( u32 o{ 0u }; o < i; ++o ){
				if( m_data[ o ] != v.m_data[ o ] ) return false;
			}
			return true;
		}

	};

	namespace util{

		template<typename T, u32 i>
		void printVector( gtVector<T, i>& v ){
			gtLogWriter::printInfo( u"Vector:" );
			u32 sz = v.getSize();
			for( u32 i{ 0u }; i < sz; ++i )
				gtLogWriter::printInfo( u"\t\t%f", v.getComponent( i ) );
		}

	}


	using v2f = gtVector<f32, 2u>;
	using v3f = gtVector<f32, 3u>;
	using v4f = gtVector<f32, 4u>;
	using v5f = gtVector<f32, 5u>;
	using v6f = gtVector<f32, 6u>;
	using v7f = gtVector<f32, 7u>;
	using v8f = gtVector<f32, 8u>;
	using v2i = gtVector<s32, 2u>;
	using v3i = gtVector<s32, 3u>;
	using v4i = gtVector<s32, 4u>;
	using v5i = gtVector<s32, 5u>;
	using v6i = gtVector<s32, 6u>;
	using v7i = gtVector<s32, 7u>;
	using v8i = gtVector<s32, 8u>;
	using v2u = gtVector<u32, 2u>;
	using v3u = gtVector<u32, 3u>;
	using v4u = gtVector<u32, 4u>;
	using v5u = gtVector<u32, 5u>;
	using v6u = gtVector<u32, 6u>;
	using v7u = gtVector<u32, 7u>;
	using v8u = gtVector<u32, 8u>;

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