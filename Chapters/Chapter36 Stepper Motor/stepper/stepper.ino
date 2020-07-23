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
  Stepper Motor Control - one revolution

  This program drives a unipolar or bipolar stepper motor.
  The motor is attached to digital pins 8 - 11 of the Arduino.

  The motor should revolve one revolution in one direction, then
  one revolution in the other direction.

*/

//#define CLOUD
#undef  CLOUD

#include <board.h>

#ifdef CLOUD
#include <ArduinoJson.h>

#define DSerial SerialUSB
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

#else
//#define DSerial Serial
#define DSerial SerialUSB

#endif

#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

// IN1 - PA6, IN2 - PA7, IN3 - SS, IN4 - MOSI
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, PA6, SS, PA7, MOSI);

void setup() 
{
  DSerial.begin(9600);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");
    
  myStepper.setSpeed(rolePerMinute);

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
        if (docOutput["Device"] == "StepperMotor")
        {
          DSerial.println("Device is a Stepper motor!");

          DSerial.println(docOutput["DeviceID"].as<String>());
          DSerial.println(docOutput["Timestamp"].as<double>(), 6);
          DSerial.println(docOutput["Device"].as<String>());
          DSerial.println(docOutput["OpCode"].as<String>());
          DSerial.println(docOutput["Step"].as<int>());
          DSerial.println(docOutput["Unit"].as<String>());

          if (docOutput["OpCode"] == "Write")
          {
            myStepper.step(docOutput["Step"].as<int>());            
            DSerial.print("\r\nNumber of steps written to stepper motor:");
            DSerial.println(docOutput["Step"].as<int>());
          }
        }
        else
        {
          DSerial.println("Device is not a Stepper motor!");
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

#else

  // step one revolution  in one direction:
  DSerial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  DSerial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);

#endif  
}
