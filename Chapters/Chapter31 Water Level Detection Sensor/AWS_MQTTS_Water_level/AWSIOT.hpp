
#ifndef __AWSIOT.H
#define __AWSIOT.H

#include "5G-NB-IoT_MQTT.h"

char aws_root_ca_pem[]= "-----BEGIN CERTIFICATE-----\n\
MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzElMCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp\n\
U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQwNjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE\n\
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3\n\
DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN\n\
+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa\n\
K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G\n\
A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fRzt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0\n\
YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w\n\
DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D\n\
eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJlxy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp\n\
VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEYWQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=\n\
-----END CERTIFICATE-----\n";

// "The certificate for this thing"
char certificate_pem_crt[] = "-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUWermTFtqBipvIdo9fw0sqlQmiUEwDQYJKoZIhvcNAQELBQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MTEyNTAwMjE1MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALalCvpF6hUcpAhQo+OAsUG28pCsbCpRpqcbw75MI22dD0V26uVH3f90kDUZrrRrfYKiUobc/QBrUi+wL27Z\n\
0+AG+4szfNT1HxTfG5cCp9AGQq/fFViD/d2vFDNwK7hfm3AV0ouIJaHg4qehrCwobvUu0GYoVU72g6wCfUQ9PMyscUBuU/BYa9P9NgUHM2VI46OEqGd+fhKGfLua6wiI\n\
TL+0uMZ102g+ixWnCVbixZg17DbXix2KyzofSj9Zcl2J2dqEyeLlnGI4pJ72vIwsFqm1n3vMSBdN33uDRALH1YTNd2+hxOWbtDKv5L9/dSCayXA9UmK9NUA4Tx1kmc24\n\
q98CAwEAAaNgMF4wHwYDVR0jBBgwFoAUEYvhKrZ37Neoj+bx7Y4QrtrNLDYwHQYDVR0OBBYEFE+2WYlcnvcn/baK2GWrsOOvhqqpMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCe8fojFAgQQWnf9bShNtxLzXLZSOLS6Xar4uSn2LK7UuORw63S6CovKcGSuTkv/Zea64W8xPI71nnWEQMqWwEPJSLP\n\
hxDelOQFMsrdd03HUzhaHxB69g3KqGoK7Rlx4u30w1sf2gYeyz5ajUx7OovpJm+VKCNdotnYmA3LLiG16u6A+0jt3N6aLC1cBYLMumo/0zgJr5jeRiv5jtYlh59SukjQ\n\
h2Yzg7LIgT9uOUzIjZ5hEF4mUT4jYPQz5MO4nTMQtWcL2RBKPgEMHLo6G4MM50xIfV8zUHJ2oh9RRkug6Y3kWdvC3HcO3onLLM9MCtOjOBDUnOsULvhCod4fFop6\n\
-----END CERTIFICATE-----\n";


