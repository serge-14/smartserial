#ifndef _SMART_SERIAL_LIB_
#define _SMART_SERIAL_LIB_

#include "SmartSerialHelper.h"

template<int TotalCommands, int CommandsPerNode, int BufferSize, int ParametersCount>
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
        node* m_arrChildren[ CommandsPerNode ];
        size_t uChildrenCount;
        char keyword[ BufferSize + 1 ];
        CallbackWrapperInterface* m_pCommand;

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
    node m_nodes[ TotalCommands ];
    node m_nodeRoot;
};

template<int TotalCommands, int CommandsPerNode, int BufferSize, int ParametersCount>
inline bool GenericSmartSerial<TotalCommands, CommandsPerNode, BufferSize, ParametersCount>::registerCommand( const char* arrKeywords, CallbackWrapperInterface* pCallback )
{
    size_t uStart = 0;

    node* pCurrentNode = &m_nodeRoot;

    const char* strCommand = arrKeywords;
    const char* strNextCommand = arrKeywords;

    while( SmartSerialHelper::iterate( strCommand, strNextCommand, BufferSize ) )
    {
        if( !find( strCommand, strNextCommand - strCommand, pCurrentNode, pCurrentNode ) )
        {
            if( pCurrentNode->uChildrenCount >= CommandsPerNode )
            {
                return false;
            }

            if( m_uUsedCommands >= TotalCommands )
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

    if( pCurrentNode == &m_nodeRoot )
    {
        return false;
    }

    pCurrentNode->m_pCommand = pCallback;

    return true;
}

template<int TotalCommands, int CommandsPerNode, int BufferSize, int ParametersCount>
inline void GenericSmartSerial<TotalCommands, CommandsPerNode, BufferSize, ParametersCount>::poll()
{
    if( SmartSerialHelper::next( m_pStream, m_strCommand, BufferSize ) )
    {
        node* pCurrentNode = &m_nodeRoot;

        const char* strCommand = m_strCommand;
        const char* strNextCommand = m_strCommand;

        while( SmartSerialHelper::iterate( strCommand, strNextCommand, BufferSize ) )
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
        size_t uIndex = 0;

        strNextCommand = strCommand;

        while( SmartSerialHelper::iterate( strCommand, strNextCommand, BufferSize ) )
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

template<int TotalCommands, int CommandsPerNode, int BufferSize, int ParametersCount>
inline bool GenericSmartSerial<TotalCommands, CommandsPerNode, BufferSize, ParametersCount>::find( const char* strKeyword, size_t uLength, node* pNode, node*& pFoundNode )
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

typedef GenericSmartSerial<16, 4, 32, 4> SmartSerial;


#endif //_SMART_SERIAL_LIB_