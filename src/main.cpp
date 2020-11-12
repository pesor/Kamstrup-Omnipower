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

#define uS_TO_S_FACTOR 1000000ULL  //Conversion factor for micro seconds to seconds
//  								2.0.0				---- First official release
//                 "2.0.1"; //  ---- Small error cor
//                 "2.0.2"; //  ---- DST correction, FTP backup, and small error corrections
const String rel = "3.0.0"; //  ---- Replaced Arduino EDI, with VSCode and PlatformIO

// Date and time syncronize
const char* ntpServer = "pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

ESP32_FTPClient ftp (ftp_server, ftp_user, ftp_pass);

// mqtt constants
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Reboot counters
RTC_DATA_ATTR int sleep5no = 0;

// Date variables
String formattedDate;
String dayStamp;
String timeStamp;
String timeStamp1;  // maybe not used
String timeMinute;
int  timeMinuteInt;
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
const char conf_key[33] = "AD1DD3B8653BE3D0DBA025389538D1AB"; //Meter Key
const char conf_authkey[33] = "107CF59A53D6BC10AD79E554653BCD36"; //Meter Key
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

// Ready to start

void Initialize() {
  //When initialising the program, it is important to set the current months data (lastMonth = current month) after initialisation, all initialisation variables have to be commented out.

  //   writeFile(SPIFFS, "/lastDay.txt", "27");
  // writeFile(SPIFFS, "/lastMonth.txt", "10");

  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt", "7037.65");
  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt", "7815,72");

  //char Netto12Months[] = R"raw({"netto12":{"Jan":"0","Feb":"0","Mar":"107.84","Apr":"-242.80","Maj":"-311.25","Jun":"-309.09","Jul":"-154.08","Aug":"-11.45","Sep":"30.95","Okt":"236.10","Nov":"0","Dec":"0"}})raw";
  //writeFile(SPIFFS, "/last12.txt", Netto12Months);

  //  char AllYears[] = R"raw({"lastYears":{"2013":"5160","2014":"5668","2015":"5830","2016":"5820","2017":"5410","2018":"6150","2019":"5790","2020":"5440","2021":"0","2022":"0","2023":"0","2024":"0","2025":"0"}})raw";
  //  writeFile(SPIFFS, "/allYears.txt", AllYears);

  Serial.println("DID YOU REMEMBER TO UNCOMMENT / COMMENT THE VARIABLES YOU WANT TO USE / NOT USE ---- IMPORTANT!!!!!!!!!!!");

  // END Initialisation
}

void goToDeepSleepFiveMinutes()
{
  Serial.print("Going to sleep... ");
  Serial.print("300");
  Serial.println(" sekunder");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  esp_wifi_stop();
  esp_bt_controller_disable();

  // Configure the timer to wake us up!
  ++sleep5no;
  esp_sleep_enable_timer_wakeup(300 * uS_TO_S_FACTOR);
}

// Write files
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    goToDeepSleepFiveMinutes();
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- frite failed");
  }
}

// Read files
void readFile(fs::FS &fs, const char * path) {
  //  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    goToDeepSleepFiveMinutes();
  }

  //  Serial.println("- read from file:");
  while (file.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = file.read();
    readString += c;
  }
  //  Serial.println(readString);
  file.close();
}

// List files on module
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    goToDeepSleepFiveMinutes();
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    goToDeepSleepFiveMinutes();
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void getLast12monthsData() {
  // Get the last 12 months data, excl current month, which is not included.
  readString = "";
  readFile(SPIFFS, "/last12.txt");
  Serial.print("readString last 12: ");
  Serial.println(readString);

  DynamicJsonDocument last12Netto(1024);

  // Parse JSON object
  DeserializationError errorL12 = deserializeJson(last12Netto, readString);
  if (errorL12) {
    Serial.print(F("deserializeJson() Last 12 Recv failed: "));
    Serial.println(errorL12.c_str());
    goToDeepSleepFiveMinutes();
  }

  twelveNetto[1] = last12Netto["netto12"]["Jan"].as<float>();
  twelveNetto[2] = last12Netto["netto12"]["Feb"].as<float>();
  twelveNetto[3] = last12Netto["netto12"]["Mar"].as<float>();
  twelveNetto[4] = last12Netto["netto12"]["Apr"].as<float>();
  twelveNetto[5] = last12Netto["netto12"]["Maj"].as<float>();
  twelveNetto[6] = last12Netto["netto12"]["Jun"].as<float>();
  twelveNetto[7] = last12Netto["netto12"]["Jul"].as<float>();
  twelveNetto[8] = last12Netto["netto12"]["Aug"].as<float>();
  twelveNetto[9] = last12Netto["netto12"]["Sep"].as<float>();
  twelveNetto[10] = last12Netto["netto12"]["Okt"].as<float>();
  twelveNetto[11] = last12Netto["netto12"]["Nov"].as<float>();
  twelveNetto[12] = last12Netto["netto12"]["Dec"].as<float>();

  for (int i = 1; i < 13; i++) {
    twelveNetto[13] += twelveNetto[i];
    Serial.print("twelveNetto(i)");
    Serial.println(twelveNetto[i]);
  }
  Serial.print("twelveNetto(13)");
  Serial.println(twelveNetto[13]);
}

