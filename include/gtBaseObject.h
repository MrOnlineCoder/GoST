﻿//	GOST

#pragma once
#ifndef __GT_BASE_OBJECT_H__
#define __GT_BASE_OBJECT_H__

/*
	Общий класс. 
*/

namespace gost{

	//	Все объекты движка наследуют его через gtRefObject
	class gtBaseObject{
	protected:

		//	ненужно создавать объект вручную
		GT_FORCE_INLINE gtBaseObject( void ){}

#if defined(GT_DEBUG)
		//	в дебагере, в методе gtPtr::release может понадобится знать, какой объект уничтожается
		gtString m_debugName;
#endif

	public:


		virtual ~gtBaseObject( void ){}

		
		virtual void first_vtable_function( void ){}


#if defined(GT_DEBUG)
		virtual void setDebugName( const gtString& s ){ m_debugName = s; }
#endif

	};

}

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