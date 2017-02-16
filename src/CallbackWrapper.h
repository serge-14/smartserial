#ifndef _CALLBACK_WRAPPER_SMART_SERIAL_LIB_
#define _CALLBACK_WRAPPER_SMART_SERIAL_LIB_

#include <stdlib.h>

#include "CallbackWrapperInterface.h"
#include "Converter.h"

class empty_parameter { };

typedef void( *SimpleFunction )( );

template<size_t Count, typename A1 = empty_parameter, typename A2 = empty_parameter, typename A3 = empty_parameter, typename A4 = empty_parameter, typename A5 = empty_parameter, typename A6 = empty_parameter>
struct CallbackWrapper : public CallbackWrapperInterface
{
    SimpleFunction m_pCallback;

    CallbackWrapper( SimpleFunction pCallback ) :
        m_pCallback( pCallback )
    {

    }

    virtual void invoke( const char** argv, size_t argc )
    {
        if( argc == Count && Count == 0 )
        {
            reinterpret_cast<void( *)(void)>( m_pCallback )();
        }
        else if( argc == Count && Count == 1 )
        {
            reinterpret_cast<void( *)( A1 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ) );
        }
        else if( argc == Count && Count == 2 )
        {
            reinterpret_cast<void( *)( A1, A2 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ), convert_string<A2>( argv[ 1 ] ) );
        }
        else if( argc == Count && Count == 3 )
        {
            reinterpret_cast<void( *)( A1, A2, A3 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ), convert_string<A2>( argv[ 1 ] ), convert_string<A3>( argv[ 2 ] ) );
        }
        else if( argc == Count && Count == 4 )
        {
            reinterpret_cast<void( *)( A1, A2, A3, A4 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ), convert_string<A2>( argv[ 1 ] ), convert_string<A3>( argv[ 2 ] ), convert_string<A4>( argv[ 3 ] ) );
        }
        else if( argc == Count && Count == 5 )
        {
            reinterpret_cast<void( *)( A1, A2, A3, A4, A5 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ), convert_string<A2>( argv[ 1 ] ), convert_string<A3>( argv[ 2 ] ), convert_string<A4>( argv[ 3 ] ), convert_string<A5>( argv[ 4 ] ) );
        }
        else if( argc == Count && Count == 6 )
        {
            reinterpret_cast<void( *)( A1, A2, A3, A4, A5, A6 )>( m_pCallback )( convert_string<A1>( argv[ 0 ] ), convert_string<A2>( argv[ 1 ] ), convert_string<A3>( argv[ 2 ] ), convert_string<A4>( argv[ 3 ] ), convert_string<A5>( argv[ 4 ] ), convert_string<A6>( argv[ 5 ] ) );
        }
    }
};

inline CallbackWrapperInterface* wrap( void( *pCommandCallback )() )
{
    return new CallbackWrapper<0>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
}

template<typename A1>
inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1 arg1 ) )
{
    return new CallbackWrapper<1, A1>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
}

template<typename A1, typename A2>
inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1 arg1, A2 arg2 ) )
{
    return new CallbackWrapper<2, A1, A2>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
}

template<typename A1, typename A2, typename A3>
inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1 arg1, A2 arg2, A3 arg3 ) )
{
    return new CallbackWrapper<3, A1, A2, A3>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
}

template<typename A1, typename A2, typename A3, typename A4>
inline CallbackWrapperInterface* wrap( void( *pCommandCallback )( A1, A2, A3, A4 ) )
{
    return new CallbackWrapper<4, A1, A2, A3, A4>( reinterpret_cast<SimpleFunction>( pCommandCallback ) );
}

#endif //_CALLBACK_WRAPPER_SMART_SERIAL_LIB_
