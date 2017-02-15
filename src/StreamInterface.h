#ifndef _STREAM_INTERFACE_SMART_SERIAL_LIB_
#define _STREAM_INTERFACE_SMART_SERIAL_LIB_

#include <stdlib.h>

struct StreamInterface
{
    virtual bool available() = 0;
    virtual size_t readBytesUntil(char cTermination, char* arrBuffer, size_t uSize) = 0;
};

#endif //_STREAM_INTERFACE_SMART_SERIAL_LIB_
