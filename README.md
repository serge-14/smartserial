[![Build Status](https://travis-ci.org/serge-14/smartserial.svg?branch=master)](https://travis-ci.org/serge-14/smartserial)

# Smart Serial library for Arduino

Download latest version from [here](archive/master.zip)

# Using library

Import library
 - [Arduino IDE](https://www.arduino.cc/en/guide/libraries#toc2)
 - [Platformio](http://docs.platformio.org/en/stable/librarymanager/)

Include following libraries:

```cpp
#include <SmartSerial.h> // Actual Smart Serial implementation
#include <SerialStream.h> // Smart Serial stream wrapper for Arduino SDK Serial class
#include <CommandStorage.h> // Command storage
```

Define following variables:

```cpp
SerialStream stream; // Wrapper for Serial
CommandStorage<16> storage; // Storage for 16 commands

// Smart Serial with following parameters:
// 12 - Maximum node counts
// 8 -  Maximum keyword size,in this case we can have maximum 8 letters per keyword
// 2 -  Maximum count of child commands per node
// 16 - Buffer size
// 4 -  Maximum parameters count for the command
GenericSmartSerial<12, 8, 2, 16, 4> smartSerial(&stream);
```

Register command:

```cpp
void commandShowTime()
{
    DateTime now = g_Clock.getTime();
    now.print();
}

void setup()
{
    ...

    smartSerial.registerCommand("show time", storage.wrap(commandShowTime));

    ...
}
```

And polling new commands:

```cpp
void loop()
{
    smartSerial.poll();
}
```

## Nodes

There are two parameters `MaxNodeCount` and `CommandsPerNode` which may be unclear. Both defines size of the tree which is going to be build in `GenericSmartSerial`.

`MaxNodeCount` - maximum amount of tree nodes can be created to hold commands

`CommandsPerNode` - maximum amount of children a node can have

For example, by adding commands:

```
alarm enable at
alarm reset
```

following tree is going to be created:

```
alarm
 |- enable
 |  |- at (command holder)
 |- reset (command holder)
```

It will consume 4 nodes in total.