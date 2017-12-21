//	GOST

#pragma once
#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"
#pragma warning(disable : 4002)	//	to many actual parameters for macro
#pragma warning(disable : 4091) // dbghelp.h 
#pragma warning(disable : 4668)
#pragma warning(disable : 4710)
#pragma warning(disable : 4777) // _snprintf %i
#pragma warning(disable : 4477) // _snprintf %#010x
#pragma warning(disable : 4820)
#pragma warning(disable : 4826)	//	conversion from 'unsigned long *' to DWORD64 is sign-extended
#pragma warning(disable : 4917)


// Windows Header Files:
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib,"winmm.lib")

#define GT_EXPORTS

#include <gost.h>

using namespace gost;

//	GOST
#include "gtOutputWindowWin32.h"

#include "gtFileWin32.h"

#include "gtFileSystemCommon.h"
#include "gtFileSystemWin32.h"

#include "gtPluginSystemImpl.h"

#include "gtPluginRender.h"
#include "gtPluginImportImage.h"

#include "gtTimerWin32.h"

#include "gtMainSystemCommon.h"
#include "gtMainSystemWin32.h"

#include "gtLogerImpl.h"


#include "gtWindowCommon.h"
#include "gtWindowWin32.h"



#endif

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