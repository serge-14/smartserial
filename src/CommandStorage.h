#ifndef _COMMAND_STORAGE_SMART_SERIAL_LIB_
#define _COMMAND_STORAGE_SMART_SERIAL_LIB_

#include "CallbackWrapper.h"

#if !defined(SMART_SERIAL_DO_NOT_DEFINE_NEW)
void * operator new ( size_t size, void * ptr )
{
    return ptr;
}
#endif

template<int Size>
class CommandStorage
{
public:
    CommandStorage() :
        m_uNextBlock( 0 )
    {

    }

    CallbackWrapperInterface* wrap( void( *pCommandCallback )( ) )
    {
        return new( advance() ) CallbackWrapper<0>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1 ) )
    {
        return new( advance() ) CallbackWrapper<1, A1>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1, typename A2>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2 ) )
    {
        return new( advance() ) CallbackWrapper<2, A1, A2>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1, typename A2, typename A3>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2, A3 ) )
    {
        return new( advance() ) CallbackWrapper<3, A1, A2, A3>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1, typename A2, typename A3, typename A4>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2, A3, A4 ) )
    {
        return new( advance() ) CallbackWrapper<4, A1, A2, A3, A4>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2, A3, A4, A5 ) )
    {
        return new( advance() ) CallbackWrapper<5, A1, A2, A3, A4, A5>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2, A3, A4, A5, A6 ) )
    {
        return new( advance() ) CallbackWrapper<6, A1, A2, A3, A4, A5, A6>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
    }

private:
    void* advance()
    {
        if( m_uNextBlock >= sizeof( CallbackWrapper<0> ) * Size )
        {
            return nullptr;
        }

        void* pMemory = &m_pData[ m_uNextBlock ];
        m_uNextBlock += sizeof( CallbackWrapper<0> );

        return pMemory;
    }

    size_t m_uNextBlock;
    uint8_t m_pData[ sizeof( CallbackWrapper<0> ) * Size ];
};

#endif //_COMMAND_STORAGE_SMART_SERIAL_LIB_