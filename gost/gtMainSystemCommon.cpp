//	GOST 

#include "stdafx.h"

gtPtr<gtLogerImpl> gtMainSystemCommon::s_loger;
gtMainSystemCommon*	gtMainSystemCommon::s_instance;
gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;

gtMainSystemCommon::gtMainSystemCommon( void ) : m_isRun( true ),
m_stackTracer( nullptr ), m_systemWindowCount( 0u ){
	s_fileSystem = nullptr;

	s_loger = gtPtrNew<gtLogerImpl>( new gtLogerImpl );
	s_instance = this;
}

gtMainSystemCommon::~gtMainSystemCommon(){
	if( m_stackTracer ){
		delete m_stackTracer;
		m_stackTracer = nullptr;
	}

	if( s_fileSystem ){
		delete  s_fileSystem;
		s_fileSystem = nullptr;
	}
}

gtLoger* gtMainSystemCommon::getLoger( void ){
	return s_loger.data();
}


void gtMainSystemCommon::initStackTracer( void ){
	if( !m_stackTracer ){
		m_stackTracer = new gtStackTrace( this );
	}
}

	///	возвратит указатель на gtMainSystem
	///	альтернатива this так как this не работает в статических методах
gtMainSystemCommon*	gtMainSystemCommon::getInstance( void ){
	return s_instance;
}
			
	///	возвратит StackTracer
gtStackTrace*	gtMainSystemCommon::getStackTracer( void ){
	return m_stackTracer;
}

	///	gtStackTrace::dumpStackTrace
void gtStackTrace::dumpStackTrace( void ){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(2u,3u);
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