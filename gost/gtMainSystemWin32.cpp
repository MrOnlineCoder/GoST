//	GOST 

#include "stdafx.h"

namespace gost{
	
		///	конструктор
	gtMainSystemWin32::gtMainSystemWin32( const gtDeviceCreationParameters& params ){
		m_params = params;
	}

		///	деструктор
	gtMainSystemWin32::~gtMainSystemWin32( void ){
	}

		///	возвратит укозатель на окно вывода
	gtOutputWindow* gtMainSystemWin32::getOutputWindow( void ){
		return this->m_output_window.data();
	}

		///	инициализация системы
	bool	gtMainSystemWin32::init( void ){

		{		
			if( !m_params.m_outputWindow ){
				this->m_output_window = gtPtrNew<gtOutputWindow>( new gtOutputWindowWin32 );
				this->m_output_window->init();
				this->m_output_window->show();
				m_params.m_outputWindow = this->m_output_window.data();
			}else{
				this->m_output_window = gtPtrNew<gtOutputWindow>( m_params.m_outputWindow );
			}
			
			gtLog->setOutputWindow( m_output_window.data() );
			gtLog->print(gtLoger::level::info, u"Starting GoST version %i.%i", 0, 1 );

			this->initStackTracer();
			
		}
		
		return true;
	}

		///	получает и обрабатывает оконные сообщения
	void gtMainSystemWin32::updateWindowEvents( void ){
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage( &msg, NULL, 0, 0 )){
				this->quit();
			}
			TranslateMessage (&msg);
		  	DispatchMessage (&msg);
		}
	}


	bool	gtMainSystemWin32::update( void ){
		this->updateWindowEvents();
		return m_isRun;
	}

		///	завершает работу
	void gtMainSystemWin32::quit( void ){
		m_isRun = false;
	}
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