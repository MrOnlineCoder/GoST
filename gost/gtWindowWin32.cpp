//	GOST

#include "stdafx.h"

gtWindowWin32::gtWindowWin32( const gtWindowInfo& wi )
	:	m_hWnd( 0u ), m_isInit( false )
{
	m_params = wi;
}

gtWindowWin32::~gtWindowWin32( void ){
	if( m_isInit ){
		if( m_hWnd ) DestroyWindow( m_hWnd );
		UnregisterClass( (wchar_t*)m_className.data(), GetModuleHandle( 0 ) );
	}
}

bool	gtWindowWin32::init( u32 i ){

	GT_ASSERT1( !m_isInit, "Window is initialized", "!m_isInit" );
	if( m_isInit ) return false;

	DWORD style = WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX; // DWORD - это unsigned long

	if( m_params.m_style & gtWindowInfo::style_::_popup ){
		style = WS_POPUP;
	}else{
		if( m_params.m_style & gtWindowInfo::style_::_maximize )
			style |= WS_MAXIMIZEBOX;
		if( m_params.m_style & gtWindowInfo::style_::_resize )
			style |= WS_SIZEBOX;
	}

	


	m_className = u"GTWINDOW_";
	m_className += i;

	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) ); // memset Winows style
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= GetModuleHandle( 0 );
	wc.hIcon			= nullptr;
	wc.hCursor			= nullptr; // LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_MENU+1);
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= (wchar_t*)m_className.data();
	wc.hIconSm			= nullptr;

	if( RegisterClassEx(&wc) == 0 ){
		// MSDN: If the function fails, the return value is zero. 
		gtLogWriter::printError( u"Can not register window class. Error code[%u]", (u32)GetLastError() );
		return false;
	}
	
	m_hWnd = CreateWindow( (wchar_t*)m_className.data(),
		(wchar_t*)m_params.m_title.data(),
		style,
		m_params.m_rect.getComponent(0u),
		m_params.m_rect.getComponent(1u),
		m_params.m_rect.getComponent(2u),
		m_params.m_rect.getComponent(3u),
		nullptr,
		nullptr,
		wc.hInstance,
		this );

	if( !m_hWnd ){
		gtLogWriter::printError( u"Can not create window. Error code[%u]", (u32)GetLastError() );
		return false;
	}
	 
	ShowWindow( m_hWnd, SW_SHOWNORMAL  );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );
	UpdateWindow( m_hWnd );
	
	
	m_isInit = true;
	return true;
}

	//	Установит заголовок окна
void	gtWindowWin32::setWindowTitle( const gtString& title ){
	if( !m_hWnd){
		gtLogWriter::printWarning( u"Can not set window title" );
		gtLogWriter::printWarning( u"HWND == 0" );
		gtStackTrace::dumpStackTrace();
		return;
	}


	if( SetWindowText( m_hWnd, (wchar_t*)title.data() ) == FALSE ){
		gtLogWriter::printWarning( u"Can not set window title. Code [%u]\n", (u32)GetLastError() );
		gtStackTrace::dumpStackTrace();
		return;
	}
}

void*	gtWindowWin32::getHandle( void ){
	return (void*)m_hWnd;
}

LRESULT CALLBACK gtWindowWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC hdc;
	gtWindowWin32* pD = nullptr;

	s32 wmId    = LOWORD(wParam);
	s32 wmEvent = HIWORD(wParam);


	gtEvent ev;
	ev.type = gtEvent::ET_SYSTEM;

	if( !pD ){
		if (message == WM_NCCREATE)   {
			pD = static_cast<gtWindowWin32*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, -21, reinterpret_cast<LONG_PTR>(pD)))
				if (GetLastError() != 0)             return FALSE;
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else{
			pD = reinterpret_cast<gtWindowWin32*>(GetWindowLongPtr(hWnd, -21));
		}
	}

	switch( message ){
	case WM_COMMAND:
		// Parse the menu selections:
		switch (wmId)
		{
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_QUIT:
		PostQuitMessage( 0 );
		break;

	case WM_MOVE:
		ev.value1 = GT_EVENT_WINDOW_MOVE;
	break;
	case WM_PAINT:
		ev.value1 = GT_EVENT_WINDOW_PAINT;
		break;
	case WM_SIZE:{
		switch( wmId ){
		case SIZE_MAXIMIZED:
			ev.value1 = GT_EVENT_WINDOW_MAXIMIZE;
			break;
		case SIZE_MINIMIZED:
			ev.value1 = GT_EVENT_WINDOW_MINIMIZE;
			break;
		case SIZE_RESTORED:
			ev.value1 = GT_EVENT_WINDOW_RESTORE;
			break;
		}
	}break;
	case WM_SIZING:
			ev.value1 = GT_EVENT_WINDOW_SIZING;
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}

	gtMainSystem::getInstance()->addEvent( ev );

	return DefWindowProc( hWnd, message, wParam, lParam );
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