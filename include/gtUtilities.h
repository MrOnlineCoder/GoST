//	GOST

#pragma once
#ifndef __GT_UTILITIES_H__
#define __GT_UTILITIES_H__

/*
В нём будут лежать вспомогательные функции,
которые могут пригодиться в любом месте.
*/

namespace gost{
	
	namespace util{
		
			//	изменяет символ \ на /
		template<typename Type>
		GT_FORCE_INLINE void stringFlipSlash( Type& str ){
			u32 sz = str.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( str[ i ] == '\\' ) 
					str[ i ] = '/'; // стандартный набор символов подходит для всех версий char
			}
		}

			//	переворачивает строку
			//	abc -> cba
		template<typename Type>
		GT_FORCE_INLINE void stringFlip( Type& str ){
			Type flippedStr;
			for( u32 i = str.size() - 1u; i >= 0u; --i ){
				flippedStr += str[ i ];
				if( !i ) break;
			}
			str = flippedStr;
		}

			//	удаляет последний символ пока не встретится указанный
		template<typename Type>
		GT_FORCE_INLINE void stringPopBackBefore( Type& str, s8 c ){
			if( str.size() )
				str.pop_back();
			if( str.size() )
				for( u32 i = str.size() - 1u; i >= 0u; --i ){
					if( str[ i ] == c ) break;
					else str.pop_back();
					if( !i ) break;
				}
		}

			//	возвратит расширение - последнюю часть строки после точки
			//	слэши должны быть такими /
		template<typename Type>
		GT_FORCE_INLINE Type stringGetExtension( const Type& str ){
			
			// по сути нужно всего лишь добавлять символы с конца
			//	пока не будет встречена точка либо slash
			Type ret;

			for( u32 i = str.size() - 1u; i >= 0u; --i ){
				auto c = str[ i ];
				if( c == '/' || c == '.' )
					break;
				else ret += c;
				if( !i ) break;
			}

			stringFlip<Type>( ret );

			return ret;
		}


	}

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