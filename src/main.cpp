#include <Arduino.h>

/*
  Author: Per Rose
  Created:  16-04-2020 15:00:00
  Kamstrup reading algorithm, developed by:
            https://github.com/TorbenNor/Kamstrup-Radius-Blynk
*/
#include <WiFi.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32_FTPClient.h>
#include "SPIFFS.h"
#include <mbusparserkam.h>
#include <mbedtls/gcm.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include "user-defined-variables.h"

#define uS_TO_S_FACTOR 1000000ULL //Conversion factor for micro seconds to seconds
//  								2.0.0				---- First official release
//                 "2.0.1"; //  ---- Small error cor
//                 "2.0.2"; //  ---- DST correction, FTP backup, and small error corrections
//                 "3.0.0"; //  ---- Replaced Arduino EDI, with VSCode and PlatformIO
const String rel = "3.1.1"; //  ---- Optimised using VSCode and PlatformIO

// Date and time syncronize
const char *ntpServer = "pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

ESP32_FTPClient ftp(ftp_server, ftp_user, ftp_pass);

// mqtt constants
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Reboot counters
RTC_DATA_ATTR int sleep5no = 0;

// Date variables
String formattedDate;
String dayStamp;
String timeStamp;
String timeStamp1; // maybe not used
String timeMinute;
int timeMinuteInt;
int thisMonth;
String thisDayStr;
String lastDayStr;
String thisMonthStr;
String lastMonthStr;

// Various variables
String readString;
float twelveNetto[14];
float twelveNettoTotal;
float nettoMonth;
float MeterToLastMonthEnergyExp;
float MeterToLastMonthEnergyImp;
float MeterEnergyTotImpNow;
float MeterEnergyTotExpNow;

// This variable is used for check of network connection.
long rssi;

// Kamstrup constants
const size_t headersize = 11;
const size_t footersize = 3;
uint8_t encryption_key[16];
uint8_t authentication_key[16];
uint8_t receiveBuffer[500];
uint8_t decryptedFrameBuffer[500];
VectorView decryptedFrame(decryptedFrameBuffer, 0);
MbusStreamParser streamParser(receiveBuffer, sizeof(receiveBuffer));
mbedtls_gcm_context m_ctx;
#define RXD2 16
#define TXD2 17

// Include subroutines
#include <goToDeepSleepFiveMinutes.h>
#include <fileManagement.h>
#include <initialize.h>
#include <getLast12monthsData.h>
#include <sendData.h>
#include <printHex.h>
#include <decrypt.h>
#include <hexStr2bArr.h>
#include <connectToNetwork.h>

void setup()
{
  // initialize serial communication:
  Serial.begin(115200);
  Serial.print("Release version: ");
  Serial.println(rel);

  // Start WiFi and update time
  connectToNetwork();
  Serial.println(" ");
  Serial.println("Connected to network");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);

  // Connect to mqtt broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  mqttClient.setServer(broker, 1883);
  // Mount file system
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    goToDeepSleepFiveMinutes();
  }
  listDir(SPIFFS, "/", 0);
  // Initiate historic variables, at first start of program, or if adjustments are needed.
  if (initiate)
  {
    Initialize();
  }

  // Start serial connection to Kamstrup
  while (!Serial2.available())
  {
    Serial.println("Connect to Serial2");
    Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
    int WLcount = 0;
    int UpCount = 0;
    while (!Serial2.available() && WLcount < 200)
    {
      delay(100);
      Serial.printf(".");
      if (UpCount >= 60) // just keep terminal from scrolling sideways
      {
        UpCount = 0;
        Serial.printf("\n");
      }
      ++UpCount;
      ++WLcount;
    }
  }

  // Start connection to Kamstrup Meter
  if (!Serial2.available())
  {
    Serial.println("ingen forbindelse til måler");
    goToDeepSleepFiveMinutes();
  }
  hexStr2bArr(encryption_key, conf_key, sizeof(encryption_key));
  hexStr2bArr(authentication_key, conf_authkey, sizeof(authentication_key));

  // Mount file system
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    goToDeepSleepFiveMinutes();
  }
  listDir(SPIFFS, "/", 0);

  // Now we start reading the files..
  getLast12monthsData();
  readString = "";

  readFile(SPIFFS, "/lastMonth.txt");
  lastMonthStr = readString;
  readString = "";

  Serial.println(lastMonthStr);

  readFile(SPIFFS, "/lastDay.txt");
  lastDayStr = readString;
  readString = "";
  //  Serial.println(lastMonthInt);
  readFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt");
  MeterToLastMonthEnergyExp = readString.toFloat();
  readString = "";
  readFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt");
  MeterToLastMonthEnergyImp = readString.toFloat();
  readString = "";
  //  Serial.print("Energi til sidste måned: ");
  //  Serial.println(yearToLastMonthEnergy);

  Serial.println("Setup completed");
}

void loop()
{

  rssi = WiFi.RSSI();
  if (rssi == 0)
  {
    WiFi.disconnect();
    connectToNetwork();
  }

#include <timeManagement.h>

  // Read Kamstrup meter
  while (Serial2.available())
  {
    //  Serial.println("serial2 active");
    if (streamParser.pushData(Serial2.read()))
    {
      VectorView frame = streamParser.getFrame();
      if (streamParser.getContentType() == MbusStreamParser::COMPLETE_FRAME)
      {
        Serial.println("Frame complete");
        if (!decrypt(frame))
        {
          Serial.println("Decryption failed");
          return;
        }
        MeterData md = parseMbusFrame(decryptedFrame);
        sendData(md);
      }
    }
  }
}
