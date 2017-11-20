//	GOST

#pragma once
#ifndef __GT_ERROR_H__
#define __GT_ERROR_H__

/*
	Здесь разные полезные вещи для выявления ошибок 
*/

namespace gost{

#ifdef GT_DEBUG

#	define GT_ASSERT1(expr,str,exprstr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed: %s", u##str );\
				gtLogWriter::printError(u"Expected: %s", u##exprstr );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };

#	define GT_ASSERT2(expr,exprstr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed" );\
				gtLogWriter::printError(u"Expected: %s", u##exprstr );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };

#	define GT_ASSERT3(expr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed" );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };


#else
#	define GT_ASSERT1(expr,str,exprstr)
#	define GT_ASSERT2(expr,exprstr)
#	define GT_ASSERT3(expr)
#endif

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