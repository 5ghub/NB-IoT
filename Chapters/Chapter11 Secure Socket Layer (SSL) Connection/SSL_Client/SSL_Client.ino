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
#include <board.h>

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

char APN[] = "m2mNB16.com.attz";
char LOGIN[] = "";
char PASSWORD[] = "";

char ssl_ip[] = "a3kjt69iibf2h0-ats.iot.us-west-2.amazonaws.com";
char ssl_cmd[] = "GET / HTTP/1.1\r\n\r\n";
int ssl_port = 8883;
unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
unsigned int comm_socket_index = 1;  // The range is 0 ~ 11
unsigned int comm_ssl_index = 1;  // The range if 0 ~ 5

_5G_NB_IoT_SSL _5GNBIoT(ATSerial, DSerial);

void setup() 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetDevCommandEcho(false);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) 
  {
    DSerial.println(inf);
  }

  char apn_error[64];
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, "", "", apn_error)) 
  {
    DSerial.println(apn_error);
  }
  DSerial.println(apn_error);

  _5GNBIoT.DeleteFiles("*");
  char filelist[64];
  /*if (_5GNBIoT.GetFilesList(filelist))
    {
    DSerial.println(filelist);
    }*/

  char ssl_error[128];
  while (!_5GNBIoT.InitSSL(comm_ssl_index, aws_root_ca_pem, certificate_pem_crt, private_pem_key, ssl_error)) 
  {
    DSerial.println(ssl_error);
  }
  DSerial.println(ssl_error);

  while (!_5GNBIoT.OpenSSLSocket(comm_pdp_index, comm_ssl_index, comm_socket_index, ssl_ip, ssl_port, BUFFER_MODE)) {
    DSerial.println("\r\nConnect the SSL Sever Fail!");
    char ssl_socket_error[64];
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    } else if (_5GNBIoT.QueryLastErrorCode(ssl_socket_error)) {
      DSerial.println(ssl_socket_error);
    }
    delay(5000);
  }
  DSerial.println("\r\nConnect the SSL Sever Success!");

  if (_5GNBIoT.SSLSocketSendData(comm_socket_index, ssl_cmd)) {
    DSerial.println("\r\nSSL Socket Send Data Success!");
  }
}

void loop() {
  char ssl_event[16];
  unsigned int index;
  char recv_data[128];
  SSL_Socket_Event_t ret = _5GNBIoT.WaitCheckSSLSocketEvent(ssl_event, 2);
  switch (ret)
  {
    case SSL_SOCKET_CLOSE_EVENT:
      index = atoi(ssl_event);
      if (_5GNBIoT.CloseSSLSocket(index)) {
        DSerial.println("\r\nClose SSL Socket Success!");
      }
      break;
    case SSL_SOCKET_RECV_EVENT:
      index = atoi(ssl_event);
      if (_5GNBIoT.SSLSocketRecvData(index, 128, recv_data)) {
        DSerial.println("\r\nSSL Socket Recv Data Success!");
        DSerial.println("");
        DSerial.println(recv_data);
        DSerial.println("");
      }
      break;
    default:
      break;
  }
}
