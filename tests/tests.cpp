// tests.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"

GTEST_API_ int main( int argc, char** argv )
{
    testing::InitGoogleTest( &argc, argv );
    int exitCode = RUN_ALL_TESTS();

    return exitCode;
}
