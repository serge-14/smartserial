#include "Converter.h"

#include <stdlib.h>

template<>
int convert_string<int>( const char* str )
{
    return atoi( str );
}

template<>
double convert_string( const char* str )
{
    return atof( str );
}

template<>
float convert_string( const char* str )
{
    return static_cast<float>( atof( str ) );
}

template<>
const char* convert_string( const char* str )
{
    return str;
}
