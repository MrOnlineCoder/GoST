//	GOST

#pragma once
#ifndef __GT_COLOR_H__
#define __GT_COLOR_H__

/*
	Цвет
*/

namespace gost{
		
		//	для преобразования из char в float
		//	255 / 255 = 1.F // ok
		//	0 / 255 - деление на ноль
		//	0 * чтолибо = 0
		//	255	*	0.00392156862745 = 0.99999999999975
	constexpr f32 gtColorDivider = 0.00392156862745f;

		//	цвет
	class gtColor{

			//	данные
		f32	m_data[ 4u ];


	public:
			//	конструктор по умолчанию
		gtColor( void ){
			//	r g b a - для лучшей совместимости с d3d11
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = 0.f;
			m_data[ 3u ] = 1.f;
		}

			//	устаналивает цвет одним значением
		gtColor( f32 v ){
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = v;
			m_data[ 3u ] = 1.f;
		}

			//	устанавливает цвет указанными значениями
		gtColor( f32 r, f32 g, f32 b, f32 a = 1.f ){
			m_data[ 0u ] = r;
			m_data[ 1u ] = g;
			m_data[ 2u ] = b;
			m_data[ 3u ] = a;
		}

			//	устанавливает цвет указанными значениями
			//	для избежания неоднозначности можно сделать так
			//	gtColor c( u8(255), 255,255,255);
		gtColor( u8 r, u8 g, u8 b, u8 a = 255 ){
			this->setAsByteAlpha( a );
			this->setAsByteRed( r );
			this->setAsByteGreen( g );
			this->setAsByteBlue( b );
		}

			//	argb
		gtColor( u32 uint_data ){
			setAsInteger( uint_data );
		}

			//	возвратит красный цвет
		const f32 getRed( void ) const {
			return m_data[ 0u ];
		}

			//	возвратит зелёный цвет
		const f32 getGreen( void ) const {
			return m_data[ 1u ];
		}

			//	возвратит синий цвет
		const f32 getBlue( void ) const {
			return m_data[ 2u ];
		}

			//	возвратит значение альфа-канала
		const f32 getAlpha( void ) const {
			return m_data[ 3u ];
		}

			//	указатель на массив
		const f32 * getData( void ) const {
			return &m_data[ 0u ];
		}

			//	возвратит красный цвет
		const u8 getAsByteRed( void ) const {
			return static_cast<u8>( m_data[ 0u ] * 255.f );
		}

			//	возвратит зелёный цвет
		const u8 getAsByteGreen( void ) const {
			return static_cast<u8>( m_data[ 1u ] * 255.f );
		}

			//	возвратит синий цвет
		const u8 getAsByteBlue( void ) const {
			return static_cast<u8>( m_data[ 2u ] * 255.f );
		}

			//	возвратит значение альфа-канала
		const u8 getAsByteAlpha( void ) const {
			return static_cast<u8>( m_data[ 3u ] * 255.f );
		}

			//	получить в виде целого типа (0xffffffff)
			//	не уверен что правильная последовательность
			//	придёт время протестирую
		u32 getAsInteger( void ){
			return GT_MAKEFOURCC(
				this->getAsByteBlue(),
				this->getAsByteGreen(),
				this->getAsByteRed(),
				this->getAsByteAlpha()
			);
		}

			//	установить значение альфа-канала
		void setAlpha( f32 v ){	m_data[ 3u ] = v;	}

			//	установить значение красного цвета
		void setRed( f32 v ){	m_data[ 0u ] = v;	}

			//	установить знаечние зелёного цвета
		void setGreen( f32 v ){	m_data[ 1u ] = v;	}

			//	установить значение синего цвета
		void setBlue( f32 v ){	m_data[ 2u ] = v;	}

			//	установить значение альфа-канала
		void setAsByteAlpha( u8 v ){
			m_data[ 3u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить значение красного цвета
		void setAsByteRed( u8 v ){
			m_data[ 0u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить знаечние зелёного цвета
		void setAsByteGreen( u8 v ){
			m_data[ 1u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить значение синего цвета
		void setAsByteBlue( u8 v ){
			m_data[ 2u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	argb
		void setAsInteger( u32 v ){
			this->setAsByteRed( static_cast<u8>( v >> 16 ) );
			this->setAsByteGreen( static_cast<u8>( v >> 8 ) );
			this->setAsByteBlue( static_cast<u8>( v ) );
			this->setAsByteAlpha( static_cast<u8>( v >> 24 ) );
		}
	};

	namespace util{

		template<typename T>
		void _____printColor( const gtColor& c ){
			gtLogWriter::printInfo( u"Color:" );
			const f32 * data = c.getData();
			for( u32 i{ 0u }; i < 4u; ++i )
				gtLogWriter::printInfo( u"\t\t%f", data[ i ] );
		}
#define printColor(x) _____printColor<f32>(x)

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