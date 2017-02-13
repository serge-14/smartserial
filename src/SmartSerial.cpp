#include "SmartSerial.h"
#include "CallbackWrapperInterface.h"
#include "StreamInterface.h"

#include <string.h>

static const char COMMAND_END_SIGN = '?';
static const char COMMAND_SPLIT_SIGN = ' ';

SmartSerial::node::node() :
    m_pCommand( nullptr ),
    uChildrenCount( 0 )
{
    memset( m_arrChildren, 0, sizeof( m_arrChildren ) );
    memset( keyword, 0, sizeof( keyword ) );
}

SmartSerial::node::node( CallbackWrapperInterface* pCommand ) :
    m_pCommand( pCommand ),
    uChildrenCount( 0 )
{
    memset( m_arrChildren, 0, sizeof( m_arrChildren ) );
    memset( keyword, 0, sizeof( keyword ) );
}

SmartSerial::SmartSerial( StreamInterface* pStream ) :
    m_pStream( pStream ),
    m_uUsedCommands( 0 )
{

}

const char* SmartSerial::getNextParameter( const char* strBuffer )
{
    while( strBuffer[ 0 ] != '\0' && strBuffer[ 0 ] != COMMAND_SPLIT_SIGN )
    {
        strBuffer++;
    }

    return strBuffer;
}

bool SmartSerial::iterate( const char*& strCommand, const char*& strNextCommand )
{
    while( *strNextCommand == COMMAND_SPLIT_SIGN )
    {
        strNextCommand++;
    }

    strCommand = strNextCommand;

    strNextCommand = getNextParameter( strCommand );

    if( strCommand == strNextCommand )
    {
        return false;
    }

    size_t uKeywordLength = strNextCommand - strCommand;
    if( uKeywordLength > MAX_COMMAND_SIZE )
    {
        return false;
    }

    return true;
}

bool SmartSerial::find( const char* strKeyword, size_t uLength, node* pNode, node*& pFoundNode )
{
    for( size_t j = 0; j < pNode->uChildrenCount; j++ )
    {
        if( pNode->m_arrChildren[ j ] != nullptr &&
            strncmp( strKeyword, pNode->m_arrChildren[ j ]->keyword, uLength ) == 0 )
        {
            pFoundNode = pNode->m_arrChildren[ j ];
            return true;
        }
    }

    return false;
}

bool SmartSerial::registerCommand( const char* arrKeywords, CallbackWrapperInterface* pCallback )
{
    size_t uStart = 0;

    node* pCurrentNode = &m_nodeRoot;

    const char* strCommand = arrKeywords;
    const char* strNextCommand = arrKeywords;

    while( iterate( strCommand, strNextCommand ) )
    {
        if( !find( strCommand, strNextCommand - strCommand, pCurrentNode, pCurrentNode ) )
        {
            if( pCurrentNode->uChildrenCount >= MAX_COMMANDS_PER_NODE )
            {
                return false;
            }

            strncpy( m_nodes[ m_uUsedCommands ].keyword, strCommand, strNextCommand - strCommand );

            pCurrentNode->m_arrChildren[ pCurrentNode->uChildrenCount ] = &m_nodes[ m_uUsedCommands ];

            m_uUsedCommands++;
            pCurrentNode->uChildrenCount++;

            pCurrentNode = pCurrentNode->m_arrChildren[ pCurrentNode->uChildrenCount - 1 ];
        }
    }

    if( pCurrentNode == &m_nodeRoot )
    {
        return false;
    }

    pCurrentNode->m_pCommand = pCallback;

    return true;
}

void SmartSerial::poll()
{
    if( m_pStream->available() )
    {
        size_t uBytesRead = m_pStream->readBytesUntil( COMMAND_END_SIGN, m_strCommand, MAX_COMMAND_SIZE );
        m_strCommand[ uBytesRead ] = 0;

        size_t uStart = 0;

        node* pCurrentNode = &m_nodeRoot;

        const char* strCommand = m_strCommand;
        const char* strNextCommand = m_strCommand;

        while( iterate( strCommand, strNextCommand ) )
        {
            if( !find( strCommand, strNextCommand - strCommand, pCurrentNode, pCurrentNode ) )
            {
                break;
            }
        }

        if( pCurrentNode->m_pCommand == nullptr )
        {
            return;
        }

        char arrParameters[ MAX_PARAMETERS_PER_COMMAND ][ MAX_COMMAND_SIZE ];
        size_t uCommandIndex = 0;
        size_t uIndex = 0;

        strNextCommand = strCommand;

        while( iterate( strCommand, strNextCommand ) )
        {
            strncpy( arrParameters[ uCommandIndex ], strCommand, strNextCommand - strCommand );
            arrParameters[ uCommandIndex ][ strNextCommand - strCommand ] = 0;
            uCommandIndex++;
        }

        const char* argv[ MAX_PARAMETERS_PER_COMMAND ];

        for( int i = 0; i < MAX_PARAMETERS_PER_COMMAND; i++ )
        {
            argv[ i ] = arrParameters[ i ];
        }

        pCurrentNode->m_pCommand->invoke( argv, uCommandIndex );
    }
}