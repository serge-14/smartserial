#ifndef _SERIAL_STREAM_SMART_SERIAL_LIB_
#define _SERIAL_STREAM_SMART_SERIAL_LIB_

#include "StreamInterface.h"

struct SerialStream : StreamInterface
{
    virtual bool available();
    virtual size_t readBytesUntil(char cTermination, char* arrBuffer, size_t uSize);
};

#endif //_SERIAL_STREAM_SMART_SERIAL_LIB_
