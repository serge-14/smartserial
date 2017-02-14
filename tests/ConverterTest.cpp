#include "gtest/gtest.h"

#include "Converter.h"

namespace tests
{
    TEST( ConverterTest, ToInteger )
    {
        ASSERT_EQ( convert_string<int>( "15" ), 15 );
        ASSERT_EQ( convert_string<int>( "-10" ), -10 );
        ASSERT_EQ( convert_string<int>( "0" ), 0 );
        ASSERT_EQ( convert_string<int>( "177text" ), 177 );

        ASSERT_EQ( convert_string<int>( "text" ), 0 );
        ASSERT_EQ( convert_string<int>( "0xFF" ), 0 );
        ASSERT_EQ( convert_string<int>( "A0" ), 0 );
    }

    TEST( ConverterTest, ToFloat )
    {
        ASSERT_FLOAT_EQ( convert_string<float>( "15.0" ), 15.0f );
        ASSERT_FLOAT_EQ( convert_string<float>( "-8.125" ), -8.125f );
        ASSERT_FLOAT_EQ( convert_string<float>( "0.0" ), 0.0f );
    }

    TEST( ConverterTest, ToDouble )
    {
        ASSERT_DOUBLE_EQ( convert_string<double>( "15.0" ), 15.0 );
        ASSERT_DOUBLE_EQ( convert_string<double>( "-8.125" ), -8.125 );
        ASSERT_DOUBLE_EQ( convert_string<double>( "0.0" ), 0.0 );
    }

    TEST( ConverterTest, ToString )
    {
        ASSERT_STRCASEEQ( convert_string<const char*>( "a string" ), "a string" );
    }

    TEST( ConverterTest, ToBool )
    {
        ASSERT_EQ( convert_string<bool>( "true" ), true );
        ASSERT_EQ( convert_string<bool>( "false" ), false );
        ASSERT_EQ( convert_string<bool>( "a string" ), false );
    }
}