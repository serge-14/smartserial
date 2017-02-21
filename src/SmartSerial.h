#ifndef _SMART_SERIAL_LIB_
#define _SMART_SERIAL_LIB_

#include <stdlib.h>

#include "SmartSerialHelper.h"

template<int MaxNodeCount, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
class GenericSmartSerial
{
public:
    GenericSmartSerial( StreamInterface* pStream ) :
        m_pStream( pStream ),
        m_uUsedCommands( 0 )
    {

    }

    bool registerCommand( const char* arrKeywords, CallbackWrapperInterface* pCallback );

    void poll();
private:
    struct node
    {
        CallbackWrapperInterface* m_pCommand;
        size_t uChildrenCount;
        node* m_arrChildren[ CommandsPerNode ];
        char keyword[ KeywordSize + 1 ];

        node() :
            m_pCommand( nullptr ),
            uChildrenCount( 0 )
        {
        }

        node( CallbackWrapperInterface* pCommand ) :
            m_pCommand( pCommand ),
            uChildrenCount( 0 )
        {
        }
    };

    bool find( const char* strKeyword, size_t uLength, node* pNode, node*& pFoundNode );

    char m_strCommand[ BufferSize + 1 ];

    StreamInterface* m_pStream;
    size_t m_uUsedCommands;
    node m_nodes[ MaxNodeCount ];
    node m_nodeRoot;
};

template<int MaxNodeCount, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
inline bool GenericSmartSerial<MaxNodeCount, KeywordSize, CommandsPerNode, BufferSize, ParametersCount>::registerCommand( const char* arrKeywords, CallbackWrapperInterface* pCallback )
{
    if( pCallback == nullptr )
    {
        return false;
    }

    node* pCurrentNode = &m_nodeRoot;

    const char* strCommand = arrKeywords;
    const char* strNextCommand = arrKeywords;

    while( SmartSerialHelper::iterate( strCommand, strNextCommand, KeywordSize ) )
    {
        if( !find( strCommand, strNextCommand - strCommand, pCurrentNode, pCurrentNode ) )
        {
            if( pCurrentNode->uChildrenCount >= CommandsPerNode )
            {
                return false;
            }

            if( m_uUsedCommands >= MaxNodeCount )
            {
                return false;
            }

            SmartSerialHelper::copy( m_nodes[ m_uUsedCommands ].keyword, strCommand, strNextCommand - strCommand );

            pCurrentNode->m_arrChildren[ pCurrentNode->uChildrenCount ] = &m_nodes[ m_uUsedCommands ];

            m_uUsedCommands++;
            pCurrentNode->uChildrenCount++;

            pCurrentNode = pCurrentNode->m_arrChildren[ pCurrentNode->uChildrenCount - 1 ];
        }
    }

    if( strNextCommand - strCommand > KeywordSize )
    {
        return false;
    }

    if( pCurrentNode == &m_nodeRoot )
    {
        return false;
    }

    pCurrentNode->m_pCommand = pCallback;

    return true;
}

template<int MaxNodeCount, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
inline void GenericSmartSerial<MaxNodeCount, KeywordSize, CommandsPerNode, BufferSize, ParametersCount>::poll()
{
    if( SmartSerialHelper::next( m_pStream, m_strCommand, BufferSize ) )
    {
        node* pCurrentNode = &m_nodeRoot;

        const char* strCommand = m_strCommand;
        const char* strNextCommand = m_strCommand;

        while( SmartSerialHelper::iterate( strCommand, strNextCommand, KeywordSize ) )
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

        char arrParameters[ ParametersCount ][ BufferSize ];
        size_t uCommandIndex = 0;

        strNextCommand = strCommand;

        while( SmartSerialHelper::iterate( strCommand, strNextCommand, BufferSize ) && uCommandIndex < ParametersCount )
        {
            SmartSerialHelper::copy( arrParameters[ uCommandIndex ], strCommand, strNextCommand - strCommand );

            uCommandIndex++;
        }

        const char* argv[ ParametersCount ];

        for( int i = 0; i < ParametersCount; i++ )
        {
            argv[ i ] = arrParameters[ i ];
        }

        SmartSerialHelper::invoke(pCurrentNode->m_pCommand, argv, uCommandIndex );
    }
}

template<int MaxNodeCount, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
inline bool GenericSmartSerial<MaxNodeCount, KeywordSize, CommandsPerNode, BufferSize, ParametersCount>::find( const char* strKeyword, size_t uLength, node* pNode, node*& pFoundNode )
{
    for( size_t j = 0; j < pNode->uChildrenCount; j++ )
    {
        if( SmartSerialHelper::compare( strKeyword, pNode->m_arrChildren[ j ]->keyword, uLength ) )
        {
            pFoundNode = pNode->m_arrChildren[ j ];
            return true;
        }
    }

    return false;
}

typedef GenericSmartSerial<16, 8, 4, 32, 4> SmartSerial;


#endif //_SMART_SERIAL_LIB_