void sendData(MeterData md) {
  // Send the MeterData to MQTT

  Serial.println(thisMonthStr);
  Serial.println(lastMonthStr);
  // if month change, we need to update the netto for the old month.
  if (thisMonthStr != lastMonthStr) {

    int lastMonthArr = lastMonthStr.toInt();
    twelveNetto[lastMonthArr] = nettoMonth / 1000;

    Serial.println(lastMonthArr);
    writeFile(SPIFFS, "/lastMonth.txt", thisMonthStr.c_str());
    lastMonthStr = thisMonthStr;

    String MeterEnergyTotExpNowStr = String(MeterEnergyTotExpNow);
    String MeterEnergyTotImpNowStr = String(MeterEnergyTotImpNow);
    writeFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt", MeterEnergyTotExpNowStr.c_str());
    writeFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt", MeterEnergyTotImpNowStr.c_str());
    MeterToLastMonthEnergyImp = MeterEnergyTotImpNow;
    MeterToLastMonthEnergyExp = MeterEnergyTotExpNow;
  }

  // Allocate a  JsonDocument
  StaticJsonDocument<1792> docSend;
  JsonObject root = docSend.to<JsonObject>();

  JsonObject output = root.createNestedObject("output");

  if (md.activePowerPlusValid) {
    // Serial.println( "/power/activePowerPlus " + String(md.activePowerPlus));
    output["activePowerPlus"] = String(md.activePowerPlus);
  }
  if (md.activePowerMinusValid) {
    // Serial.println(  "/power/activePowerMinus " + String(md.activePowerMinus));
    output["activePowerMinus"] = String(md.activePowerMinus);
  }
  if (md.activePowerPlusValidL1) {
    // Serial.println(  "/power/activePowerPlusL1 " + String(md.activePowerPlusL1));
    output["activePowerPlusL1"] = String(md.activePowerPlusL1);
  }
  if (md.activePowerMinusValidL1) {
    // Serial.println(  "/power/activePowerMinusL1 " + String(md.activePowerMinusL1));
    output["activePowerMinusL1"] = String(md.activePowerMinusL1);
  }
  if (md.activePowerPlusValidL2) {
    // Serial.println(  "/power/activePowerPlusL2 " + String(md.activePowerPlusL2));
    output["activePowerPlusL2"] = String(md.activePowerPlusL2);
  }
  if (md.activePowerMinusValidL2) {
    // Serial.println(  "/power/activePowerMinusL2 " + String(md.activePowerMinusL2));
    output["activePowerMinusL2"] = String(md.activePowerMinusL2);
  }
  if (md.activePowerPlusValidL3) {
    // Serial.println(  "/power/activePowerPlusL3 " + String(md.activePowerPlusL3));
    output["activePowerPlusL3"] = String(md.activePowerPlusL3);
  }
  if (md.activePowerMinusValidL3) {
    // Serial.println(  "/power/activePowerMinusL3 " + String(md.activePowerMinusL3));
    output["activePowerMinusL3"] = String(md.activePowerMinusL3);
  }
  if (md.reactivePowerPlusValid) {
    // Serial.println(  "/power/reactivePowerPlus " + String(md.reactivePowerPlus));
    output["reactivePowerPlus"] = String(md.reactivePowerPlus);
  }
  if (md.reactivePowerMinusValid) {
    // Serial.println(  "/power/reactivePowerMinus " + String(md.reactivePowerMinus));
    output["reactivePowerMinus"] = String(md.reactivePowerMinus);
  }
  if (md.powerFactorValidL1) {
    // Serial.println(  "/power/powerFactorL1 " + String(md.powerFactorL1));
    output["powerFactorL1"] = String(md.powerFactorL1);
  }
  if (md.powerFactorValidL2) {
    // Serial.println(  "/power/powerFactorL2 " + String(md.powerFactorL2));
    output["powerFactorL2"] = String(md.powerFactorL2);
  }
  if (md.powerFactorValidL3) {
    // Serial.println(  "/power/powerFactorL3 " + String(md.powerFactorL3));
    output["powerFactorL3"] = String(md.powerFactorL3);
  }
  if (md.powerFactorTotalValid) {
    // Serial.println(  "/power/powerFactorTotal " + String(md.powerFactorTotal));
    output["powerFactorTotal"] = String(md.powerFactorTotal);
  }
  if (md.voltageL1Valid) {
    // Serial.println(  "/voltage/L1 " + String(md.voltageL1));
    output["voltageL1"] = String(md.voltageL1);
  }
  if (md.voltageL2Valid) {
    // Serial.println(  "/voltage/L2 " + String(md.voltageL2));
    output["voltageL2"] = String(md.voltageL2);
  }
  if (md.voltageL3Valid) {
    // Serial.println(  "/voltage/L3 " + String(md.voltageL3));
    output["voltageL3"] = String(md.voltageL3);
  }
  if (md.centiAmpereL1Valid) {
    // Serial.println(  "/current/L1 " + String(md.centiAmpereL1 / 100.));
    output["centiAmpereL1"] = String(md.centiAmpereL1 / 100.);
  }
  if (md.centiAmpereL2Valid) {
    // Serial.println(  "/current/L2 " + String(md.centiAmpereL2 / 100.));
    output["centiAmpereL2"] = String(md.centiAmpereL2 / 100.);
  }
  if (md.centiAmpereL3Valid) {
    // Serial.println(  "/current/L3 " + String(md.centiAmpereL3 / 100.));
    output["centiAmpereL3"] = String(md.centiAmpereL3 / 100.);
  }
  if (md.activeImportWhValid) {
    // Serial.println(  "/energy/activeImportKWh " + String(md.activeImportWh / 1000.));
    output["activeImportWh"] = String(md.activeImportWh / 1000.);
  }
  if (md.activeExportWhValid) {
    // Serial.println(  "/energy/activeExportKWh " + String(md.activeExportWh / 1000.));
    output["activeExportWh"] = String(md.activeExportWh / 1000.);
  }
  if (md.activeImportWhValidL1) {
    // Serial.println(  "/energy/activeImportKWhL1 " + String(md.activeImportWhL1 / 1000.));
    output["activeImportWhL1"] = String(md.activeImportWhL1 / 1000.);
  }
  if (md.activeExportWhValidL1) {
    // Serial.println(  "/energy/activeExportKWhL1 " + String(md.activeExportWhL1 / 1000.));
    output["activeExportWhL1"] = String(md.activeExportWhL1 / 1000.);
  }
  if (md.activeImportWhValidL2) {
    // Serial.println(  "/energy/activeImportKWhL2 " + String(md.activeImportWhL2 / 1000.));
    output["activeImportWhL2"] = String(md.activeImportWhL2 / 1000.);
  }
  if (md.activeExportWhValidL2) {
    // Serial.println(  "/energy/activeExportKWhL2 " + String(md.activeExportWhL2 / 1000.));
    output["activeExportWhL2"] = String(md.activeExportWhL2 / 1000.);
  }
  if (md.activeImportWhValidL3) {
    // Serial.println(  "/energy/activeImportKWhL3 " + String(md.activeImportWhL3 / 1000.));
    output["activeImportWhL3"] = String(md.activeImportWhL3 / 1000.);
  }
  if (md.activeExportWhValidL3) {
    // Serial.println(  "/energy/activeExportKWhL3 " + String(md.activeExportWhL3 / 1000.));
    output["activeExportWhL3"] = String(md.activeExportWhL3 / 1000.);
  }
  // Calculate netto for the month


  MeterEnergyTotImpNow = md.activeImportWh / 1000.;
  MeterEnergyTotExpNow = md.activeExportWh / 1000.;
  nettoMonth = (MeterEnergyTotImpNow - MeterToLastMonthEnergyImp) - (MeterEnergyTotExpNow - MeterToLastMonthEnergyExp);
  output["nettoMonthTot"] = String(nettoMonth);
  twelveNetto[thisMonth] = nettoMonth;

  if (md.reactiveImportWhValid) {
    // Serial.println(  "/energy/reactiveImportKWh " + String(md.reactiveImportWh / 1000.));
    output["reactiveImportWh"] = String(md.reactiveImportWh / 1000.);
  }
  if (md.reactiveExportWhValid) {
    // Serial.println(  "/energy/reactiveExportKWh " + String(md.reactiveExportWh / 1000.));
    output["reactiveExportWh"] = String(md.reactiveExportWh / 1000.);
  }

  String timeStampKamstrup = formattedDate;
  output["dayStampKamstrup"] = timeStampKamstrup;

  float NettoSum = 0.00;

  for (int i = 1; i < 13; i++) {
    NettoSum += twelveNetto[i];
  }

  output["Jan"] = String(twelveNetto[1]);
  output["Feb"] = String(twelveNetto[2]);
  output["Mar"] = String(twelveNetto[3]);
  output["Apr"] = String(twelveNetto[4]);
  output["Maj"] = String(twelveNetto[5]);
  output["Jun"] = String(twelveNetto[6]);
  output["Jul"] = String(twelveNetto[7]);
  output["Aug"] = String(twelveNetto[8]);
  output["Sep"] = String(twelveNetto[9]);
  output["Okt"] = String(twelveNetto[10]);
  output["Nov"] = String(twelveNetto[11]);
  output["Dec"] = String(twelveNetto[12]);
  output["netto12"] = String(NettoSum);

  // if day change, we save the 12 last months netto
  if (thisDayStr != lastDayStr) {
    File f12 = SPIFFS.open("/last12.txt", "w");

    StaticJsonDocument<512> doc12;
    JsonObject root = doc12.to<JsonObject>();
    JsonObject netto12 = root.createNestedObject("netto12");

    // Now we set the 12 last months production
    netto12["Jan"] = String(twelveNetto[1]);
    netto12["Feb"] = String(twelveNetto[2]);
    netto12["Mar"] = String(twelveNetto[3]);
    netto12["Apr"] = String(twelveNetto[4]);
    netto12["Maj"] = String(twelveNetto[5]);
    netto12["Jun"] = String(twelveNetto[6]);
    netto12["Jul"] = String(twelveNetto[7]);
    netto12["Aug"] = String(twelveNetto[8]);
    netto12["Sep"] = String(twelveNetto[9]);
    netto12["Okt"] = String(twelveNetto[10]);
    netto12["Nov"] = String(twelveNetto[11]);
    netto12["Dec"] = String(twelveNetto[12]);
    netto12["netto12"] = String(NettoSum);

    // Serialize JSON to file
    if (serializeJson(doc12, f12) == 0) {
      Serial.println(F("Failed to write to file"));
    }

    f12.close();

    Serial.println("Date change in effect");
  }

  // Connect to mqtt broker
  //  Serial.print("Attempting to connect to the MQTT broker: ");
  //  Serial.println(broker);
  if (!mqttClient.connected()) {
    mqttClient.setServer(broker, 1883);
    if (!mqttClient.connect(broker, mqttuser, mqttpass)) {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.state());
      goToDeepSleepFiveMinutes();
    }
  }

  // Send to mqtt
  char buffer[1792];
  serializeJson(docSend, buffer);

  Serial.println("Sending message to topic: ");
  Serial.print(topic);
  Serial.print(" ");
  Serial.println(buffer);

  // send message, the Print interface can be used to set the message contents

  bool retained = true;
    if (mqttClient.publish(topic, buffer, retained)) {
    Serial.println("Message published successfully");
  } else {
    Serial.println("Error in Message, not published");
  }
  Serial.println();
  mqttClient.disconnect();

  if (thisDayStr != lastDayStr) {

    // FTP to backup every day
    ftp.OpenConnection();
    //Change directory
    ftp.ChangeWorkDir("/docker/kamstrup");
    //upload the text file or read it
    String response = "";
    ftp.InitFile("Type A");
    ftp.NewFile("kamstrup_mqtt.json");
    ftp.Write(buffer);

    ftp.CloseConnection();
    
    writeFile(SPIFFS, "/lastDay.txt", thisDayStr.c_str());
    lastDayStr = thisDayStr;
  }
}

