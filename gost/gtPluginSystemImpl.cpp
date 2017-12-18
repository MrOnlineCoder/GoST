//	GOST

#include "stdafx.h"


gtPluginSystemImpl::gtPluginSystemImpl( void ):
	m_numOfPlugins( 0u )
{
}


gtPluginSystemImpl::~gtPluginSystemImpl( void ){}


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
				}else{
					gtLogWriter::printInfo( u"Add plugin: %s", pi.m_name.data() );
					gtLogWriter::printInfo( u"Description: %s", pi.m_description.data() );
				}
				
				if( pi.m_type == gtPluginType::unknown ){
				}else if( pi.m_type == gtPluginType::render ){
					
					gtPtr<gtPluginRender> plugin = gtPtrNew<gtPluginRender>( new gtPluginRender( &pi_dl ) );

					if( !plugin->checkLibraryFunctions( lib ) )
						continue;
										
					m_numOfPlugins++;

					this->m_renderPluginCache.push_back( plugin.data() );

				}else if( pi.m_type == gtPluginType::import_image ){

					gtPtr<gtPluginImportImage> plugin = gtPtrNew<gtPluginImportImage>( new gtPluginImportImage( &pi_dl ) );

					if( !plugin->checkLibraryFunctions(lib))
						continue;

					m_numOfPlugins++;

					this->m_importImagePluginCache.push_back( plugin.data() );
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
	
	u32 sz = m_renderPluginCache.size();

	if( !sz ){
		gtLogWriter::printError( u"Can not load render plugin [%s]. No plugin.", params.m_GUID.data() );
		return nullptr;
	}

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &m_renderPluginCache[ i ];
		if( o->data()->getInfo().m_info.m_GUID == params.m_GUID ){
			
			o->data()->loadDriver( params );
			
			return o->data()->m_driver[ o->data()->m_driver.size() - 1u ].driver;
		}
	}
	
	
	return nullptr;
}

void gtPluginSystemImpl::unloadRenderPlugin( gtDriver* d ){
	u32 sz = m_renderPluginCache.size();

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &m_renderPluginCache[ i ];

		for( u32 j = 0u; j < o->data()->m_driver.size(); ++j ){
			if( o->data()->m_driver[ j ].driver == d ){
				o->data()->unloadDriver( j );
				break;
			}
		}
	}
}

	//	загружает картинку
gtImage * gtPluginSystemImpl::importImage( const gtString& fileName, const gtString& guid, bool useguid){

	if( !gtFileSystem::existFile( fileName ) ){
		gtLogWriter::printWarning( u"Can not load image [%s]. File not exist.", fileName.data() );
		return nullptr;
	}

	gtString file = fileName;
	util::stringFlipSlash( file );
	
	gtString ext = util::stringGetExtension( file );
	util::stringToLower( ext );
	
	gtImage * image = new gtImage;

	u32 sz = this->m_importImagePluginCache.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &this->m_importImagePluginCache[ i ];

		if( useguid ){
			if( o->data()->getInfo().m_info.m_GUID == guid ){
				o->data()->load();
				o->data()->loadImage( &file, &image );
				break;
			}
		}else{
			u32 esz = o->data()->m_extensions.size();
			for( u32 j = 0u; j < esz; ++j ){
				if( o->data()->m_extensions[ j ] == ext ){
					o->data()->load();
					o->data()->loadImage( &file, &image );
					if( image->data ) break;
				}
			}
		}

	}

	if( image->data ){
		gtLogWriter::printInfo(u"Loading image: %s", fileName.data());
		return image;
	}

	delete image;
	return nullptr;
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