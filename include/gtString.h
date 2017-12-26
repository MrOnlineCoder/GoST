//	GOST

#pragma once
#ifndef __GT_STRING_H__
#define __GT_STRING_H__


/*
*/

namespace gost{

	/* чтобы постоянно не выделять память при добавлении символов */
	const unsigned int StringWordSize = 16u;

	template<typename Type>
	class String{

	/* строка */
	Type * m_data;

	/* выделенная память */
	unsigned int m_allocated;

	/* размер строки */
	unsigned int m_size;

	/* выделение новой памяти */
	void reallocate( unsigned int new_allocated ){

		Type * new_data = new Type[ new_allocated ];

		if( m_data ){
			memcpy( new_data, m_data, m_allocated ); /* копирование старой строки в новую */
			delete []m_data;
		}else{
			memset( new_data, 0,  new_allocated );
		}

		m_data = new_data;

		m_allocated = new_allocated;

	}

	/* размер строки */
	unsigned int getlen( const Type* str ){
		unsigned int len = 0u;
		const Type * p = &str[ 0u ];
		while( *p++ ) len++;
		return len;
	}

	/* копирует src в dst */
	void copy( Type * dst, const Type* src ){
		while( *src ){
			*dst = *src;
			dst++;
			src++;
		}
	}


	public:

	String():
		m_size(0u),
		m_allocated(StringWordSize),
		m_data(nullptr)
	{
		reallocate( m_allocated );
	}

	String( const Type* str ):
		m_size(0u),
		m_allocated(StringWordSize),
		m_data(nullptr)
	{
		reallocate( m_allocated );
		assing( str );
	}
	
	String( const String& str ):
		m_size(0u),
		m_allocated(StringWordSize),
		m_data(nullptr)
	{
		reallocate( m_allocated );
		assing( str );
	}
	
	~String(){
		if( m_data ){
			delete []m_data;
		}
	}
	
	/* присвоить */
	void assing( const Type * str ){
		/* типа очистили строку, просто поставили НОЛЬ вперёд */
		m_size = 0u;
		m_data[ m_size ] = 0x0;

		/* теперь можно добавить строку */
		append( str );
	}

	/* присвоить */
	void assing( const String& str ){
		/* типа очистили строку, просто поставили НОЛЬ вперёд */
		m_size = 0u;
		m_data[ m_size ] = 0x0;

		/* теперь можно добавить строку */
		append( str );
	}

	/* добавить в конец */
	void append( const Type * str ){
		u32 new_size = getlen( str ) + m_size;

		/* если размер больше выделенной памяти (учитывая НОЛЬ) */
		/* то выделяем ещё память + дополнительно StringWordSize*/
		if( (new_size + 1u) > m_allocated ){
			reallocate( (new_size + 1u) + StringWordSize );
		}

		copy( &m_data[m_size], str );

		m_size = new_size;
		m_data[ m_size ] = 0x0;
	}

	/* добавить в конец */
	void append( const String& str ){
		append( str.data() );
	}

	const Type * c_str( void ) const {
		return m_data;
	}

	const Type * data( void ) const {
		return m_data;
	}

	const unsigned int size( void ) const {
		return m_size;
	}

	String& operator=( const String& str ){
		assing( str );
		return *this;
	}

	String operator+( const Type* str ){
		String ret(*this);
		ret.append( str );
		return ret;
	}

	String operator+( const String& str ){
		return operator+(str.data());
	}


	const Type& operator[]( unsigned int i ) const {
		return m_data[ i ];
	}

	Type& operator[]( unsigned int i ){
		return m_data[ i ];
	}

	friend std::ostream& operator<<( std::ostream& os, const String& str );

	};

	inline std::ostream& operator<<( std::ostream& os, const String<char>& str ){
		os << str.data();
		return os;
	}

	using StringA = String<char>;
	using StringW = String<wchar_t>;
	using Stringu = String<char16_t>;
	using StringU = String<char32_t>;
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