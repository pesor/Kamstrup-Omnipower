// Read the Fronius Data Module
void getFroniusJSONData()
{
  // Get the JSON data from the Fronius Datamanager.
  if (!wifiClient.connect("192.168.1.239", 80))
  {
    Serial.println(F("Connection failed"));
    goToDeepSleepFiveMinutes();
  }
  wifiClient.setTimeout(10000);

  Serial.println(F("Connected to Fronius Datamanager"));

  //Serial.println("Buffer from last run ");
  //Serial.println(buffer);

  // Send HTTP request
  wifiClient.println(F("GET /solar_api/v1/GetInverterRealtimeData.cgi?Scope=Device&DeviceId=1&DataCollection=CommonInverterData HTTP/1.0"));
  wifiClient.println(F("Host: 192.168.1.239"));
  wifiClient.println(F("Connection: close"));
  if (wifiClient.println() == 0)
  {
    Serial.println(F("Failed to send request"));
    goToDeepSleepFiveMinutes();
  }

  Serial.println(F("Connected to Fronius Datamanager 1"));

  // Check HTTP status
  char status[32] = {0};
  wifiClient.readBytesUntil('\r', status, sizeof(status));
  Serial.print(F("wifiClient response: "));
  Serial.println(status);

  if (strcmp(status, "HTTP/1.0 200 OK") != 0)
  {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    goToDeepSleepFiveMinutes();
  }
  Serial.println(F("Connected to Fronius Datamanager 2"));
  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!wifiClient.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    goToDeepSleepFiveMinutes();
  }
  Serial.println(F("Connected to Fronius Datamanager 3"));
  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  // const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;

  DynamicJsonDocument doc(1792);

  /*
  ReadLoggingStream loggingStream(wifiClient, Serial);
*/

  Serial.println(F("Connected to Fronius Datamanager 3a"));

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, wifiClient);
  if (error)
  {
    Serial.print(F("deserializeJson() doc failed: "));
    Serial.println(error.c_str());
    goToDeepSleepFiveMinutes();
  }

  Serial.println(F("Connected to Fronius Datamanager 3b"));

  // #############################################################################################
  // Are we sending data to MQTT, we do not do this after sunset, or if UDC = 0
  // #############################################################################################
  String UDCstr = doc["Body"]["Data"]["UDC"]["Value"].as<String>();
  // check if there still is power on the Fronius converter.
  if (UDCstr.toInt() == 0 || doc["Body"]["Data"]["PAC"]["Value"].as<String>() == "null")
  {
    if (thisDay != lastDayInt.toInt())
    {
      lastDayInt = String(thisDay);
      writeFile(SPIFFS, "/lastDay.txt", lastDayInt.c_str());
    }
    if (thisHour == 23)
    {
      if (thisMinute == 59)
      {
        if (thisSecond > 39)
        {
          Serial.println("ingen data fra Fronius");
          Serial.println(buffer);
#include <ftp-buffer.h>
        }
      }
    }
  }
  else
  {
    Serial.println(F("Connected to Fronius Datamanager 4"));
    // Allocate a  JsonDocument
    StaticJsonDocument<1792> docSend;
    JsonObject root = docSend.to<JsonObject>();

    JsonObject now = root.createNestedObject("now");
    Serial.println(F("Connected to Fronius Datamanager 5"));
    // Set the values in the document
    // Device changes according to device placement
    now["PAC"] = doc["Body"]["Data"]["PAC"]["Value"].as<String>();
    now["FAC"] = doc["Body"]["Data"]["FAC"]["Value"].as<String>();
    now["IAC"] = doc["Body"]["Data"]["IAC"]["Value"].as<String>();
    now["IDC"] = doc["Body"]["Data"]["IDC"]["Value"].as<String>();
    float totalEnergy = doc["Body"]["Data"]["TOTAL_ENERGY"]["Value"].as<float>() / 1000;
    now["TOTAL_ENERGY"] = String(totalEnergy);
    now["UAC"] = doc["Body"]["Data"]["UAC"]["Value"].as<String>();
    now["UDC"] = doc["Body"]["Data"]["UDC"]["Value"].as<String>();
    now["Day_Energy"] = String(producedNow / 1000);
    yearEnergy = doc["Body"]["Data"]["YEAR_ENERGY"]["Value"].as<float>() / 1000;
    now["YEAR_ENERGY"] = String(yearEnergy);
    String statusFronius = doc["Head"]["Status"]["Code"].as<String>();
    statusFroniusInt = statusFronius.toInt();
    now["Code"] = doc["Head"]["Status"]["Code"].as<String>();
    now["Reason"] = doc["Head"]["Status"]["Reason"].as<String>();
    now["UserMessage"] = doc["Head"]["Status"]["UserMessage"].as<String>();
    timeStampFronius = doc["Head"]["Timestamp"].as<String>();
    timeStampFronius = timeStampFronius.substring(0, 10) + " " + timeStampFronius.substring(11, 19);
    now["timeStamp"] = timeStampFronius;

    // END of data directly received from Fronius Converter
    Serial.println(F("Connected to Fronius Datamanager 6"));

// Check for month change
#include <month-procedure.h>

    //  Serial.print(" check twelve last i maaneden: ");
    //  Serial.println(twelveLast[1]);
    Serial.println(F("Connected to Fronius Datamanager 9"));
    // Now we set the 12 last months production
    now["Jan"] = String(twelveLast[1]);
    now["Feb"] = String(twelveLast[2]);
    now["Mar"] = String(twelveLast[3]);
    now["Apr"] = String(twelveLast[4]);
    now["Maj"] = String(twelveLast[5]);
    now["Jun"] = String(twelveLast[6]);
    now["Jul"] = String(twelveLast[7]);
    now["Aug"] = String(twelveLast[8]);
    now["Sep"] = String(twelveLast[9]);
    now["Okt"] = String(twelveLast[10]);
    now["Nov"] = String(twelveLast[11]);
    now["Dec"] = String(twelveLast[12]);
    now["lastTwelve"] = String(twelveLastTotal);
    Serial.print("last twelve: ");
    Serial.println(twelveLast[13]);
    Serial.print("twelve last total: ");
    Serial.println(twelveLastTotal);
    twelveLastTotal = 0;

    // Now we set the last years production

    // First determinate which year:
    Serial.print("YearStamp 2020 ");
    Serial.println(yearStamp);

    if (yearStamp == 2020)
    {
      Serial.print("YearStamp 2020 ");
      Serial.println(yearStamp);
      yearsLast[8] = yearEnergy;
    }
    if (yearStamp == 2021)
    {
      yearsLast[9] = yearEnergy;
    }
    if (yearStamp == 2022)
    {
      yearsLast[10] = yearEnergy;
    }
    if (yearStamp == 2023)
    {
      yearsLast[11] = yearEnergy;
    }
    if (yearStamp == 2024)
    {
      yearsLast[12] = yearEnergy;
    }
    if (yearStamp == 2025)
    {
      yearsLast[13] = yearEnergy;
    }
    Serial.println(F("Connected to Fronius Datamanager 10"));
    now["thirteen"] = String(yearsLast[1]);
    now["fourteen"] = String(yearsLast[2]);
    now["fifteen"] = String(yearsLast[3]);
    now["sixteen"] = String(yearsLast[4]);
    now["seventeen"] = String(yearsLast[5]);
    now["eightteen"] = String(yearsLast[6]);
    now["nineteen"] = String(yearsLast[7]);
    now["twenty"] = String(yearsLast[8]);
    now["twentyone"] = String(yearsLast[9]);
    now["twentytwo"] = String(yearsLast[10]);
    now["twentythree"] = String(yearsLast[11]);
    now["twentyfour"] = String(yearsLast[12]);
    now["twentyfive"] = String(yearsLast[13]);

    // Connect to mqtt broker
    //  Serial.print("Attempting to connect to the MQTT broker: ");
    //  Serial.println(broker);
    if (!mqttClient.connected())
    {
      Serial.println("You're NOT connected to the MQTT broker!");
      mqttClient.setServer(broker, 1883);
      if (!mqttClient.connect(broker, mqttuser, mqttpass))
      {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.state());
        goToDeepSleepFiveMinutes();
      }
    }
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();

    Serial.println(F("Connected to Fronius Datamanager 11"));
    // Send to mqtt
    // char buffer[1792];
    serializeJson(docSend, buffer);

    Serial.println("Sending message to topic: ");
    Serial.print(topic);
    Serial.print(" ");
    Serial.println(buffer);
    Serial.println();

    // send message, the Print interface can be used to set the message contents
    bool retained = true;

    // Check for full transmission sendt

    if (mqttClient.publish(topic, buffer, retained))
    {
      Serial.println("Message published successfully");
    }
    else
    {
      Serial.println("Error in Message, not published");
      mqttClient.disconnect();
      goToDeepSleepFiveMinutes();
    }

    Serial.println(F("Connected to Fronius Datamanager 12"));

    Serial.println(F("Disconnect the MQTT broker"));
    mqttClient.disconnect();
    Serial.println(F("Connected to Fronius Datamanager 13"));
    Serial.println(" ");
  }
}