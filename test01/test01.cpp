// test01.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"

#include <gost.h>

#pragma comment(lib, "gost_d.lib")

using namespace gost;

class myOutput : public gtOutputWindow {
	HWND m_hWnd;
	bool m_isInit;
public:

	myOutput( void ) : m_hWnd( 0 ), m_isInit( false ){
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

#if defined( GT_PLATFORM_WIN32 )
#include <Windows.h>
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
#endif

	

	gtDeviceCreationParameters params;

	myOutput output;

//#define O1
#ifdef O1
	output.init();
	output.setWindowText( u"Output" );
	params.m_outputWindow = &output;
#endif

	gtPtr<gtMainSystem> my_system( gtPtrNew<gtMainSystem>( InitializeGoSTEngine(params) ) );
	
//	gtStackTrace::dumpStackTrace();

//	GT_ASSERT1( 1 == 2, "bad expr", "1 == 2" );

	while( my_system->update() ){

	}

	output.shutdown();

    return 0;
}

