// test01.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"

#include <gost.h>

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

using namespace gost;

class myOutput : public gtOutputWindow {
	HWND m_hWnd;
	bool m_isInit;
	
	~myOutput( void ){	}

public:

	myOutput( void ) : m_hWnd( 0 ), m_isInit( false ){
		this->setDebugName( u"OutputWindow" );
	}


	//	Прежде чем использовать нужно вызвать Init
	void	init( void ){
		if( !AllocConsole() ){
			return;
		}

		freopen("CONOUT$", "w", stdout);

		m_hWnd = GetConsoleWindow();

		m_isInit = true;
	}

		//	При завершении работы нужно вызвать этот метод
	void	shutdown( void ){
		if( m_isInit ){
			if( FreeConsole() ){
				m_isInit = false;
			}
		}
	}

		//	Возвратит true если инициализирован
	bool	isInit( void ){
		return m_isInit;
	}

		//	Если окно невидимо, покажет его
	void	show( void ){
	}

		//	Сделает окно невидимым (спрячет, или сделает прозрачным, зависит от реализации)
	void	hide( void ){
	}

		//	Напечатает текст
	void	print( const gtString& s ){
		if( m_isInit )
			wprintf( L"%s\n", (wchar_t*)s.data() );
	}

		//	Установит заголовок
	void	setWindowText( const gtString& s ){
		if( m_isInit )
			SetWindowText( m_hWnd, (wchar_t*)s.data() );
	}
};

void ScanDirs( gtString dir, bool subDir ){

	//	сначала получаем все файлы и папки текущей дирректории
	//	потом рекурсивно проходимся по папкам
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
		if( o->type == gtFileSystem::DirObjectType::info ) 
			continue;

		gtLogWriter::printInfo( 
			u"%s - %s - %u bytes", 
			o->type == gtFileSystem::DirObjectType::folder
				? u"Dir" : u"File",
			o->path, o->size );

		if( o->type == gtFileSystem::DirObjectType::folder ){
			if( subDir ){
				ScanDirs( (char16_t*)o->path, true );
			}
		}
	}

}


#if defined( GT_PLATFORM_WIN32 )
#include <Windows.h>
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
#endif
		
	gtDeviceCreationParameters params;


	myOutput * output = new myOutput;

//#define O1
#ifdef O1
	output->init();
	output->setWindowText( u"Output" );
	params.m_outputWindow = output;
#endif

	gtPtr<gtMainSystem> my_system( gtPtrNew<gtMainSystem>( InitializeGoSTEngine(params) ) );

		
	gtWindowInfo wi;
	
	gtPtr< gtWindow > window;

	wi.m_style = gtWindowInfo::_standart;
	wi.m_style |= gtWindowInfo::_maximize;
	wi.m_style |= gtWindowInfo::_resize;

	window = gtPtrNew<gtWindow>( my_system->createSystemWindow( wi ) );


	////	СКАНИРОВАНИЕ ПАПКИ
	/*gtFileSystem::scanDirBegin( u"D:\\GOST\\gost\\" );
	
	gtFileSystem::DirObject dirObj;

	while( gtFileSystem::getDirObject( &dirObj ) ){
		
		if( dirObj.type == gtFileSystem::DirObjectType::info ) continue;

		gtLogWriter::printInfo( u"%s - %s - %u bytes", dirObj.type == gtFileSystem::DirObjectType::folder ? u"Dir" : u"File",
			dirObj.path, dirObj.size );
	}

	gtFileSystem::scanDirEnd();*/
	ScanDirs( u"D:\\GOST\\gost\\", true );

//	gtFileSystem::copyFile( u"consola.ttf", u"consola_copy.ttf", false );
	{
		gtFile_t f = gtFileSystem::createFile(
					u"gost_d.dll",
					gtFileSystem::FileMode::EFM_BINARY,
					gtFileSystem::FileAccessMode::EFAM_READ,
					gtFileSystem::FileAction::EFA_OPEN );
		gtLogWriter::printInfo( u"File size [%u]", f->size() );
	}


	{// тест seek и tell
		gtFile_t f = gtFileSystem::createFile(
					u"out.bin",
					gtFileSystem::FileMode::EFM_BINARY,
					gtFileSystem::FileAccessMode::EFAM_WRITE,
					gtFileSystem::FileAction::EFA_OPEN_NEW );
		gtLogWriter::printInfo( u"File cur pos [%u]", f->tell() );

		s32 i = 0xFFFFFFFF;
		f->write( reinterpret_cast<u8*>(&i), sizeof(s32) );

		gtLogWriter::printInfo( u"File cur pos [%u]", f->tell() );

		i = 0xAAAAAAAA;
		f->write( reinterpret_cast<u8*>(&i), sizeof(s32) );
		gtLogWriter::printInfo( u"File cur pos [%u]", f->tell() );

		f->seek( 2u, gtFile::SeekPos::ESP_BEGIN );

		i = 0x44444444;
		f->write( reinterpret_cast<u8*>(&i), sizeof(s32) );
		gtLogWriter::printInfo( u"File cur pos [%u]", f->tell() );
	}

	while( my_system->update() ){

	}

	output->shutdown();
	output->release();


    return 0;
}

