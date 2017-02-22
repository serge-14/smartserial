#include "SerialStream.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

bool SerialStream::available()
{
#if defined(ARDUINO)
    return Serial.available() != 0;
#else
    return false;
#endif
}

size_t SerialStream::readBytesUntil(char cTermination, char* arrBuffer, size_t uSize)
{
#if defined(ARDUINO)
    return Serial.readBytesUntil(cTermination, arrBuffer, uSize);
#else
    return 0;
#endif
}
