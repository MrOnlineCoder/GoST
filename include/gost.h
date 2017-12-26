//	GOST

#pragma once
#ifndef __GOST_H__
#define __GOST_H__

/*
	Главный include файл движка GOST 
*/

//	C++
//#include <cctype> //isalpha
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

//	GOST 
#include <gtInfo.h>
#include <gtTypes.h>

#include <gtInput.h>
#include <gtEvent.h>

#include <gtString.h>

#include <gtUtilities.h>

#include <gtError.h>

#include <gtList.h>
#include <gtPriorQueue.h>

#include <gtVector.h>
#include <gtColor.h>

#include <gtVertex.h>

#include <gtBaseObject.h>
#include <gtRefObject.h>
#include <gtPtr.h>

#include <gtMainSystem.h>

#include <gtImage.h>
#include <gtTexture.h>
#include <gtShader.h>
#include <gtMaterial.h>

#include <gtTimer.h>

#include <gtDriver.h>

#include <gtPlugin.h>
#include <gtPluginSystem.h>

#include <gtWindow.h>

#include <gtOutputWindow.h>

#include <gtLoger.h>

#include <gtFileSystem.h>





#define GT_USE_STACK_TRACE
#include <gtStackTrace.h>

namespace gost{

	///	Инициализация 
GT_API gtMainSystem* InitializeGoSTEngine( const gtDeviceCreationParameters& );

}


#endif

/*
Copyright (c) 2017

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