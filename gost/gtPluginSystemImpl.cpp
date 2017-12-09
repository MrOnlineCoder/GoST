//	GOST

#include "stdafx.h"


gtPluginSystemImpl::gtPluginSystemImpl( void ):
	m_numOfPlugins( 0u )
{
}


gtPluginSystemImpl::~gtPluginSystemImpl( void ){
}


void gtPluginSystemImpl::scanFolder( const gtString& dir ){
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
		if( o->type == gtFileSystem::DirObjectType::file ){
			gtString extension = util::stringGetExtension<gtString>( gtString((char16_t*)o->path) );
			if( extension == u"gpl" ){

				GT_LIBRARY_HANDLE lib = GT_LOAD_LIBRARY(o->path);
				if( !lib ){
					gtLogWriter::printWarning( u"Can not load plugin [%s]", o->path );
					continue;
				}

				gtGetPlugunInfo f_GetPluginInfo = (gtGetPlugunInfo)GT_LOAD_FUNCTION( lib, "getInfo" );
				if( !f_GetPluginInfo ){
					gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"getInfo", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}

				gtPluginInfo pi = f_GetPluginInfo();

				if( pi.m_type == gtPluginType::unknown ){
					gtLogWriter::printWarning( u"Unsupported plugin [%s]", o->path );
				}else 
					m_numOfPlugins++;

				GT_FREE_LIBRARY( lib );

			}
		}
	}

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &objs[ i ];

		if( o->type == gtFileSystem::DirObjectType::folder ){
			scanFolder( (char16_t*)o->path );
		}
	}
}

bool gtPluginSystemImpl::init( void ){

	gtString pluginsDir( gtFileSystem::getProgramPath() + u"plugins/" );

	if( !gtFileSystem::existDir( pluginsDir ) ){
		gtLogWriter::printError( u"Plugins folder not found" );
		return false;
	}

	scanFolder( pluginsDir );

	return true;
}

	//	получить количество плагинов в папке plugins
u32	gtPluginSystemImpl::getNumOfPlugins( void ){
	return m_numOfPlugins;
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