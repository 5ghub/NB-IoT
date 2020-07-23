/*

  Copyright 2019, 5G HUB

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

/*
  LED1 indicates sound input, and the sensitivity is adjusted by potentiometer.
  There is a tiny screw on the blue potentiometer block that you can use for adjustment. Turning that
  clockwise lowers the potentiometer value, while counter-clockwise raises the potentiometer value.
  Use the potentiometer to adjust the Sound Sensor sensitivity. Turn the potentiometer
  several rotations until you see the LED1 extinguish (or just faintly blink). This might be slightly greater than
  500, if you are also watching Serial Monitor (inital adjustment), or, Serial Plotter (the latter is prefererd for observation).
  Special thanks to user CRomer, for his input and hard work!
*/

#include <board.h>

//#define DSerial Serial
#define DSerial SerialUSB

#define CLOUD
//#undef  CLOUD

#ifdef CLOUD
#include "5G-NB-IoT_MQTT.h"
#include "AWSIOT.hpp"
#include <ArduinoJson.h>

#define ATSerial Serial1

StaticJsonDocument<200> docInput;
StaticJsonDocument<200> docOutput;

char APN[] = "m2mNB16.com.attz";
char LOGIN[] = "";
char PASSWORD[] = "";

char mqtt_server[] = "a3kjt69iibf2h0-ats.iot.us-west-2.amazonaws.com";
unsigned int mqtt_port = 8883;
char mqtt_clientId[] = "basicPubSub";
char mqtt_topicName[] = "MyTopic";
unsigned int mqtt_index = 3;
Mqtt_Qos_t mqtt_qos = AT_MOST_ONCE;
unsigned long pub_time;

//IMEI of the modem
char IMEI[20];

_5G_NB_IoT_MQTT AWSIoT(ATSerial, DSerial);

#endif


int  sensorAnalogPin = A0;    // Select the Arduino input pin to accept the Sound Sensor's analog output
int  sensorDigitalPin = PA6;    // Select the Arduino input pin to accept the Sound Sensor's digital output
int  analogValue = 0;         // Define variable to store the analog value coming from the Sound Sensor
int  digitalValue;            // Define variable to store the digital value coming from the Sound Sensor

void setup()
{
  DSerial.begin(9600);               // The IDE settings for Serial Monitor/Plotter (preferred) must match this speed
  pinMode(sensorDigitalPin, INPUT); // Define pin 7 as an input port, to accept digital input
  pinMode(LED1, OUTPUT);           // Define LED13 as an output port, to indicate digital trigger reached

#ifdef CLOUD
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  InitModemMQTT(AWSIoT, DSerial, APN, LOGIN, PASSWORD,
                mqtt_server, mqtt_port,
                mqtt_clientId, mqtt_topicName,
                AT_MOST_ONCE, mqtt_index,
                1, 2, IMEI);

#endif   
}

void loop() 
{
  analogValue = analogRead(sensorAnalogPin); // Read the value of the analog interface A0 assigned to digitalValue
  digitalValue = digitalRead(sensorDigitalPin); // Read the value of the digital interface 7 assigned to digitalValue
  DSerial.println(analogValue); // Send the analog value to the serial transmit interface

  if (digitalValue == HIGH)   // When the Sound Sensor sends signla, via voltage present, light LED13 (L)
  {
    digitalWrite(LED1, HIGH);
  }
  else
  {
    digitalWrite(LED1, LOW);
  }

  delay(50);                  // Slight pause so that we don't overwhelm the serial interface

#ifdef CLOUD  
 
  char payload[256];
  char *sta_buf;
  int res;
  DeserializationError error;

  Mqtt_URC_Event_t ret = AWSIoT.WaitCheckMQTTURCEvent(payload, 2);
  switch (ret)
  {
    case MQTT_RECV_DATA_EVENT:
      error = deserializeJson(docOutput, payload);

      if (error == DeserializationError::Ok)
      {
        if (docOutput["Device"] == "Sound Sensor")
        {
          DSerial.println("Device is a Sound sensor!");

          DSerial.println(docOutput["DeviceID"].as<String>());
          DSerial.println(docOutput["Timestamp"].as<double>(), 6);
          DSerial.println(docOutput["Device"].as<String>());
          DSerial.println(docOutput["OpCode"].as<String>());
          DSerial.println(docOutput["Analog"].as<int>());
          DSerial.println(docOutput["Unit"].as<String>());
        }
        else
        {
          DSerial.println("Device is not a Sound sensor!");
        }
      }
      else
      {
        DSerial.println("\r\n Error in  Deserialization!");
        DSerial.println(error.c_str());
      }      

      break;

    case MQTT_STATUS_EVENT:
      sta_buf = strchr(payload, ',');
      if (atoi(sta_buf + 1) == 1)
      {
        if (AWSIoT.CloseMQTTClient(mqtt_index))
        {
          DSerial.println("\r\nClose the MQTT Client Success!");
        }
      }
      else
      {
        DSerial.print("\r\nStatus cade is :");
        DSerial.println(atoi(sta_buf + 1));
        DSerial.println("Please check the documentation for error details.");
      }
      break;
  }

  if (millis() - pub_time >= 5000UL) {
    pub_time = millis();

    // ...... Previous code lines go here

    docInput["DeviceID"] = IMEI;
    docInput["Timestamp"] = millis();
    docInput["Device"] = "Sound Sensor";
    docInput["OpCode"] = "Read";
    docInput["Analog"] = analogValue;
    docInput["Unit"] = "V";
    serializeJsonPretty(docInput, payload);

    // ..... Next code lines go here

    res = AWSIoT.MQTTPublishMessages(mqtt_index, 1, AT_LEAST_ONCE, mqtt_topicName, false, payload);

    if (res == PACKET_SEND_SUCCESS_AND_RECV_ACK ||
        res == PACKET_RETRANSMISSION)
    {
      DSerial.println("Publish Succeded!");
    }
    else
    {
      DSerial.println("Publish failed!");
    }
  }
#endif   
}
