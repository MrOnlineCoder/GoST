//	GOST

#pragma once
#ifndef __GT_EVENT_H__
#define __GT_EVENT_H__

/*
*/

namespace gost{

#define GT_EVENT_WINDOW_SIZING   1u
#define GT_EVENT_WINDOW_RESTORE  2u
#define GT_EVENT_WINDOW_MAXIMIZE 3u
#define GT_EVENT_WINDOW_MINIMIZE 4u
#define GT_EVENT_WINDOW_MOVE     5u
#define GT_EVENT_WINDOW_PAINT    6u

		//	событие
	struct gtEvent{

		gtEvent( void ):
			type( ET_NONE ),
			value1( -1 ),
			value2( -1 ),
			dataSize( 0u ),
			data( nullptr )
		{}

		enum type_t{
			ET_NONE,
			ET_KEY,
			ET_CHAR,
			ET_MOUSE,
			ET_JOY,
			ET_GUI,
			ET_SYSTEM
		}type;
		
		s32 value1;
		s32 value2;

		u32 dataSize;
		void* data;

	};
	
	class gtEventConsumer{
	public:

		virtual void processEvent( const gtEvent& ev ) = 0;

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