void printHex(const unsigned char* data, const size_t length) {
  for (int i = 0; i < length; i++) {
    Serial.printf("%02X", data[i]);
  }
}

void printHex(const VectorView & frame) {
  for (int i = 0; i < frame.size(); i++) {
    Serial.printf("%02X", frame[i]);
  }
}

bool decrypt(const VectorView & frame) {

  if (frame.size() < headersize + footersize + 12 + 18) {
    Serial.println("Invalid frame size.");
  }

  memcpy(decryptedFrameBuffer, &frame.front(), frame.size());

  uint8_t system_title[8];
  memcpy(system_title, decryptedFrameBuffer + headersize + 2, 8);

  uint8_t initialization_vector[12];
  memcpy(initialization_vector, system_title, 8);
  memcpy(initialization_vector + 8, decryptedFrameBuffer + headersize + 14, 4);

  uint8_t additional_authenticated_data[17];
  memcpy(additional_authenticated_data, decryptedFrameBuffer + headersize + 13, 1);
  memcpy(additional_authenticated_data + 1, authentication_key, 16);

  uint8_t authentication_tag[12];
  memcpy(authentication_tag, decryptedFrameBuffer + headersize + frame.size() - headersize - footersize - 12, 12);

  uint8_t cipher_text[frame.size() - headersize - footersize - 18 - 12];
  memcpy(cipher_text, decryptedFrameBuffer + headersize + 18, frame.size() - headersize - footersize - 12 - 18);

  uint8_t plaintext[sizeof(cipher_text)];

  mbedtls_gcm_init(&m_ctx);
  int success = mbedtls_gcm_setkey(&m_ctx, MBEDTLS_CIPHER_ID_AES, encryption_key, sizeof(encryption_key) * 8);
  if (0 != success) {
    Serial.println("Setkey failed: " + String(success));
    return false;
  }
  success = mbedtls_gcm_auth_decrypt(&m_ctx, sizeof(cipher_text), initialization_vector, sizeof(initialization_vector),
                                     additional_authenticated_data, sizeof(additional_authenticated_data), authentication_tag, sizeof(authentication_tag),
                                     cipher_text, plaintext);
  if (0 != success) {
    Serial.println("authdecrypt failed: " + String(success));
    return false;
  }
  mbedtls_gcm_free(&m_ctx);

  //copy replace encrypted data with decrypted for mbusparser library. Checksum not updated. Hopefully not needed
  memcpy(decryptedFrameBuffer + headersize + 18, plaintext, sizeof(plaintext));
  decryptedFrame = VectorView(decryptedFrameBuffer, frame.size());

  return true;
}

