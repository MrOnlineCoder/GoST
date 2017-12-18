//	GOST

#include "stdafx.h"

gtPluginRender::gtPluginRender( gtPluginInfoDL* info ){
	m_info = *info;
}

gtPluginRender::~gtPluginRender( void ){
	u32 sz = m_driver.size();
	for( u32 i = sz-1u; i >= 0u; --i  ){
		unloadDriver( i );
		if( !i ) break;
	}
}


void gtPluginRender::loadDriver( const gtDriverInfo& params ){
		
		if( !m_info.m_handle )
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
				

		m_driver.push_back(
			gtPluginRender::Driver( 
				(void(*)())loadProc,
				((gtLoadGPUDriver_t)loadProc)( gtMainSystemCommon::getInstance(), params )
			 ));

		if( m_driver[ m_driver.size() - 1u ].driver ){
		//	setHandle( m_driver.data(), m_info.m_handle );
			m_isLoad = true;

		}else{
			if( !m_driver.size() )
				GT_FREE_LIBRARY( m_info.m_handle );
		}
}

void gtPluginRender::unloadDriver( u32 id ){

	GT_ASSERT2( (id < m_driver.size()), "id < m_driver.size()" );

	if( m_isLoad ){

		if( m_driver[ id ].driver ){
			m_driver[ id ].driver->release();
			m_driver[ id ].driver = nullptr;
			m_driver.erase( m_driver.begin() + id );
		}

		if( !m_driver.size() ){
			GT_FREE_LIBRARY( m_info.m_handle );
			m_info.m_handle = nullptr;

			m_isLoad = false;
		}
	}
}


const gtPluginInfoDL&	gtPluginRender::getInfo( void ){
	return m_info;
}

	//	проверит есть ли нужные функции в плагине
bool gtPluginRender::checkLibraryFunctions( GT_LIBRARY_HANDLE lib ){
	gtLoadGPUDriver_t gtLoadGPUDriver = 	(gtLoadGPUDriver_t)GT_LOAD_FUNCTION(lib,"gtLoadGPUDriver");
	if( !gtLoadGPUDriver ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadGPUDriver", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	return true;
}

	//	загрузить плагин
void gtPluginRender::load( void ){
}

	//	выгрузитьплагин
void gtPluginRender::unload( void ){
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