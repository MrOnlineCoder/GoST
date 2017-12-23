// test01.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"
#include <iostream>

#include <gost.h>

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

using namespace gost;

class myOutput : public gtOutputWindow {
	HWND m_hWnd;
	bool m_isInit;
	
	~myOutput( void ){	}

public:

	myOutput( void ) : m_hWnd( 0 ), m_isInit( false ){
#ifdef GT_DEBUG
		this->setDebugName( u"OutputWindow" );
#endif

	}


	//	Прежде чем использовать нужно вызвать Init
	void	init( void ){
		if( !AllocConsole() ){
			return;
		}

		freopen("CONOUT$", "w", stdout);

		m_hWnd = GetConsoleWindow();

		m_isInit = true;
	}

		//	При завершении работы нужно вызвать этот метод
	void	shutdown( void ){
		if( m_isInit ){
			if( FreeConsole() ){
				m_isInit = false;
			}
		}
	}

		//	Возвратит true если инициализирован
	bool	isInit( void ){
		return m_isInit;
	}

		//	Если окно невидимо, покажет его
	void	show( void ){
	}

		//	Сделает окно невидимым (спрячет, или сделает прозрачным, зависит от реализации)
	void	hide( void ){
	}

		//	Напечатает текст
	void	print( const gtString& s ){
		if( m_isInit )
			wprintf( L"%s\n", (wchar_t*)s.data() );
	}

		//	Установит заголовок
	void	setWindowText( const gtString& s ){
		if( m_isInit )
			SetWindowText( m_hWnd, (wchar_t*)s.data() );
	}
};

void ScanDirs( gtString dir, bool subDir ){

	//	сначала получаем все файлы и папки текущей дирректории
	//	потом рекурсивно проходимся по папкам
	gtArray<gtFileSystem::DirObject> objs;

	gtFileSystem::scanDirBegin( dir );
	gtFileSystem::DirObject o;
	while( gtFileSystem::getDirObject( &o ) ){
		objs.push_back( o );
	}
	gtFileSystem::scanDirEnd();

	u32 sz = objs.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &objs[ i ];
		if( o->type == gtFileSystem::DirObjectType::info ) 
			continue;

	/*	gtLogWriter::printInfo( 
			u"%s - %s - %u bytes", 
			o->type == gtFileSystem::DirObjectType::folder
				? u"Dir" : u"File",
			o->path, o->size );*/

		if( o->type == gtFileSystem::DirObjectType::folder ){
			if( subDir ){
				ScanDirs( (char16_t*)o->path, true );
			}
		}
	}

}

class eventConsumer : public gtEventConsumer{
public:

	void processEvent( const gtEvent& ev ){

		switch( ev.type ){
			case gtEvent::ET_SYSTEM:{
				switch( ev.value1 ){
				case GT_EVENT_WINDOW_MAXIMIZE:
					gtLogWriter::printInfo( u"Window maximized" );
					break;
				case GT_EVENT_WINDOW_MINIMIZE:
					gtLogWriter::printInfo( u"Window minimized" );
					break;
				case GT_EVENT_WINDOW_MOVE:
					gtLogWriter::printInfo( u"Window moved" );
					break;
				case GT_EVENT_WINDOW_PAINT:
					gtLogWriter::printInfo( u"Window painted" );
					break;
				case GT_EVENT_WINDOW_RESTORE:
					gtLogWriter::printInfo( u"Window restored" );
					break;
				case GT_EVENT_WINDOW_SIZING:
					gtLogWriter::printInfo( u"Change size" );
					break;
				}
			}break;
		}
		
	}
};

#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
#endif

	eventConsumer	events;

    gtDeviceCreationParameters params;
	params.m_consumer = &events;

	 
    gtPtr<gtMainSystem> my_system( gtPtrNew<gtMainSystem>( InitializeGoSTEngine(params) ) );
    
    if( !my_system.data() ) return 1;
    

    gtWindowInfo wi;
    
    gtPtr< gtWindow > window1;
 
	 
    wi.m_style = gtWindowInfo::_standart;
    wi.m_style |= gtWindowInfo::_maximize;
    wi.m_style |= gtWindowInfo::_resize;
	wi.m_rect.set({0,0,800,600});
 
    window1 = gtPtrNew<gtWindow>( my_system->createSystemWindow( wi ) );
 
	gtLogWriter::printInfo( u"sz %u", sizeof(gtEvent) );

        
    gtDriverInfo vparams;
    vparams.m_GUID = GT_GUID_RENDER_D3D11;
	vparams.m_vSync = false;
    
    vparams.m_outWindow = window1.data();
    gtDriver* driver1 = my_system->createVideoDriver( vparams );



	gtImage * image = my_system->loadImage( gtFileSystem::getProgramPath() + u"bmp24bit.bmp" );
	
	// если без gtPtr, то в конце нужно вызвать texture->release();
	gtPtr<gtTexture> texture = gtPtrNew<gtTexture>( driver1->createTexture( image ) );

	/* software картинка больше не нужна */
	my_system->removeImage( image );
	
	gtMaterial material;
	material.textureLayer[ 0u ].texture = texture.data();

	f32 posx = 300.f;
	f32 posy = 250.f;

	u32 then = my_system->getTime();

	u32 time = 0u;
	f32 deltaTime = 0.f;
	f32 angle = 0.f;

    while( my_system->update() ){

		u32 now = my_system->getTime();
		
		deltaTime = f32(now - then)*0.001f;
 
		time += now - then;

		then = now;


		if( time > 1000u ){
			gtLogWriter::printInfo( u"deltaTime %f", deltaTime );
			
			time = 0u;
		}
        if( driver1 ){
            driver1->beginRender(true,gtColor(1.f,0.f,0.f,1.f));

			angle += 4 * deltaTime;
			if( angle > 720.f ) angle = 0.f;

			posx += 0.4f * sin(angle);
			posy += 0.4f * cos(angle);

			driver1->draw2DImage( v4i({ 0 + (s32)posx, 0 + (s32)posy, 50 + (s32)posx, 50 + (s32)posy }), v4i( { 19, 381, 80, 460 } ), material);

			//	отдельная буква
			driver1->draw2DImage( v4i({ 0, 0, 100, 100 }), material);

			//	
			driver1->draw2DImage( v4i({ 200, 0, 600, 400 }), v4i( { 0, 0, 1024, 1024 } ), material);


            driver1->endRender();
        }
 
    }

//	if( texture )
	//	texture->release();

    return 0;
}

