//	GOST

#pragma once
#ifndef __GT_PLUGIN_SYSTEM_IMPL_H__
#define __GT_PLUGIN_SYSTEM_IMPL_H__

namespace gost{

	/*	���������� � �������. �������� ���� � �����
		��������� �� ������� �������,
		�����,
		��� �� ����� ���������� gtPluginInfo*/
	struct gtPluginInfoDL{

		gtPluginInfoDL( void ):
			m_loadPlugin( nullptr ),
			m_handle( nullptr )
		{}

		~gtPluginInfoDL( void ){}

		//	���� � �����
		gtString m_path;

		//	��������� �� ������� ������
		void (*m_loadPlugin)();
		
		//	handle �������
		GT_LIBRARY_HANDLE m_handle;

		//	����� ����������
		gtPluginInfo m_info;
	};

	/*
		��� ������ � render �������.
		������ ����������.
		��������� ����� ����� ������� � ��������� ������ 
		��������� ������ load unload
	*/
	class gtPluginRender : public gtRefObject{

			//	�������� �� ������
		bool m_isLoad;

			//	����������
		gtPluginInfoDL	m_info;

	public:

		gtPluginRender( gtPluginInfoDL* info ):
			m_isLoad( false )
		{
			m_info = *info;
		}

		~gtPluginRender( void ){
			unload();
		}

			//	����� �������
		gtDriver* m_driver;

			//	��������� ������
		void load( const gtDriverInfo& params );

			//	���������������
		void unload( void );

	};

	class gtPluginSystemImpl GT_FINAL : public gtPluginSystem{

		void scanFolder( const gtString& );

		u32 m_numOfPlugins;

		//	��������� ��� ��������
		gtArray<gtPluginInfoDL> m_renderPluginCache;
		
		//	����������� �������
		gtList<gtPtr<gtPluginRender>>	m_renderPlugins;

	public:
		gtPluginSystemImpl( void );
		virtual ~gtPluginSystemImpl( void );

		bool init( void );

			//	�������� ���������� �������� � ����� plugins
		u32	getNumOfPlugins( void );

			//	��������� ����� ������
		gtDriver*	loadRenderPlugin( const gtDriverInfo& params );

			//	��������� � ������� �� ���������
		void 		unloadRenderPlugin( gtDriver* );

	};

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