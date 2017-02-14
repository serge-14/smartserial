#ifndef _CONVERTER_SMART_SERIAL_LIB_
#define _CONVERTER_SMART_SERIAL_LIB_

template<typename To>
To convert_string( const char* str )
{
    return To();
}

template<>
int convert_string( const char* str );

template<>
double convert_string( const char* str );

template<>
float convert_string( const char* str );

template<>
const char* convert_string( const char* str );

template<>
bool convert_string( const char* str );

#endif //_CONVERTER_SMART_SERIAL_LIB_