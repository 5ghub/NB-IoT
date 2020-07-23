#include <ArduinoJson.h>

#define DSerial SerialUSB

// Allocate the JSON document
// Inside the brackets, 500 is the capacity of the memory pool in bytes.
// This document is used to serialize
StaticJsonDocument<500> docInput;

// This document is used to deserialize
StaticJsonDocument<500> docOutput;
  
void setup() 
{
  // Initialize serial port
  DSerial.begin(115200);
  while (!Serial) continue;
  
  DSerial.println("This is JSON Serializer example!");
}

void loop() 
{
  char output[500];
  static int counter=0;
  
  docInput["sensor"] = "GPS"; 
  docInput["lat"] = 48.748010; 
  docInput["lon"] = 120.12345;
  docInput["cnt"] = counter;

  counter++;
  
  serializeJsonPretty(docInput, output);
  DSerial.println("Json format:\r\n");
  DSerial.println(output);
  
  DeserializationError error = deserializeJson(docOutput, output);
  
  if (error != DeserializationError::Ok)
  {
    DSerial.println("\r\n Error in  Deserialization!");
    Serial.println(error.c_str());
    return;
  }
  
  DSerial.println(docOutput["sensor"].as<String>());

  if (docOutput["sensor"] == "GPS")
  {
    DSerial.println(docOutput["lat"].as<double>(), 6);
    DSerial.println(docOutput["lon"].as<double>(), 6);     
    DSerial.println(docOutput["cnt"].as<int>());    
  }

  delay(1000);
}
