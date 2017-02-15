#ifndef _SMART_SERIAL_HELPER_LIB_
#define _SMART_SERIAL_HELPER_LIB_

#include <stdlib.h>

struct StreamInterface;
struct CallbackWrapperInterface;

template<int TotalCommands, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
class GenericSmartSerial;

class SmartSerialHelper
{
    template<int TotalCommands, int KeywordSize, int CommandsPerNode, int BufferSize, int ParametersCount>
    friend class GenericSmartSerial;
private:
    static const char* getNextParameter( const char* strBuffer );
    static bool iterate( const char*& strCommand, const char*& strNextCommand, size_t uMaxKeywordLength );
    static void copy( char* strBuffer, const char* strSource, size_t uLength );
    static bool compare( const char* str1, const char* str2, size_t uLength );
    static bool next( StreamInterface* pStream, char* strBuffer, size_t uLength );
    static void invoke( CallbackWrapperInterface* pCommand, const char** arrParams, size_t uCount );
};

#endif //_SMART_SERIAL_HELPER_LIB_
