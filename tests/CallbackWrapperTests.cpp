#include "gtest/gtest.h"

#include "CallbackWrapper.h"

namespace tests
{
    class CallbackTests : public ::testing::Test
    {
    protected:

        virtual void SetUp()
        {
            g_bTestMethodCalled = false;
            g_iTestMethodA1 = 0;
            g_iTestMethodA2 = 0;
            g_iTestMethodA3 = 0.0f;
            g_iTestMethodA4 = "";
        }

        static void testMethod1( int a1 )
        {
            g_bTestMethodCalled = true;
            g_iTestMethodA1 = a1;
        }

        static void testMethod2( int a1, double a2 )
        {
            g_bTestMethodCalled = true;
            g_iTestMethodA1 = a1;
            g_iTestMethodA2 = a2;
        }

        static void testMethod3( int a1, double a2, float a3 )
        {
            g_bTestMethodCalled = true;
            g_iTestMethodA1 = a1;
            g_iTestMethodA2 = a2;
            g_iTestMethodA3 = a3;
        }

        static void testMethod4( int a1, double a2, float a3, const char* a4 )
        {
            g_bTestMethodCalled = true;
            g_iTestMethodA1 = a1;
            g_iTestMethodA2 = a2;
            g_iTestMethodA3 = a3;
            g_iTestMethodA4 = a4;
        }

        static bool g_bTestMethodCalled;
        static int g_iTestMethodA1;
        static double g_iTestMethodA2;
        static float g_iTestMethodA3;
        static const char* g_iTestMethodA4;
    };

    bool CallbackTests::g_bTestMethodCalled = false;
    int CallbackTests::g_iTestMethodA1 = 0;
    double CallbackTests::g_iTestMethodA2 = 0;
    float CallbackTests::g_iTestMethodA3 = 0.0f;
    const char* CallbackTests::g_iTestMethodA4 = "";


    TEST_F( CallbackTests, OneArgument )
    {
        CallbackWrapperInterface* wrapper = wrap( testMethod1 );

        const char* params[] = { "25" };
        wrapper->invoke( static_cast<const char**>( params ), 1 );

        wrapper->invoke( params, 1 );

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_iTestMethodA1, 25 );
    }

    TEST_F( CallbackTests, TwoArgument )
    {
        CallbackWrapperInterface* wrapper = wrap( testMethod2 );

        const char* params[] = { "15", "10.5" };
        wrapper->invoke( static_cast<const char**>( params ), 2 );

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_iTestMethodA1, 15 );
        ASSERT_DOUBLE_EQ( g_iTestMethodA2, 10.5 );
    }

    TEST_F( CallbackTests, ThreeArgument )
    {
        CallbackWrapperInterface* wrapper = wrap( testMethod3 );

        const char* params[] = { "15", "10.5", "5.25" };
        wrapper->invoke( static_cast<const char**>( params ), 3 );

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_iTestMethodA1, 15 );
        ASSERT_DOUBLE_EQ( g_iTestMethodA2, 10.5 );
        ASSERT_FLOAT_EQ( g_iTestMethodA3, 5.25 );
    }

    TEST_F( CallbackTests, FourArgument )
    {
        CallbackWrapperInterface* wrapper = wrap( testMethod4 );

        const char* params[] = { "15", "10.5", "5.35", "hello" };
        wrapper->invoke( static_cast<const char**>( params ), 4 );

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_iTestMethodA1, 15 );
        ASSERT_DOUBLE_EQ( g_iTestMethodA2, 10.5 );
        ASSERT_FLOAT_EQ( g_iTestMethodA3, 5.35f );
        ASSERT_STRCASEEQ( g_iTestMethodA4, "hello" );
    }
}