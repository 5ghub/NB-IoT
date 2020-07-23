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

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

char APN[] = "m2mNB16.com.attz";
char LOGIN[] = "";
char PASSWORD[] = "";

const char DFOTA[] = "+QFOTADL";
const char http_url[] = "http://95.171.16.128/MPU6050.zip";
const char FOTA_HTTPSTART[] = "+QIND: \"FOTA\",\"HTTPSTART\"";
const char FOTA_HTTPEND[] = "+QIND: \"FOTA\",\"HTTPEND\",0";
const char FOTA_START[] = "+QIND: \"FOTA\",\"START\"";
const char FOTA_UPDATING[] = "+QIND: \"FOTA\",\"UPDATING\"";
const char FOTA_RESTORE[] = "+QIND: \"FOTA\",\"RESTORE\"";
const char FOTA_END[] = "+QIND: \"FOTA\",\"END\",0";

unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
unsigned int comm_socket_index = 1;  // The range is 0 ~ 11

_5G_NB_IoT_TCPIP _5GNBIoT(ATSerial, DSerial);

bool dfota()
{
  char cmd[16];
  int timeout = 200;

  strcpy(cmd, DFOTA);
  strcat(cmd, "=\"");
  strcat(cmd, http_url);
  strcat(cmd, "\"");
  if (!_5GNBIoT.sendAndSearch(cmd, RESPONSE_OK, 2)) {
    return false;
  }

  //+QIND: "FOTA","HTTPSTART"
  Cmd_Response_t ret = _5GNBIoT.readResponseAndSearch(FOTA_HTTPSTART, timeout);
  if (ret != SUCCESS_RESPONSE) {
    DSerial.println("\r\HTTPSTART FAIL!");
    return false;
  }
  DSerial.println("\r\HTTPSTART OK!");

  //+QIND: "FOTA","HTTPEND",0
  ret = _5GNBIoT.readResponseAndSearch(FOTA_HTTPEND, timeout);
  if (ret != SUCCESS_RESPONSE) {
    DSerial.println("\r\HTTPEND FAIL!");
    return false;
  }
  DSerial.println("\r\HTTPEND OK!");

  //+QIND: "FOTA","START"
  ret = _5GNBIoT.readResponseAndSearch(FOTA_START, timeout);
  if (ret != SUCCESS_RESPONSE) {
    DSerial.println("\r\FOTA_START FAIL!");
    return false;
  }
  DSerial.println("\r\FOTA_START OK!");

  //+QIND: "FOTA","UPDATING", 1%
  do {
    ret = _5GNBIoT.readResponseAndSearch(FOTA_UPDATING, timeout);
    DSerial.println("\r\FOTA_UPDATING OK!");
  } while (ret == SUCCESS_RESPONSE);

  //+QIND: "FOTA","RESTORE", 1%
  do {
    ret = _5GNBIoT.readResponseAndSearch(FOTA_RESTORE, timeout);
  } while (ret == SUCCESS_RESPONSE);

  //+QIND: "FOTA","END",0
  ret = _5GNBIoT.readResponseAndSearch(FOTA_END, timeout);
  if (ret != SUCCESS_RESPONSE) {
    DSerial.println("\r\FOTA_END FAIL!");
    return false;
  }
  DSerial.println("\r\FOTA_END OK!");
}

void setup() {
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetDevCommandEcho(false);

  char apn_error[64];
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, LOGIN, PASSWORD, apn_error)) {
    DSerial.println(apn_error);
    delay(1000);
  }
  DSerial.println(apn_error);

  if (dfota())
    DSerial.println("\nDFOTA successful!");
  else
    DSerial.println("\nDFOTA fail!");
}

void loop() {
  delay(5000);
}
