#include "SmartSerialHelper.h"
#include "CallbackWrapperInterface.h"
#include "StreamInterface.h"

#include <string.h>

static const char COMMAND_END_SIGN = '?';
static const char COMMAND_SPLIT_SIGN = ' ';

const char* SmartSerialHelper::getNextParameter( const char* strBuffer )
{
    while( strBuffer[ 0 ] != '\0' && strBuffer[ 0 ] != COMMAND_SPLIT_SIGN )
    {
        strBuffer++;
    }

    return strBuffer;
}

bool SmartSerialHelper::iterate( const char*& strCommand, const char*& strNextCommand, size_t uMaxKeywordLength )
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
    if( uKeywordLength > uMaxKeywordLength )
    {
        return false;
    }

    return true;
}

void SmartSerialHelper::copy( char* strBuffer, const char* strSource, size_t uLength )
{
    strncpy( strBuffer, strSource, uLength );
    strBuffer[ uLength ] = 0;
}

bool SmartSerialHelper::next( StreamInterface* pStream, char* strBuffer, size_t uLength )
{
    if( pStream->available() )
    {
        size_t uBytesRead = pStream->readBytesUntil( COMMAND_END_SIGN, strBuffer, uLength );
        strBuffer[ uBytesRead ] = 0;

        return true;
    }

    return false;
}

void SmartSerialHelper::invoke( CallbackWrapperInterface* pCommand, const char** arrParams, size_t uCount )
{
    pCommand->invoke( arrParams, uCount );
}

bool SmartSerialHelper::compare( const char* str1, const char* str2, size_t uLength )
{
    return strncmp( str1, str2, uLength ) == 0;
}