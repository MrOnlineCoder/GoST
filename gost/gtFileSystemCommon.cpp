//	GOST


#include "stdafx.h"


gtFileSystemCommon::gtFileSystemCommon(){}
gtFileSystemCommon::~gtFileSystemCommon(){}

gtPtrNew<gtFile> gtFileSystem::createFile( const gtString& fileName, FileMode mode,
			FileAccessMode access,
			FileAction action,
			FileShareMode EFSM,
			u32 EFA
		){
	return gtPtrNew<gtFile>( gtMainSystemCommon::s_fileSystem->createFile( fileName, mode, access, action, EFSM, EFA ) );;
}

bool gtFileSystem::deleteFile( const gtString& fileName ){
	return gtMainSystemCommon::s_fileSystem->deleteFile( fileName );
}

bool gtFileSystem::deleteDir( const gtString& dir ){
	return gtMainSystemCommon::s_fileSystem->deleteDir( dir );
}

bool gtFileSystem::existFile( const gtString& fileName ){
	return gtMainSystemCommon::s_fileSystem->existFile( fileName );
}

	//	true ���� ����� ��������
bool gtFileSystem::existDir( const gtString& dir ){
	return gtMainSystemCommon::s_fileSystem->existDir( dir );
}

bool gtFileSystem::createDir( const gtString& dir ){
	return gtMainSystemCommon::s_fileSystem->createDir( dir );
}

//	//	�������� ���������� �������� � ��������� �����
//u32 gtFileSystem::getDirItemsCount( const gtString& dir ){
//	return gtMainSystemCommon::s_fileSystem->getDirItemsCount( dir );
//}

	//	����� ������������� ����� ����� ������� ���
void gtFileSystem::scanDirBegin( const gtString& dir ){
	gtMainSystemCommon::s_fileSystem->scanDirBegin( dir );
}
			
	//	����� ���������� ������������ ����� ������� ���
void gtFileSystem::scanDirEnd( void ){
	gtMainSystemCommon::s_fileSystem->scanDirEnd();
}

	//	�������� ��������� DirObject ���� ���� ����/�����
	//	����, �����/����� ���, ��� ������������ ����� ��������� - ������� false
bool gtFileSystem::getDirObject( gtFileSystem::DirObject* o ){
	return gtMainSystemCommon::s_fileSystem->getDirObject( o );
}

	//	�������� ������������ ����. true ���� �����
bool gtFileSystem::copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite ){
	return gtMainSystemCommon::s_fileSystem->copyFile( existingFileName, newFileName, overwrite );
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