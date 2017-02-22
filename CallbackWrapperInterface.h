#ifndef _CALLBACK_WRAPPER_INTERFACE_SMART_SERIAL_LIB_
#define _CALLBACK_WRAPPER_INTERFACE_SMART_SERIAL_LIB_

#define MAX_PARAMETERS

struct CallbackWrapperInterface
{
    virtual void invoke( const char** argv, size_t argc ) = 0;
};

#endif //_CALLBACK_WRAPPER_INTERFACE_SMART_SERIAL_LIB_