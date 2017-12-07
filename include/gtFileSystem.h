//	GOST

#pragma once
#ifndef __GT_FILE_SYSTEM_H__
#define __GT_FILE_SYSTEM_H__

/*
*/

namespace gost{

		//	чтобы не мучится в выделении памяти в разных модулях (.dll)
		//	проще использовать массивы с заранее выделенной паматью.
		//	Размер массива который будет хранить путь к файлу
	constexpr u32 GT_MAX_PATH = 256u;


		//	Интерфейс для работы с файлом
	class gtFile : public gtRefObject{
	public:

			//	Если файл открыт в текстовом режиме (подразумевается что открываемый файл - текстовый)
			//		то происходит определение формата текстового файла
		struct TextFileInfo{

				//	формат текста
			enum class format{

				//	utf-8
				utf_8,

				//	стандартный для gost
				utf_16,

				//	на будущее
				utf_32

				// ansi нет из за сложности определения кодовых страниц
				// если файл - ansi, то открыается как utf-8
			}m_format;

				// Порядок байтов для utf-16 и utf-32
			enum class endian{

				//	0xFFFE0000
				little,

				//	0x0000FEFF
				big

			}m_endian;

				//	Есть ли маркер последовательности байтов
			bool m_hasBOM;
		};

			//	если файл текстовый то вернётся актуальная информация о текстовом файле
		virtual TextFileInfo	getTextFileInfo( void ) = 0;
			
			//	для двоичной записи
		virtual void	write( u8 * data, u32 size ) = 0;

			//	для текста. Работает если файл открыт в текстовом режиме.
		virtual void	write( const gtStringA& string ) = 0;

			//	для текста. Работает если файл открыт в текстовом режиме.
		virtual void	write( const gtString& string ) = 0;

			//	для текста. Работает если файл открыт в текстовом режиме.
		virtual void	write( const gtString32& string ) = 0;

			//	всё что в буффере запишется в файл
		virtual void	flush( void ) = 0;

			//	размер в байтах
		virtual u32		size( void ) = 0;

			//	получить позицию указателя
		virtual u32		tell( void ) = 0;

			//	для установки позиции курсора
			//	начинать отсчёт дистанции с
		enum SeekPos{

			//	начала
			ESP_BEGIN,

			//	с текущей позиции
			ESP_CURRENT,

			//	с конца(хз зачем)
			ESP_END
		};

			//	установить позицию указателя
		virtual void	seek( u32 distance, SeekPos pos ) = 0;
	};

	//	для краткости
#define gtFile_t gtPtr<gtFile>
	
	//	класс для работы с файлами и папками
	class gtFileSystem{
	public:

			//	Режим доступа к файлу
		enum FileAccessMode{

			//	только чтение
			EFAM_READ,

			//	только запись
			EFAM_WRITE,

			//	чтение и запись
			EFAM_BOTH,
			
			// добавление в конец файла, для текстового режима
			EFAM_APPEND
		};

			//	Режим открытия файла
			//	По сути TEXT это расширение над BINARY
			//	то есть в текстовом режиме можно записать двоичные данные
		enum FileMode{

				//	для того чтобы открыть текстовый файл и получить информацию о тексте
			EFM_TEXT,

				//	для того чтобы открыть файл для чтения/записи двоичных данных
			EFM_BINARY
		};

			//	Может ли открытый файл использоваться другими процессами
		enum FileShareMode{

			//	файл доступен только этому процессу
			EFSM_NONE,

			//	файл доступен другим процессам для удаления
			EFSM_DELETE,

			//	файл доступен другим процессам для чтения
			EFSM_READ,

			//	файл доступен другим процессам для записи
			EFSM_WRITE
		};

		enum FileAction{

			//	создаёт/открывает новый файл, сохраняя содержимое если оно есть
			EFA_OPEN,

			//	всегда создаёт/открывает новый файл, удаляя содержимое если оно есть
			EFA_OPEN_NEW,

		};

			//	Аттрибуты для файла
		enum FileAttribute{

				//	обычный файл
			EFA_NORMAL,

				//	скрытый файл
			EFA_HIDDEN,

				//	файл только для чтенияы
			EFA_READONLY
		};

			//	Создаёт или открывает(если доступен) файл
		GT_API static gtPtrNew<gtFile> createFile( 
			const gtString& fileName,
			FileMode mode,
			FileAccessMode access,
			FileAction action,
			FileShareMode = FileShareMode::EFSM_NONE,
			u32 attributeFlags = FileAttribute::EFA_NORMAL
		);

			//	если файл доступен, он будет удалён
		GT_API static bool deleteFile( const gtString& file );

			//	если папка доступна, она будет удалена
		GT_API static bool deleteDir( const gtString& dir );

			//	true если файл доступен
		GT_API static bool existFile( const gtString& file );

			//	true если папка доступна
		GT_API static bool existDir( const gtString& dir );

			//	true если папка создана
		GT_API static bool createDir( const gtString& dir );

			//	тип объекта. нужно для сканирования папки
		enum class DirObjectType{
			
			// для точек . ..
			info,

			//	папка
			folder, 

			//	файл
			file 
		};

			//	объект дирректории
		struct DirObject{
			// конструктор
			DirObject( void ){
				memset( path, 0u, GT_MAX_PATH * sizeof(wchar_t) );
			}

				//	полный путь
			wchar_t path[GT_MAX_PATH];

				//	тип
			DirObjectType type;

				//	размер
			u32 size;
		};

			//	перед сканированием папки нужно вызвать это
		GT_API static void scanDirBegin( const gtString& dir );

			//	после завершения сканирования нужно вызвать это
		GT_API static void scanDirEnd( void );

			//	заполнит структуру DirObject если есть файл/папка
			//	если, файла/папки нет, или сканирование папки завершено - возврат false
		GT_API static bool getDirObject( DirObject* );

			//	копирует существующий файл. true если успех
		GT_API static bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );


	};

	namespace util{
	
		/*
			Укороченные версии функций.
			Нужно будет дополнить для двоичных файлов.
		*/

		template<typename T>
		gtPtrNew<gtFile> _openFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		template<typename T>
		gtPtrNew<gtFile> _createFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		template<typename T>
		gtPtrNew<gtFile> _openFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_APPEND, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		template<typename T>
		gtPtrNew<gtFile> _createFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_WRITE,
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		#define openFileForReadText _openFileForReadText<s8>
		#define createFileForReadText _createFileForReadText<s8>
		#define openFileForWriteText _openFileForWriteText<s8>
		#define createFileForWriteText _createFileForWriteText<s8>

	}


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