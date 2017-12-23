//	GOST 

#include "stdafx.h"

gtPtr<gtLogerImpl> gtMainSystemCommon::s_loger;
gtMainSystemCommon*	gtMainSystemCommon::s_instance;
gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;

gtMainSystemCommon::gtMainSystemCommon( void ) : m_isRun( true ),
	m_stackTracer( nullptr ), 
	m_systemWindowCount( 0u )
{
	s_fileSystem = nullptr;

	s_loger = gtPtrNew<gtLogerImpl>( new gtLogerImpl );
	s_instance = this;

	m_pluginSystem	= gtPtrNew<gtPluginSystemImpl>( new gtPluginSystemImpl );
}

gtMainSystemCommon::~gtMainSystemCommon(){
	if( m_stackTracer ){
		delete m_stackTracer;
		m_stackTracer = nullptr;
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

void gtMainSystemCommon::initEventSystem( void ){
	m_events		= gtPtrNew<gtEventSystem>( new gtEventSystem( &m_engineConsumer, m_params.m_consumer ) );
}

	//	возвратит указатель на gtMainSystem
	//	альтернатива this так как this не работает в статических методах
gtMainSystemCommon*	gtMainSystemCommon::getInstance( void ){
	return s_instance;
}

gtMainSystem*	gtMainSystem::getInstance( void ){
	return gtMainSystemCommon::getInstance();
}
			
	//	возвратит StackTracer
gtStackTrace*	gtMainSystemCommon::getStackTracer( void ){
	return m_stackTracer;
}

	//	gtStackTrace::dumpStackTrace
void gtStackTrace::dumpStackTrace( void ){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(2u,3u);
}

	//	Инициализирует видео драйвер
gtDriver* gtMainSystemCommon::createVideoDriver( const gtDriverInfo& params ){
	return m_pluginSystem->loadRenderPlugin( params );
}

	//	Завершает работу видео драйвера
void gtMainSystemCommon::removeVideoDriver( gtDriver** pDriver ){
	m_pluginSystem->unloadRenderPlugin( *pDriver );
	*pDriver = nullptr;
}

	//	Выделяет память размером size. Для освобождения нужно вызвать freeMemory
bool gtMainSystemCommon::allocateMemory( void** data, u32 size ){
	GT_ASSERT1( !(*data), "Memory block is not free or pointer not set nullptr", "*data==nullptr" );
	*data = std::malloc( size );
	return (*data)?true:false;
}

	//	Освобождает память, выделенную с помощью allocateMemory
void gtMainSystemCommon::freeMemory( void** data ){
	GT_ASSERT1( *data, "Memory block is not allocated or set nullptr", "*data!=nullptr" );
	std::free( *data );
	*data = nullptr;
}

	//	Загрузит gtImage, если расширение поддерживается хоть каким-то плагином
gtImage*	gtMainSystemCommon::loadImage( const gtString& fileName ){
	gtPtr<gtImage> image = gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName ) );
	if( !image.data() ) return nullptr;
	image->addRef();
	return image.data();
}

	//	Загрузит gtImage плагином имеющим указанный код
gtImage*	gtMainSystemCommon::loadImage( const gtString& fileName, const gtString& pluginGUID ){
	gtPtr<gtImage> image = gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName, pluginGUID, true ) );
	if( !image.data() ) return nullptr;
	image->addRef();
	return image.data();
}

	//	Удаляет картинку из памяти
void		gtMainSystemCommon::removeImage( gtImage* image ){
	if( image ){
		image->release();
		image = nullptr;
	}
}

	//	добавить событие. inFront если вперёд.
void		gtMainSystemCommon::addEvent( const gtEvent& ev, u8 prior ){
	m_events->addEvent( ev, prior );
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