#include "gtest/gtest.h"

#define SMART_SERIAL_DO_NOT_DEFINE_NEW
#include "CommandStorage.h"

namespace tests
{
    static bool g_bCalled;

    void command()
    {
        g_bCalled = true;
    }

    TEST( CommandStorageTests, ReturnsProperWrapper )
    {
        CommandStorage<1> storage;

        CallbackWrapperInterface* wrapper = storage.wrap( command );

        ASSERT_TRUE( wrapper != nullptr );

        g_bCalled = false;
        wrapper->invoke(nullptr, 0);

        ASSERT_TRUE( g_bCalled );
    }

    TEST( CommandStorageTests, StorageForOneCommand )
    {
        CommandStorage<1> storage;

        ASSERT_TRUE( storage.wrap( command ) != nullptr );
        ASSERT_TRUE( storage.wrap( command ) == nullptr );
    }

    TEST( CommandStorageTests, TwoCommands )
    {
        CommandStorage<2> storage;

        ASSERT_TRUE( storage.wrap( command ) != nullptr );
        ASSERT_TRUE( storage.wrap( command ) != nullptr );
        ASSERT_TRUE( storage.wrap( command ) == nullptr );
    }

    TEST( CommandStorageTests, Nullptr )
    {
        CommandStorage<2> storage;

        ASSERT_TRUE( storage.wrap( nullptr ) == nullptr );
    }
}