void hexStr2bArr(uint8_t* dest, const char* source, int bytes_n)
{
  uint8_t* dst = dest;
  uint8_t* end = dest + sizeof(bytes_n);
  unsigned int u;

  while (dest < end && sscanf(source, "%2x", &u) == 1)
  {
    *dst++ = u;
    source += 2;
  }
}

// Actual routine to connect to the WIFI network
void connectToNetwork() {
  Serial.print("Size of SSID array ");
  Serial.println(ssidArrNo);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  bool breakLoop = false;
  for (int i = 0; i <= ssidArrNo; i++) {
    ssid = ssidArr[i].c_str();
    Serial.print("SSID name: ");
    Serial.print(ssidArr[i]);

    while ( WiFi.status() !=  WL_CONNECTED )
    {
      // wifi down, reconnect here
      WiFi.begin(ssid, password);
      int WLcount = 0;
      int UpCount = 0;
      while (WiFi.status() != WL_CONNECTED )
      {
        delay( 100 );
        Serial.printf(".");
        if (UpCount >= 60)  // just keep terminal from scrolling sideways
        {
          UpCount = 0;
          Serial.printf("\n");
        }
        ++UpCount;
        ++WLcount;
        if (WLcount > 200) {
          Serial.println("we should break");
          breakLoop = true;
          break;
        }
      }
      if (breakLoop) {
        breakLoop = false;
        break;
      }
    }
  }
  if (WiFi.status() !=  WL_CONNECTED) {
    goToDeepSleepFiveMinutes();
  }
}

