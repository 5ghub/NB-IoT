/*

  Copyright 2020, 5G HUB

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
  associated documentation files (the "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
  following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <board.h>
#include "YACL.h"

#define DSerial SerialUSB

void setup()
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the Debug Serial!");
  delay(3000);

  DSerial.println("{");
  DSerial.println("\t \"DeviceID\": \"123456789012345\",");
  DSerial.println("\t \"Timestamp\": 50406,");
  DSerial.println("\t \"Device\": \"Temperature Sensor\",");
  DSerial.println("\t \"OpCode\": \"Read\",");
  DSerial.println("\t \"Temperature\": 30,");
  DSerial.println("\t \"Unit\": \"F\"");
  DSerial.println("}");

  // Data is a dictionnary of key/values, which corresponds to CBORPair objects
  // Thus, we create a CBORPair, and reserve a buffer of 100 bytes for it.
  // This is not mandatory, as CBORPair can reallocate a bigger buffer on the
  // fly, but preferable in terms of memory management.
  CBORPair data = CBORPair(100);
  data.append("DeviceID", "123456789012345");
  data.append("Timestamp", 50406);
  data.append("Device", "Temperature Sensor");
  data.append("OpCode", "Read");
  data.append("Temperature", 30);
  data.append("Unit", "F");

  // That's it! Let's see how our data looks.
  DSerial.print("CBOR Encoded data size: ");
  DSerial.println(data.length(),DEC);  
  
  DSerial.print("CBOR Encoded data: 0x");
  const uint8_t *cbor_encoded = data.to_CBOR();
  uint8_t cbor_data[256];
  for (size_t i = 0 ; i < data.length(); ++i) 
    {
     if (cbor_encoded[i] < 0x10) 
     {
      DSerial.print('0');
     }
     DSerial.print(cbor_encoded[i], HEX);
     //DSerial.print(' ');
     cbor_data[i] = cbor_encoded[i];
   }

  {
    DSerial.println("\nCBOR Decoded data:");
    
    // Import dictionary into a CBOR object
    CBOR data = CBOR(cbor_data, String((char*)cbor_data).length(), true);

    DSerial.println("{");

    if (!data["DeviceID"].is_string()) {
      DSerial.println("\"DeviceID\" is not a string. Aborting.");
      return;
    }
    DSerial.print("\t \"DeviceID\": ");
    char devid[32];
    data["DeviceID"].get_string(devid);
    DSerial.print(devid);
    DSerial.print(",\n");

    if (!data["Timestamp"].is_uint32()) {
      DSerial.println("\"Timestamp\" is not a int. Aborting.");
      return;
    }
    DSerial.print("\t \"Timestamp\": ");
    DSerial.print(uint32_t(data["Timestamp"]));
    DSerial.print(",\n");

    if (!data["Device"].is_string()) {
      DSerial.println("\"Device\" is not a string. Aborting.");
      return;
    }
    DSerial.print("\t \"Device\": ");
    char dev[32];
    data["Device"].get_string(dev);
    DSerial.print(dev);
    DSerial.print(",\n");

    if (!data["OpCode"].is_string()) {
      DSerial.println("\"OpCode\" is not a string. Aborting.");
      return;
    }
    DSerial.print("\t \"OpCode\": ");
    char opc[32];
    data["OpCode"].get_string(opc);
    DSerial.print(opc);
    DSerial.print(",\n");

    if (!data["Temperature"].is_uint32()) {
      DSerial.println("\"Temperature\" is not a int. Aborting.");
      return;
    }
    DSerial.print("\t \"Temperature\": ");
    DSerial.print(uint32_t(data["Temperature"]));
    DSerial.print(",\n");

    if (!data["Unit"].is_string()) {
      DSerial.println("\"Unit\" is not a string. Aborting.");
      return;
    }
    DSerial.print("\t \"Unit\": ");
    char un[32];
    data["Unit"].get_string(un);
    DSerial.print(un);
    DSerial.print("\n");

    DSerial.println("}");
  }

//  while (1) 
//  {
//  }
}

void loop()
{
  delay(1000);   // publish to topic every 5seconds
}
