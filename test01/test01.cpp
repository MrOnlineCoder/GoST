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
	//	хранит состояния нажатых клавиш
	bool m_keysDown[ 256u ];

	//	отжатых клавиш. возможно всё можно уместить в один массив. я не задумывался. так нагляднее
	bool m_keysUp[ 256u ];
public:

	eventConsumer( void ):
		mouse_x( 0 ),
		mouse_y( 0 ),
		lmb( false ),
		rmb( false ),
		mmb( false ),
		ext1( false ),
		ext2( false ),
		wheel_delta( 0 ){
		memset( m_keysDown, 0, 256u );
		memset( m_keysUp, 0, 256u );
	}

	void processEvent( const gtEvent& ev ){

		rmb = lmb = mmb = ext1 = ext2 = false;

		switch( ev.type ){
			case gtEvent::ET_SYSTEM:{
			}break;
			case gtEvent::ET_KEY:{
				m_keysDown[ ev.value1 & GT_EVENT_MASK_KEYS ] = (ev.value1 & GT_EVENT_MASK_KEY_PRESS) ? true : false;
				m_keysUp[ ev.value1 & GT_EVENT_MASK_KEYS ] = (ev.value1 & GT_EVENT_MASK_KEY_PRESS) ? false : true;

				if( ev.value2 ){
					char16_t s[2]{ ev.value2 , 0 };
					gtLogWriter::printInfo( u"%s", s );
				}

			}break;
			case gtEvent::ET_MOUSE:{
				mouse_x = LOWORD(ev.value1);
				mouse_y = HIWORD(ev.value1);
				wheel_delta = (s32)ev.dataSize;

				if(ev.value2 & GT_EVENT_MASK_MOUSE_LMB) lmb = true;
				if(ev.value2 & GT_EVENT_MASK_MOUSE_RMB) rmb = true;
				if(ev.value2 & GT_EVENT_MASK_MOUSE_MMB) mmb = true;
				if(ev.value2 & GT_EVENT_MASK_MOUSE_EXTRA1) ext1 = true;
				if(ev.value2 & GT_EVENT_MASK_MOUSE_EXTRA2) ext1 = true;

				if( ev.value2 & GT_EVENT_MASK_MOUSE_LMB_DBL ){
					gtLogWriter::printInfo( u"LMB DBL" );
				}

				if( ev.value2 & GT_EVENT_MASK_MOUSE_RMB_DBL ){
					gtLogWriter::printInfo( u"RMB DBL" );
				}

				if( wheel_delta > 0 ){
					gtLogWriter::printInfo( u"Wheel up [%i]", wheel_delta );
				}

				if( wheel_delta < 0 ){
					gtLogWriter::printInfo( u"Wheel down [%i]", wheel_delta );
				}

			}break;
		}
	}

	bool isKeyDown( gtKey key ){
		return m_keysDown[ (u32)key ];
	}

		/* отжатую клавишу нужно обработать только 1 раз. по этому если true то сброс */
		/* примерно так же можно сделать определение НАЖАТИЯ ТОЛЬКО 1 РАЗ. без зажима */
		/* это только пример как можно сделать */
	bool isKeyUp( gtKey key ){
		bool r = m_keysUp[ (u32)key ];
		if( r ) m_keysUp[ (u32)key ] = false; // сброс
		return r;
	}

	s32 mouse_x, mouse_y;
	s32 wheel_delta;
	bool lmb, rmb, mmb, ext1, ext2;


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



	gtImage * image1 = my_system->loadImage( gtFileSystem::getProgramPath() + u"png8.png" );
	gtImage * image2 = my_system->loadImage( gtFileSystem::getProgramPath() + u"png24.png" );
	gtImage * image3 = my_system->loadImage( gtFileSystem::getProgramPath() + u"png32.png" );
	gtImage * image4 = my_system->loadImage( gtFileSystem::getProgramPath() + u"png48.png" );
	
	gtPtr<gtTexture> texture1 = gtPtrNew<gtTexture>( driver1->createTexture( image1 ) );
	gtPtr<gtTexture> texture2 = gtPtrNew<gtTexture>( driver1->createTexture( image2 ) );
	gtPtr<gtTexture> texture3 = gtPtrNew<gtTexture>( driver1->createTexture( image3 ) );
	gtPtr<gtTexture> texture4 = gtPtrNew<gtTexture>( driver1->createTexture( image4 ) );

	/* software картинка больше не нужна */
	my_system->removeImage( image1 );
	my_system->removeImage( image2 );
	my_system->removeImage( image3 );
	my_system->removeImage( image4 );
	
	gtMaterial material1;
	material1.flags = gtMaterialFlag::MF_BLEND;
	material1.opacity = 1.f;
	material1.textureLayer[ 0u ].texture = texture1.data();

	gtMaterial material2;
	//material2.flags = gtMaterialFlag::MF_BLEND;
	material2.opacity = 0.5f;
	material2.textureLayer[ 0u ].texture = texture2.data();

	gtMaterial material3;
	material3.flags = gtMaterialFlag::MF_BLEND;
	material3.opacity = 0.5f;
	material3.textureLayer[ 0u ].texture = texture3.data();

	gtMaterial material4;
	//material4.flags = gtMaterialFlag::MF_BLEND;
	material4.opacity = 0.1f;
	material4.textureLayer[ 0u ].texture = texture4.data();

	
    while( my_system->update() ){


		if( events.isKeyDown( gtKey::K_ESCAPE ) ){
			break;
		}

		if( events.lmb ){
			gtLogWriter::printInfo( u"LMB" );
		}

		if( events.rmb ){
			gtLogWriter::printInfo( u"RMB" );
		}

		if( events.mmb ){
			gtLogWriter::printInfo( u"MMB" );
		}

		/* вывод координат когда будет нажата клавиша пробел */
		if( events.isKeyDown( gtKey::K_SPACE ) ){
			gtLogWriter::printInfo( u"Mouse [%i][%i]", events.mouse_x, events.mouse_y );
		}

		if( events.isKeyDown( gtKey::K_UP ) ){
			gtLogWriter::printInfo( u"UP" );
		}
		if( events.isKeyDown( gtKey::K_DOWN ) ){
			gtLogWriter::printInfo( u"DOWN" );
		}
		if( events.isKeyDown( gtKey::K_LEFT ) ){
			gtLogWriter::printInfo( u"LEFT" );
		}
		if( events.isKeyDown( gtKey::K_RIGHT ) ){
			gtLogWriter::printInfo( u"RIGHT" );
		}

		if( events.isKeyUp( gtKey::K_RIGHT ) ){
			gtLogWriter::printInfo( u"RIGHT UP" );
		}
		
        if( driver1 ){
            driver1->beginRender(true,gtColor(1.f,0.f,0.f,1.f));

			driver1->draw2DImage( v4i({ 0, 0, 400, 300 }), material1 );
			driver1->draw2DImage( v4i({ 400, 0, 800, 300 }), material2 );
			driver1->draw2DImage( v4i({ 0, 300, 400, 600 }), material3 );
			driver1->draw2DImage( v4i({ 400, 300, 800, 600 }), material4 );

			driver1->endRender();
        }
 
    }

//	if( texture )
	//	texture->release();

    return 0;
}