void setup() {
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

  // Start serial connection to Kamstrup
  while (!Serial2.available()) {
    Serial.println("Connect to Serial2");
    Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
    int WLcount = 0;
    int UpCount = 0;
    while (!Serial2.available()  && WLcount < 200 ) {
      delay( 100 );
      Serial.printf(".");
      if (UpCount >= 60)  // just keep terminal from scrolling sideways
      {
        UpCount = 0;
        Serial.printf("\n");
      }
      ++UpCount;
      ++WLcount;
    }
  }

  // Start connection to Kamstrup Meter
  if (!Serial2.available() ) {
    Serial.println("ingen forbindelse til måler");
    goToDeepSleepFiveMinutes();
  }
  hexStr2bArr(encryption_key, conf_key, sizeof(encryption_key));
  hexStr2bArr(authentication_key, conf_authkey, sizeof(authentication_key));


  // Mount file system
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    goToDeepSleepFiveMinutes();
  }
  listDir(SPIFFS, "/", 0);

  // Initiate historic variables, at first start of program, or if adjustments are needed.
  if (initiate) {
    Initialize();
  }

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

void loop() {

  rssi = WiFi.RSSI();
  if (rssi == 0) {
    WiFi.disconnect();
    connectToNetwork();
  }

  timeClient.setTimeOffset(gmtOffset_sec);
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  dayStamp = dayStamp.substring(5);
  String dateMonth = dayStamp.substring(0, 2);
  String dateDay = dayStamp.substring(3, 5);
  //  Serial.print("dateMonth: ");
  //  Serial.println(dateMonth);
  //  Serial.print("dateDay: ");
  //  Serial.println(dateDay);
  dayStamp = dateDay + "-" + dateMonth;
  //  Serial.print("dayStamp ");
  //  Serial.println(dayStamp);
  //  config.date = dayStamp;
  // Extract time
  timeStamp1 = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
  //  Serial.print("timeStamp1 ");
  //  Serial.println(timeStamp1);
  //  config.time = timeStamp1.substring(0, 5);
  //  Serial.print("Day ");
  //  Serial.println(timeClient.getDay());
  // variables needed for DST test
  int thisHour = timeClient.getHours();
  int thisDay = dateDay.toInt();
  thisDayStr = dateDay;
  thisMonthStr = dateMonth;
  thisMonth = dateMonth.toInt();
  //  int thisMonth = 2; // Test purposes
  int thisWeekday = timeClient.getDay();
  bool dst = false;

  // Test for DST active
  // Test for DST active
  if (thisMonth == 11 && thisDay < 8 && thisDay < thisWeekday)
  {
    Serial.println("1");
    dst = true;
  }

  if (thisMonth == 11 && thisDay < 8 && thisWeekday == 1 && thisHour < 1)
  {
    Serial.println("2");
    dst = true;
  }

  if (thisMonth < 11 && thisMonth > 3) {
    Serial.println("3");
    dst = true;
  }

  if (thisMonth == 3 && thisDay > 7 && thisDay >= (thisWeekday + 7))
  {
    Serial.println("4");
    if (!(thisWeekday == 1 && thisHour < 2)) {
      Serial.println("5");
      dst = true;
    }
  }

  if (dst) {
    Serial.println("IN SOMMERTIME");
    timeClient.setTimeOffset(gmtOffset_sec + 3600);
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
    // The formattedDate comes with the following format:
    // 2018-05-28T16:00:13Z
    // We need to extract date and time
    formattedDate = timeClient.getFormattedDate();
    // Extract date
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    dayStamp = dayStamp.substring(5);
    String dateMonth = dayStamp.substring(0, 2);
    String dateDay = dayStamp.substring(3, 5);
    Serial.print("dateMonth: ");
    Serial.println(dateMonth);
    Serial.print("dateDay: ");
    Serial.println(dateDay);
    dayStamp = dateDay + "-" + dateMonth;
    Serial.print("dayStamp ");
    Serial.println(dayStamp);
    //  config.date = dayStamp;
    // Extract time
    timeStamp1 = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
    Serial.print("timeStamp1 ");
    Serial.println(timeStamp1);
    //  config.time = timeStamp1.substring(0, 5);
    Serial.print("Day ");
    Serial.println(timeClient.getDay());
  }

  // Read Kamstrup meter
  while (Serial2.available()) {
    //  Serial.println("serial2 active");
    if (streamParser.pushData(Serial2.read())) {
      VectorView frame = streamParser.getFrame();
      if (streamParser.getContentType() == MbusStreamParser::COMPLETE_FRAME) {
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
