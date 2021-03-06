﻿//	GOST

/* 
	Здесь будут экспортируемые функции для запуска движка
*/
#include "stdafx.h"

#include <gost.h>

namespace gost{

GT_API gtMainSystem* InitializeGoSTEngine( const gtDeviceCreationParameters& params ){

	gtMainSystem*	main_system( nullptr );

	switch( params.m_device_type ){
#if defined( GT_PLATFORM_WIN32 )
		case gtDeviceType::windows:{
			main_system = new gtMainSystemWin32( params );

			if( !((gtMainSystemWin32*)main_system)->init() ){
				
				MessageBox( 0, L"Can not initialize GoST", L"Error", MB_OK | MB_ICONERROR );

				delete main_system;
				main_system = nullptr;
			}
			
		}
		break;
#endif
		case gtDeviceType::android:
		case gtDeviceType::ios:
		case gtDeviceType::linux:
		case gtDeviceType::osx:
		case gtDeviceType::playstation:
		case gtDeviceType::wii:
		case gtDeviceType::xbox:
		default:
			return nullptr;
			break;
	}

#ifdef GT_DEBUG
	if( main_system )
		main_system->setDebugName( u"MainSystem" );
#endif

	return main_system;
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