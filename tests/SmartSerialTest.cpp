#include "gtest/gtest.h"

#include "CallbackWrapper.h"
#include "SmartSerial.h"
#include "StreamInterface.h"

namespace tests
{
    struct MockStream : StreamInterface
    {
        virtual bool available()
        {
            return m_bAvailable;
        }

        virtual size_t readBytesUntil( char cTermination, char* arrBuffer, size_t uSize )
        {
            size_t uBytes = std::min( m_strBuffer.size(), uSize );
            size_t uIndex = 0;

            while( uIndex < uBytes && m_strBuffer[ uIndex ] != cTermination )
            {
                arrBuffer[ uIndex++ ] = m_strBuffer[ uIndex ];
            }

            return uIndex;
        }

        bool m_bAvailable;
        std::string m_strBuffer;

    };

    class SmartSerialTest : public ::testing::Test
    {
    protected:

        virtual void SetUp()
        {
            g_bTestMethodCalled = false;
            g_uTestMethodSum = 0;
        }

        static void testMethod()
        {
            g_bTestMethodCalled = true;
        }

        static void testMethodWithArguments(int a1, int a2, int a3, int a4)
        {
            g_bTestMethodCalled = true;
            g_uTestMethodSum = a1 + a2 + a3 + a4;
        }

        static void testMethodWithTwoArguments( int a1, int a2 )
        {
            g_bTestMethodCalled = true;
            g_uTestMethodSum = a1 * a2;
        }

        static bool g_bTestMethodCalled;
        static int g_uTestMethodSum;
    };

    bool SmartSerialTest::g_bTestMethodCalled = false;
    int SmartSerialTest::g_uTestMethodSum = 0;


    TEST_F( SmartSerialTest, ExecuteCommand_EmptyBuffer )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = false;

        SmartSerial smartSerial( &mockStream );

        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_UnknownCommand )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "test command?";

        SmartSerial smartSerial( &mockStream );

        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_TestCommand )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "test command?";

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "test command", wrap( testMethod ) ) );

        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_SingleWordCommand )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "test", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "command?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );

        mockStream.m_strBuffer = "test?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_TestCommandWithArguments )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "test command 0 1 2 3?";

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "test command", wrap( testMethodWithArguments ) ) );

        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 6 );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_OneWord_TestCommandWithArguments )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "test 0 1 2 3?";

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "test", wrap( testMethodWithArguments ) ) );

        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 6 );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_ThreeWord_TestCommandWithArguments )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "a + b 0 1 2 3?";

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethodWithArguments ) ) );

        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 6 );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_TestCommandWithArguments_WrongFormat )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "test command", wrap( testMethodWithArguments ) ) );

        mockStream.m_strBuffer = "just command2 a1 a2 a3 a4?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );

        mockStream.m_strBuffer = "test command2 a1 a2 a3 a4?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );

        mockStream.m_strBuffer = "test command a1 a2 a3 a4?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 0 );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_CantRegister_VeryBigKeyword )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;
        mockStream.m_strBuffer = "test command a1 a2 a3 a4?";

        SmartSerial smartSerial( &mockStream );

        ASSERT_FALSE( smartSerial.registerCommand( "command_keyword_is_very_big_command_keyword_is_very_big", wrap( testMethodWithArguments ) ) );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_VeryBigCommand)
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "very_big test23", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "very_big test23?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, ExecuteCommand_TwoCommands )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a + c", wrap( testMethodWithArguments ) ) );

        mockStream.m_strBuffer = "a + b?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 0 );

        g_bTestMethodCalled = false;
        g_uTestMethodSum = 0;

        mockStream.m_strBuffer = "a + c 2 5 1 6?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 14 );
    }

    TEST_F( SmartSerialTest, DoNotRegisterMore4CommandsPerNode )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a a", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "b a", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "c a", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "d a", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "f a", wrap( testMethod ) ) );
    }

    TEST_F( SmartSerialTest, TooManyCommands )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "b", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "c", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "d", wrap( testMethod ) ) );

        ASSERT_TRUE( smartSerial.registerCommand( "a b", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "b b", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "c b", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "d b", wrap( testMethod ) ) );

        ASSERT_TRUE( smartSerial.registerCommand( "a c", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "b c", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "c c", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "d c", wrap( testMethod ) ) );

        ASSERT_TRUE( smartSerial.registerCommand( "a d", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "b d", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "c d", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "d d", wrap( testMethod ) ) );

        ASSERT_FALSE( smartSerial.registerCommand( "a b c", wrap( testMethod ) ) );
    }

    TEST_F( SmartSerialTest, OverrideCommand )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        SmartSerial smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethodWithArguments ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "a + b?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, OneLetterKeyword )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<4, 1, 4, 16, 4> smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethodWithArguments ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a bb b", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a b cc", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "a + b 25 10 2 35?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 25 + 10 + 2 + 35 );
    }

    TEST_F( SmartSerialTest, MaximumOneCommand )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<1, 3, 4, 16, 4> smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "aaa", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "bbb", wrap( testMethod ) ) );
    }

    TEST_F( SmartSerialTest, MaximumTwoParameters )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<6, 3, 4, 16, 2> smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a * b", wrap( testMethodWithTwoArguments ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a + b", wrap( testMethodWithArguments ) ) );

        mockStream.m_strBuffer = "a * b 25 2?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
        ASSERT_EQ( g_uTestMethodSum, 50 );

        g_bTestMethodCalled = false;

        mockStream.m_strBuffer = "a + b 25 10 2 35?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, Buffer4Bytes )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<6, 16, 4, 4, 2> smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "command 1", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "command 2", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "command 1?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, OneCommandPerNode )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<6, 16, 1, 4, 2> smartSerial( &mockStream );

        ASSERT_TRUE( smartSerial.registerCommand( "a 1", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 2", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a 1 3", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 1 2", wrap( testMethod ) ) );
    }

    TEST_F( SmartSerialTest, RegisterNullptr )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<6, 16, 1, 4, 2> smartSerial( &mockStream );

        ASSERT_FALSE( smartSerial.registerCommand( "a 1", nullptr ) );
    }

    TEST_F( SmartSerialTest, Minimalistic )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<1, 1, 1, 1, 1> smartSerial( &mockStream );

        ASSERT_FALSE( smartSerial.registerCommand( "aa", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 1", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 2", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "a?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );

        g_bTestMethodCalled = false;

        mockStream.m_strBuffer = "a 1?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );
    }

    TEST_F( SmartSerialTest, MinimalisticWithBigBuffer )
    {
        MockStream mockStream;
        mockStream.m_bAvailable = true;

        GenericSmartSerial<1, 1, 1, 16, 1> smartSerial( &mockStream );

        ASSERT_FALSE( smartSerial.registerCommand( "aa", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 1", wrap( testMethod ) ) );
        ASSERT_TRUE( smartSerial.registerCommand( "a", wrap( testMethod ) ) );
        ASSERT_FALSE( smartSerial.registerCommand( "a 2", wrap( testMethod ) ) );

        mockStream.m_strBuffer = "a?";
        smartSerial.poll();

        ASSERT_TRUE( g_bTestMethodCalled );

        g_bTestMethodCalled = false;

        mockStream.m_strBuffer = "a 1?";
        smartSerial.poll();

        ASSERT_FALSE( g_bTestMethodCalled );
    }
}