// "The private key of the thing". The public key is not needed here
char private_pem_key[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAtqUK+kXqFRykCFCj44CxQbbykKxsKlGmpxvDvkwjbZ0PRXbq5Ufd/3SQNRmutGt9gqJShtz9AGtSL7AvbtnT4Ab7izN81PUfFN8blwKn0AZCr98V\n\
WIP93a8UM3AruF+bcBXSi4gloeDip6GsLChu9S7QZihVTvaDrAJ9RD08zKxxQG5T8Fhr0/02BQczZUjjo4SoZ35+EoZ8u5rrCIhMv7S4xnXTaD6LFacJVuLFmDXsNteL\n\
HYrLOh9KP1lyXYnZ2oTJ4uWcYjiknva8jCwWqbWfe8xIF03fe4NEAsfVhM13b6HE5Zu0Mq/kv391IJrJcD1SYr01QDhPHWSZzbir3wIDAQABAoIBAF+6HwfPjwGjskF9\n\
anh54e2sYnP7OvHvO6aIQc7CfysbT3679h9XzH3wbQi3vj5nvC+0vgZbWutWVIwUDrhNyOO5JSvvP3UqSDEbQo2iYawBLACbrsdm3q7K7Y6MMw3+ZiyX83MYV4kBoinz\n\
7ZZzVryA3c89QoERaS/FWjxi0DQBJutsO0daqpA5sTHlih1i7ULm4jFhP1y6zf/Q/TqX+xU6/xkV/qOgkV8sMUsgWr0W3sR+dkuj+gRG2UPDgftOMOOUatYz8bXxBhRt\n\
RTGWXG9stjyLzf+LrLUOLJTPPdbh10fVcQrFZjmDT/5944eLpIXIBHUyyJknIgiP4JHjLdECgYEA2QbHyejlyDI/BeX3I7oC9D3f3JkQReUbQv0HeEGDn+ABolZjMm+p\n\
ukjrRloGCfPk149W6JHN/C5Wk5HyK4Z7+HKFnIDupu8IyZdkrx1LCcKaS1Yl0/uMVPAjGinJ2VY05XUvsZ6fQEKIDI5pHcPDUCEtVHeGuqRNcipHyE6eHRkCgYEA13Gm\n\
jIGtCnIMwshjKyQZdXQtAyKC0FI/+IyYyrAGQ+ITkhDn5z9QudEgX1rYlxUAC9ed46Gf7IFoKDfGncHdQ4K2k0vtlWuI5x1lwVUgCs9iDzDaBCQH+/UWrjQ3kaJhGr3b\n\
A2DvEy9h0wkptCm+RmrOPZKPA3mKBqp2V3qtt7cCgYAdKUITKU4m1kmNm1bX4BihhObdl1uOaLh/CczFJdhv1QOjMoRltIdT1pR2gYYqNDFdnY8J9P1+kyNi9MtbediS\n\
i6DUZKasm5XMsKEAvqjkiNd+o/a1QymaCCJPU6ohn+m/oz1jWnL/iXEKbuBE9Zb631pd7SmXTphxbKj0i0vHkQKBgQDFcuxf82zCmPnm/j4cyK3ofjvOlq9q+yaTxU82\n\
25IyXpzvDj1rKxTx556SpbDA7kcnk0mp2HO4DnWF/nAyHv2JJN/tKPYqLe6/WarrP/fo0F2fbHcyuk7y9JD/wO14tGfoWT3C3/DcFV3DSq+E5Yj0AmjX91zIo3gDWfCx\n\
hcZ2CwKBgCFUIsL9o4IQOOpCdvw9HDxOu3EOuOuQJfDve4gddCBYGxzHvBc1BJXV7Ogvk2vpebfw1jv/F+CMk+b6kr/zzgb7lrcaCPvc3W7mb8MkuZ4Jw2xHAHRG2zuo\n\
nsfodqU1/jMSKuy477qsnneFRKiHTBymwlQ4dF46SnrSZQEl7lLA-----END RSA PRIVATE KEY-----\n";

// AWSIoT is the object of class _5G_NB_IoT_MQTT 
// DSerial USB serial
// APN name
// APIN login,
// APN password ,
// MQTT server FQDN, 
// MQTT port, number
// MQTT Client Id such as "BasibPubSub",
// MQTT topic name, 
// MQTT QoS  such as AT_MOST_ONCE, 
// MQTT index: The range is 0 ~ 5
// PDP index:  The range is 1 ~ 16
// SSL index:  The range is 0 ~ 5

//char *ModemIMEI  is an output contains the IMEI
bool InitModemMQTT(_5G_NB_IoT_MQTT  &AWSIoT, 
                   Stream &DSerial, 
                   char *APN,
                   char *LOGIN,
                   char *PASSWORD,
                   char *MQTTServer, 
                   unsigned int MQTTPort, 
                   char *MQTTClientId,
                   char *mqtt_topicName, 
                   Mqtt_Qos_t MQTT_QoS = AT_MOST_ONCE, 
                   unsigned int MQTTIndex = 3, 
                   unsigned int PDPIndex = 1, 
                   unsigned int SSLIndex = 2,
                   char *ModemIMEI = NULL)
{
  Mqtt_Version_t version = MQTT_V3;

  AWSIoT.InitModule();
  DSerial.println("\r\n InitModule() OK!");
  
  AWSIoT.SetDevCommandEcho(false);
  
  char inf[64];
  if (AWSIoT.GetDevInformation(inf))
  {
    DSerial.println(inf);
  }
  
  char imei_tmp[64];
  if (AWSIoT.GetDevIMEI(imei_tmp))
  {
    String s = String(imei_tmp);
    s.trim();
    s.toCharArray(ModemIMEI, 64);
    DSerial.println(ModemIMEI);
  }
  
  char filelist[64];
  AWSIoT.DeleteFiles("ca_cert.pem");
  AWSIoT.DeleteFiles("client_cert.pem");
  AWSIoT.DeleteFiles("client_key.pem");
  
  char apn_error[64];
  while (!AWSIoT.InitAPN(PDPIndex, APN, LOGIN, PASSWORD, apn_error)) 
  {
    DSerial.println(apn_error);
  }
  DSerial.println(apn_error);
  
  char ssl_error[128];
  while (!AWSIoT.InitSSL(SSLIndex, aws_root_ca_pem, certificate_pem_crt, private_pem_key, ssl_error)) 
  {
    DSerial.println(ssl_error);
  }
  DSerial.println(ssl_error);
  
  DSerial.println("\r\nStart Config the MQTT Parameter!");
  while (!AWSIoT.SetMQTTConfigureParameters(MQTTIndex, PDPIndex, version, 150, SERVER_STORE_SUBSCRIPTIONS)) 
  {
    DSerial.println("\r\nConfig the MQTT Parameter Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
  }
  DSerial.println("\r\nConfig the MQTT Parameter Success!");
  
  while (!AWSIoT.SetMQTTEnableSSL(MQTTIndex, SSLIndex, true)) 
  {
    DSerial.println("\r\SetMQTTEnableSSL the MQTT Parameter Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
  }
  DSerial.println("\r\SetMQTTEnableSSL the MQTT Parameter Success!");
  
  while (AWSIoT.OpenMQTTNetwork(MQTTIndex, MQTTServer, MQTTPort) != 0) 
  {
    DSerial.println("\r\nSet the MQTT Service Address Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
  }
  DSerial.println("\r\nSet the MQTT Service Address Success!");
  
  DSerial.println("\r\nConfigure Timeout!");
  while(!AWSIoT.SetMQTTMessageTimeout(MQTTIndex, 10, 5, 1))
  {
      DSerial.println("\r\nMQTT Timeout Fail!");
      int e_code;
      if (AWSIoT.returnErrorCode(e_code)) 
      {
        DSerial.print("\r\nERROR CODE: ");
        DSerial.println(e_code);
        DSerial.println("Please check the documentation for error details.");
      }
    }
  
  DSerial.println("\r\nStart Create a MQTT Client!");
  while (AWSIoT.CreateMQTTClient(MQTTIndex, MQTTClientId, "", "") != 0) 
  {
    DSerial.println("\r\nCreate a MQTT Client Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
  }
  DSerial.println("\r\nCreate a MQTT Client Success!");
  
  DSerial.println("\r\nStart MQTT Subscribe Topic!");
  while (AWSIoT.MQTTSubscribeTopic(MQTTIndex, 1, mqtt_topicName, MQTT_QoS) != 0) 
  {
    DSerial.println("\r\nMQTT Subscribe Topic Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
  }
  DSerial.println("\r\nMQTT Subscribe Topic Success!");

  return true;
}

#endif
