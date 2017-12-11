//	GOST

#include "stdafx.h"


gtPluginSystemImpl::gtPluginSystemImpl( void ):
	m_numOfPlugins( 0u )
{
}


gtPluginSystemImpl::~gtPluginSystemImpl( void ){
	for each ( auto var in m_renderPluginCache ){
		if( var.m_handle ){
			GT_FREE_LIBRARY( var.m_handle );
		}
	}
	m_renderPluginCache.clear();
}


void gtPluginSystemImpl::scanFolder( const gtString& dir ){
	gtArray<gtFileSystem::DirObject> objs;

	gtFileSystem::scanDirBegin( dir );
	gtFileSystem::DirObject dob;
	while( gtFileSystem::getDirObject( &dob ) ){
		objs.push_back( dob );
	}
	gtFileSystem::scanDirEnd();

	u32 sz = objs.size();

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &objs[ i ];
		if( o->type == gtFileSystem::DirObjectType::file ){
			gtString extension = util::stringGetExtension<gtString>( gtString((char16_t*)o->path) );
			if( extension == u"gpl" || extension == u"gpld" ){
				 
#ifdef GT_DEBUG
				if( extension.size() == 3 ) continue;
#else
				if( extension.size() == 4 ) continue;
#endif


				gtPluginInfoDL pi_dl;

				GT_LIBRARY_HANDLE lib = GT_LOAD_LIBRARY(o->path);
				if( !lib ){
					gtLogWriter::printWarning( u"Can not load plugin [%s]", o->path );
					continue;
				}

				gtGetPluginInfo f_GetPluginInfo = (gtGetPluginInfo)GT_LOAD_FUNCTION( lib, "GetPluginInfo" );
				if( !f_GetPluginInfo ){
					gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"GetPluginInfo", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}

				gtPluginInfo pi;
				f_GetPluginInfo( pi );
				
				//	пока добавляется только общая информация
				pi_dl.m_path.assign((char16_t*)o->path);
				pi_dl.m_info = pi;
				
				if( pi.m_type == gtPluginType::unknown ){
					gtLogWriter::printWarning( u"Unsupported plugin [%s]", o->path );
				}else if( pi.m_type == gtPluginType::render ){
					m_numOfPlugins++;

					gtLoadGPUDriver_t gtLoadGPUDriver = 	(gtLoadGPUDriver_t)GT_LOAD_FUNCTION(lib,"gtLoadGPUDriver");
					if( !gtLoadGPUDriver ){
						gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadGPUDriver", o->path );
						GT_FREE_LIBRARY( lib );
						continue;
					}
										
					this->m_renderPluginCache.push_back( pi_dl );
				}


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

gtDriver*	gtPluginSystemImpl::loadRenderPlugin( const gtDriverInfo& params ){
	gtDriver* ret(nullptr);

	u32 sz = m_renderPluginCache.size();

	if( !sz ){
		gtLogWriter::printError( u"Can not load render plugin [%s]. No plugin.", params.m_GUID.data() );
		return ret;
	}

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &m_renderPluginCache[ i ];
		if( o->m_info.m_GUID == params.m_GUID ){

			gtPtr<gtPluginRender> render = gtPtrNew<gtPluginRender>( new gtPluginRender( o ) );
			render->load( params );

			ret = render->m_driver.data();

			m_renderPlugins.add( render.data() );
			render->addRef();

			break;
		}
	}
	
	
	return ret;
}

void gtPluginSystemImpl::unloadRenderPlugin( gtDriver* d ){
	u32 sz = m_renderPlugins.size();

	for( u32 i = 0u; i < sz; ++i ){
		auto  o = m_renderPlugins.get( i );

		if( o->m_driver.data() == d ){
			m_renderPlugins.remove( o );
			o->unload();
			delete o;
			break;
		}

	}
}

void gtPluginRender::load( const gtDriverInfo& params ){
	if( !m_isLoad ){

		if( m_info.m_handle )
			GT_FREE_LIBRARY( m_info.m_handle );

		m_info.m_handle	=	GT_LOAD_LIBRARY( (wchar_t*)m_info.m_path.data() );

		if( !m_info.m_handle ){
			gtLogWriter::printWarning( u"Can not load plugin [%s]", m_info.m_path.data() );
			return;
		}

		gtLoadGPUDriver_t loadProc = (gtLoadGPUDriver_t)GT_LOAD_FUNCTION( m_info.m_handle, "gtLoadGPUDriver" );
		if( !loadProc ){
			gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadGPUDriver", m_info.m_path.data() );
			GT_FREE_LIBRARY( m_info.m_handle );
			return;
		}
				
		m_info.m_loadPlugin = (void(*)())loadProc;

		m_driver = gtPtrNew<gtDriver>( ((gtLoadGPUDriver_t)m_info.m_loadPlugin)( gtMainSystemCommon::getInstance(), params ) );

		if( m_driver.data() ){
		//	setHandle( m_driver.data(), m_info.m_handle );
			m_isLoad = true;

		}else{
			GT_FREE_LIBRARY( m_info.m_handle );
		}
	}
}

void gtPluginRender::unload( void ){
	if( m_isLoad ){

		if( m_driver.data() )
			m_driver->release();

		GT_FREE_LIBRARY( m_info.m_handle );
		m_info.m_handle = nullptr;

		m_isLoad = false;
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