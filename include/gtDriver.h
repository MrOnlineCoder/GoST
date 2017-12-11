//	GOST

#pragma once
#ifndef __GT_DRIVER_H__
#define __GT_DRIVER_H__

/*
*/

namespace gost{

	class gtWindow;


		//	параметры запуска драйвера
	struct gtDriverInfo{

		gtDriverInfo( void ):
			m_GUID(u"-"),
			m_fullScreen( false ),
			m_stencilBuffer( true ),
			m_doubleBuffer( true ),
			m_vSync( false ),
			m_colorDepth( 32u ),
			m_adapterID( 0u ),
			m_outWindow( nullptr )
		{
			m_backBufferSize.set({800,600});
		}

			//	ID драйвера
		gtString m_GUID;

			//	разрешение экрана
		v2i		m_backBufferSize;

			//	запускать ли программу в полноэкранном режиме
		bool	m_fullScreen;

			//	использовать ли стенсильный буффер. Нужен для OGL, в D3D по умолчанию есть.
		bool	m_stencilBuffer;

			//	двойная буферизация
		bool	m_doubleBuffer;

			//	вертикльная синхронизация
		bool	m_vSync;

			//	глубина цвета - биты на пиксель
		u8		m_colorDepth;

			// only for Direct3D
		u32		m_adapterID;

			//	окно для рисования
		gtWindow * m_outWindow;
	};

		//	видео драйвер
	class gtDriver : public gtRefObject{
	public:

			//	получить параметры
		virtual const gtDriverInfo&	getParams( void ) = 0;
			
			//	
		virtual void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) ) = 0;
		virtual void endRender( void ) = 0;

		virtual void *	getPluginHandle( void ) = 0;
	};

		//	реализация драйвера в отдельных dll
		//	по этому создавать общий класс вижу только таким способом
	class gtDriverCommon : public gtDriver{

	protected:

			//	параметры запуска
		gtDriverInfo m_params;
			
			/*временно*/
		void * m_handle;

	public:

		gtDriverCommon( void ):
			m_handle(nullptr)
		{
		}

		virtual ~gtDriverCommon( void ){
		}


			//	получить параметры
		virtual const gtDriverInfo&	getParams( void ){
			return m_params;
		}

		virtual void *	getPluginHandle( void ){
			return m_handle;
		}

		/*временно*/
		friend void setHandle( gtDriver* instance, void * h );
	};

	/*временно*//*возможно уберу*/
	GT_FORCE_INLINE void setHandle( gtDriver* instance, void * h ){
		((gtDriverCommon*)instance)->m_handle = h;
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