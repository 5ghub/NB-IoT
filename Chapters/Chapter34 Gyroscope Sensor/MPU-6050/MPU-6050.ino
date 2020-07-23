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

// MPU-6050 Short Example Sketch

//#define CLOUD
//#undef  CLOUD

#ifdef CLOUD
#include <board.h>
#include <ArduinoJson.h>

#define DSerial SerialUSB
#define ATSerial Serial1

StaticJsonDocument<300> docInput;
StaticJsonDocument<300> docOutput;

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

#include<Wire.h>

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

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
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  DSerial.print("AcX = "); DSerial.print(AcX);
  DSerial.print(" | AcY = "); DSerial.print(AcY);
  DSerial.print(" | AcZ = "); DSerial.print(AcZ);
  DSerial.print(" | Tmp = "); DSerial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  DSerial.print(" | GyX = "); DSerial.print(GyX);
  DSerial.print(" | GyY = "); DSerial.print(GyY);
  DSerial.print(" | GyZ = "); DSerial.println(GyZ);
  delay(333);
    
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
        if (docOutput["Device"] == "Gyroscope")
        {
          DSerial.println("Device is a Gyroscope sensor!");

          DSerial.println(docOutput["DeviceID"].as<String>());
          DSerial.println(docOutput["Timestamp"].as<double>(), 6);
          DSerial.println(docOutput["Device"].as<String>());
          DSerial.println(docOutput["OpCode"].as<String>());
          DSerial.println(docOutput["AcX"].as<int>());
          DSerial.println(docOutput["AcY"].as<int>());
          DSerial.println(docOutput["AcZ"].as<int>());
          DSerial.println(docOutput["Tmp"].as<int>());
          DSerial.println(docOutput["AcX"].as<int>());
          DSerial.println(docOutput["AcY"].as<int>());
          DSerial.println(docOutput["AcZ"].as<int>());                                        
          DSerial.println(docOutput["TmpUnit"].as<String>());
        }
        else
        {
          DSerial.println("Device is not a Gyroscope sensor!");
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
    docInput["Device"] = "Gyroscope";
    docInput["OpCode"] = "Read";
    docInput["AcX"] = AcX;
    docInput["AcY"] = AcY;
    docInput["AcZ"] = AcZ;
    docInput["Tmp"] = Tmp;    
    docInput["GyX"] = GyX;
    docInput["GyY"] = GyY;
    docInput["GyZ"] = GyZ; 
    docInput["TmpUnit"] = "°F";
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

  delay(5000);
#endif  
}
