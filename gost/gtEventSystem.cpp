//	GOST

#include"stdafx.h"

gtEventSystem::gtEventSystem( gtEventConsumer* ec, gtEventConsumer* uc ):
	m_engineConsumer( ec ),
	m_userConsumer( uc ),
	m_numOfEvents( 0u ),
	m_currentEvent( 0u )
{}

gtEventSystem::~gtEventSystem( void ){
}

void gtEventSystem::runEventLoop( void ){
	while( true ){

		if( m_events[ m_currentEvent ].type == gtEvent::ET_NONE )
			break;

		if( m_userConsumer )
			m_userConsumer->processEvent( m_events[ m_currentEvent ] );

		m_events[ m_currentEvent ].type = gtEvent::ET_NONE;


		m_currentEvent++;
		if( m_currentEvent == m_numOfEvents )
			m_currentEvent = 0u;
	}

	m_numOfEvents = 0u;
}

void gtEventSystem::addEvent( const gtEvent& ev, u8 prior ){
	if( m_numOfEvents < EventMax ){
		m_events[ m_numOfEvents ] = ev;
		m_numOfEvents++;
	}
}

//	=============================================================

gtEngineEventConsumer::gtEngineEventConsumer(){
}

gtEngineEventConsumer::~gtEngineEventConsumer(){
}

void gtEngineEventConsumer::processEvent( const gtEvent& ev ){

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