//	GOST 

#include "stdafx.h"


gtLogerImpl::gtLogerImpl( void )
: m_level(level::info){}

gtLogerImpl::~gtLogerImpl( void ){}

	///	напечатает форматированную строку
	///	%f - float
	///	%i - int
	///	%u - unsigned
	///	%s - char16_t*
	///	%c - char16_t
void gtLogerImpl::print( level lvl, char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	print( lvl, str, args );
	va_end( args );
}


void gtLogerImpl::print( level lvl, char16_t* str, void * p ){
	if( m_level >= lvl ){
		gtString message;
		switch( lvl ){
		case gost::gtLoger::level::error:
			message.assign(u"Error: ");
			break;
		case gost::gtLoger::level::warning:
			message.assign(u"Warning: ");
			break;
		case gost::gtLoger::level::info:
			break;
		}

		gt_va_list args = p;
		deformat( str, args, message );

		m_out->print( message );
	}
}

void gtLogerImpl::deformat( const char16_t* fmt, 
	gt_va_list& args, gtString& message ){

	///	получаю размер строки fmt
	u32 len = 0U;
	const char16_t* p = fmt;
	do	{		++len;	} while(*p++);
	--len;

	va_list list = (va_list)args;

	bool S = false;
	for( u32 i(0u); i < len; ++i ){

		std::wostringstream ss;


		if( S ){
			if( fmt[ i ] == u'f' ){
				ss << va_arg( list, f64 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'i' ){
				ss << va_arg( list, s32 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'u' ){
				ss << va_arg( list, u32 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'c' ){
				message += va_arg( list, char16_t );
				continue;
			}else if( fmt[ i ] == u's' ){
				char16_t * p2 = va_arg( list, char16_t* );
				u32 len2( 0U );
				do{ ++len2; } while(*p2++);
				p2 -= len2; // возвращаюсь к началу строки
				// и копирую её
				for( u32 o(0U); o < len2-1u; ++o )
					message += p2[ o ];
				continue;
			}
		}

		if( fmt[ i ] == u'%' ){
			if( !S ) S = true;
			else S = false;
		}else S = false;

		if( !S )
			message += fmt[ i ];
	}

}

	///	установка окна, в которое будет выводится текст
void gtLogerImpl::setOutputWindow( gtOutputWindow* out ){
	m_out = out;
}

	///	info - будут все сообщения
	///	warning - warning и error
	///	error - только error
void gtLogerImpl::setInfoLevel( level lvl ){
	m_level = lvl;
}

	///	Вывести сообщение об ошибке
void gtLogWriter::printError( char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtLog->print( gost::gtLoger::level::error, str, args );
	va_end( args );
}

	///	Вывести предупреждение
void gtLogWriter::printWarning( char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtLog->print( gost::gtLoger::level::warning, str, args );
	va_end( args );
}

	///	Вывести информационное сообщение
void gtLogWriter::printInfo( char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtLog->print( gost::gtLoger::level::info, str, args );
	va_end( args );
}

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