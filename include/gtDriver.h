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

			//	нарисует картинку
			//	rect - координаты левого верхнего и правого нижнего углов
		virtual void draw2DImage( const v4i& rect, const gtMaterial& ) = 0;

			//	Render 2d image using region of texture
			//	нарисует картинку с выбранной областью текстуры
			//	rect - координаты левого верхнего и правого нижнего углов
			//	region - координаты левого верхнего и правого нижнего углов области картинки которую нужно нарисовать
		virtual void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& ) = 0;

		virtual void *	getPluginHandle( void ) = 0;

			//	компилировать либо получить ранее скомпилированный шейдер
		virtual gtShader *	getShader( 
				//	путь к файлу хранящем вершинный шейдер
			const gtString& vertexShader,
				//	главная функция вершинного шейдера, точка входа
			const gtStringA& vertexShaderMain,
				//	путь к файлу хранящем пиксельный/фрагментный шейдер
			const gtString& pixelShader,
				//	главная функция пиксельного/фрагментного шейдера, точка входа
			const gtStringA& pixelShaderMain,
				//	тип шейдерного языка
			gtShaderModel shaderModel,
				//	тип вершины (должен быть массив)
			gtVertexType * vertexType
			) = 0;


			//	Создаёт текстуру из gtImage. Обязательно нужно в ручную удалить вызвав release()
		virtual gtTexture*	createTexture( gtImage* ) = 0;
	};

		//	реализация драйвера в отдельных dll
		//	по этому создавать общий класс вижу только таким способом
	class gtDriverCommon : public gtDriver{

	protected:

			//	параметры запуска
		gtDriverInfo m_params;
		
			//	текущий размер окна
		v2i			m_currentWindowSize;

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

			//	получить текущий размер окна
		virtual const v2i&	getСurrentWindowSize( void ) const {
			return m_currentWindowSize;
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