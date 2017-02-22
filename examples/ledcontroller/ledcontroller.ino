//
// LED Controller Sample
//
// Enables/Disables built-in LED by commands over serial port
//
// Upload application to Arduino, connect to it over serial port and send following commands:
// 
// led status true?
// led status false?
//

#include <SmartSerial.h> // Actual Smart Serial implementation
#include <SerialStream.h> // Smart Serial stream wrapper for Arduino SDK Serial class
#include <CommandStorage.h> // Command storage

// Wrapper for Serial
SerialStream stream;

// Storage for 1 commands
CommandStorage<1> storage;

// Smart Serial with following parameters:
GenericSmartSerial<4, 8, 2, 16, 4> smartSerial(&stream);

// Command to enable/disable LED
void commandLED(bool mode)
{
    digitalWrite(LED_BUILTIN, mode ? HIGH : LOW);

    // Just sending response for debuging purpose
    Serial.println("ok");
}

void setup()
{
  //Configuring serial port
  Serial.begin(9600);
  
  // Enabling built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Registering command
  smartSerial.registerCommand("led status", storage.wrap(commandLED));
}

void loop()
{
  // Polling for new commands, it will not block execution
  smartSerial.poll();

  // Waiting for some time
  delay(500);
}

