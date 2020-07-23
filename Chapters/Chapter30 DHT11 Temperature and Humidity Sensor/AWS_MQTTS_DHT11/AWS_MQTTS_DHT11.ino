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

#include "5G-NB-IoT_MQTT.h"
#include "AWSIOT.hpp"
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

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

// Allocate the documents to serialize and deserialize
StaticJsonDocument<200> docInput;
StaticJsonDocument<200> docOutput;

//IMEI of the modem
char IMEI[64];

_5G_NB_IoT_MQTT AWSIoT(ATSerial, DSerial);

#define DHTPIN 8     // pin PA6, Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  // Init DHT11
  dht.begin();
  DSerial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  DSerial.println(F("------------------------------------"));
  DSerial.println(F("Temperature Sensor"));
  DSerial.print  (F("Sensor Type: ")); DSerial.println(sensor.name);
  DSerial.print  (F("Driver Ver:  ")); DSerial.println(sensor.version);
  DSerial.print  (F("Unique ID:   ")); DSerial.println(sensor.sensor_id);
  DSerial.print  (F("Max Value:   ")); DSerial.print(sensor.max_value); DSerial.println(F("°C"));
  DSerial.print  (F("Min Value:   ")); DSerial.print(sensor.min_value); DSerial.println(F("°C"));
  DSerial.print  (F("Resolution:  ")); DSerial.print(sensor.resolution); DSerial.println(F("°C"));
  DSerial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  DSerial.println(F("Humidity Sensor"));
  DSerial.print  (F("Sensor Type: ")); DSerial.println(sensor.name);
  DSerial.print  (F("Driver Ver:  ")); DSerial.println(sensor.version);
  DSerial.print  (F("Unique ID:   ")); DSerial.println(sensor.sensor_id);
  DSerial.print  (F("Max Value:   ")); DSerial.print(sensor.max_value); DSerial.println(F("%"));
  DSerial.print  (F("Min Value:   ")); DSerial.print(sensor.min_value); DSerial.println(F("%"));
  DSerial.print  (F("Resolution:  ")); DSerial.print(sensor.resolution); DSerial.println(F("%"));
  DSerial.println(F("------------------------------------"));

  // Init modem
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  InitModemMQTT(AWSIoT, DSerial, APN, LOGIN, PASSWORD,
                mqtt_server, mqtt_port,
                mqtt_clientId, mqtt_topicName,
                AT_MOST_ONCE, mqtt_index,
                1, 2, IMEI);
}

void loop() {
  char payload[500];
  char recv_payload[512] = "";
  char *sta_buf;
  int res;
  DeserializationError error;
  float tempF;
  float humid;

  Mqtt_URC_Event_t ret = AWSIoT.WaitCheckMQTTURCEvent(recv_payload, 2);
  switch (ret)
  {
    case MQTT_RECV_DATA_EVENT:
      error = deserializeJson(docOutput, recv_payload);

      if (error != DeserializationError::Ok)
      {
        DSerial.println("\r\n Error in  Deserialization!");
        DSerial.println(error.c_str());
      }
      else
      {
        if (docOutput["Device"] == "Temperature Sensor")
        {
          DSerial.println("Device is a Temperature sensor!");

          DSerial.println(docOutput["DeviceID"].as<String>());
          DSerial.println(docOutput["Timestamp"].as<double>(), 6);
          DSerial.println(docOutput["Device"].as<String>());
          DSerial.println(docOutput["OpCode"].as<String>());
          DSerial.println(docOutput["Temperature"].as<int>());
          DSerial.println(docOutput["Unit"].as<String>());
          DSerial.println(docOutput["Humidity"].as<int>());
        }
        else
        {
          DSerial.println("Device is not a Temperature sensor!");
        }
      }

      break;

    case MQTT_STATUS_EVENT:
      sta_buf = strchr(recv_payload, ',');
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

    default:
      //DSerial.println("\r\nUnknown event from Recv is received");
      //      DSerial.println(ret);
      break;
  }

  if (millis() - pub_time >= 5000UL) {
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      DSerial.println(F("Error reading temperature!"));
    }
    else {
      DSerial.print(F("Temperature: "));
      tempF = (event.temperature * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
      DSerial.print(tempF);
      DSerial.println(F("°F"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      DSerial.println(F("Error reading humidity!"));
    }
    else {
      DSerial.print(F("Humidity: "));
      DSerial.print(event.relative_humidity);
      DSerial.println(F("%"));
      humid = event.relative_humidity;
    }

    pub_time = millis();

    docInput["DeviceID"] = IMEI;
    docInput["Timestamp"] = millis();
    docInput["Device"] = "Temperature and Humidity Sensor";
    docInput["OpCode"] = "Read";
    docInput["Temperature"] = event.temperature;
    docInput["Unit"] = "°C";
    docInput["Humidity"] = event.relative_humidity;
    serializeJsonPretty(docInput, payload);

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
}
