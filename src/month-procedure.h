// *************************************
// Now we have to check for month change
// *************************************

if (firstRun)
{
  firstRun = false;
}
else
{
  Serial.println("Sådan ser buffer ud: ");
  Serial.println(buffer);
  DeserializationError errorbuf = deserializeJson(docSave, buffer);
  if (errorbuf)
  {
    Serial.print(F("Month deserializeJson() buffer failed: "));
    Serial.println(error.c_str());
    if (logging)
    {
      String logInfo = formattedDate + " deserializeJson buffer failed - sleep 5 minutes: \n" + String(buffer) + " \n";
      const char *logInfo_write = logInfo.c_str();
      writeFile(SPIFFS, "/error.log", logInfo_write);
    }

    goToDeepSleepFiveMinutes();
  }
  else
  {
    lastDayOfMonth = false;
    Serial.println("Serialization of buffer successfull");
    // Now we check for month changing, in order to save data accordingly.
    thisMonthInMonth = timeStampFronius.substring(5, 7).toInt();
    thisDayInMonth = timeStampFronius.substring(8, 10).toInt();
    thisHourInMonth = timeStampFronius.substring(11, 13).toInt();
    thisMinuteInMonth = timeStampFronius.substring(14, 16).toInt();
    Serial.println("thisDayInMonth ");
    Serial.println(thisDayInMonth);
    Serial.println("thisMonthInMonth ");
    Serial.println(thisMonthInMonth);

    // Find if we are last day of month
    if (thisDayInMonth == 28)
    {
      if (thisMonthInMonth == 2)
      {
        if (thisYearInMonth == 2024 || thisYearInMonth == 2028 || thisYearInMonth == 2032 || thisYearInMonth == 2036 || thisYearInMonth == 2040)
        {
          Serial.println("Leap Year, wait until tomorrow");
        }
        else
        {
          lastDayOfMonth = true;
        }
      }
    }
    if (thisDayInMonth == 29)
    {
      if (thisMonthInMonth == 2)
      {
        if (thisYearInMonth == 2024 || thisYearInMonth == 2028 || thisYearInMonth == 2032 || thisYearInMonth == 2036 || thisYearInMonth == 2040)
        {
          lastDayOfMonth = true;
        }
      }
    }
    if (thisDayInMonth == 30)
    {
      if (thisMonthInMonth == 4 || thisMonthInMonth == 6 || thisMonthInMonth == 9 || thisMonthInMonth == 11)
      {
        lastDayOfMonth = true;
      }
    }
    if (thisDayInMonth == 31)
    {
      if (thisMonthInMonth == 1 || thisMonthInMonth == 3 || thisMonthInMonth == 5 || thisMonthInMonth == 7 || thisMonthInMonth == 8 || thisMonthInMonth == 10 || thisMonthInMonth == 12)
      {
        lastDayOfMonth = true;
      }
    }

    // Are we after SunSet
    if (lastDayOfMonth)
    {
      if (logging)
      {
        String logInfo = formattedDate + " We are in day of last month: \n";
        const char *logInfo_write = logInfo.c_str();
        writeFile(SPIFFS, "/error.log", logInfo_write);
      }
      if (thisHourInMonth == sunset_hour)
      {
        Serial.println("sunset hour");
        Serial.println(sunset_hour);

        Serial.println("Month is changing");
        if (logging)
        {
          String logInfo = formattedDate + " We are in sunset hour of last day of month: \n";
          const char *logInfo_write = logInfo.c_str();
          writeFile(SPIFFS, "/error.log", logInfo_write);
        }
        lastDayInt = String(thisDay);
        writeFile(SPIFFS, "/lastDay.txt", lastDayInt.c_str());

#include <ftp-buffer.h>
        getFTPmessage();
        // Set the values in the document from the last saved read.
        // Device changes according to device placement
        now["PAC"] = docSave["Body"]["Data"]["PAC"]["Value"].as<String>();
        now["FAC"] = docSave["Body"]["Data"]["FAC"]["Value"].as<String>();
        now["IAC"] = docSave["Body"]["Data"]["IAC"]["Value"].as<String>();
        now["IDC"] = docSave["Body"]["Data"]["IDC"]["Value"].as<String>();
        float totalEnergy = docSave["Body"]["Data"]["TOTAL_ENERGY"]["Value"].as<float>() / 1000;
        now["TOTAL_ENERGY"] = String(totalEnergy);
        now["UAC"] = docSave["Body"]["Data"]["UAC"]["Value"].as<String>();
        now["UDC"] = docSave["Body"]["Data"]["UDC"]["Value"].as<String>();
        now["Day_Energy"] = String(producedNow / 1000);
        yearEnergy = docSave["Body"]["Data"]["YEAR_ENERGY"]["Value"].as<float>() / 1000;
        now["YEAR_ENERGY"] = String(yearEnergy);
        String statusFronius = docSave["Head"]["Status"]["Code"].as<String>();
        statusFroniusInt = statusFronius.toInt();
        now["Code"] = docSave["Head"]["Status"]["Code"].as<String>();
        now["Reason"] = docSave["Head"]["Status"]["Reason"].as<String>();
        now["UserMessage"] = docSave["Head"]["Status"]["UserMessage"].as<String>();
        timeStampFronius = docSave["Head"]["Timestamp"].as<String>();
        timeStampFronius = timeStampFronius.substring(0, 10) + " " + timeStampFronius.substring(11, 19);
        now["timeStamp"] = timeStampFronius;

        // we need to move this month energy to the appropiate lastTwelve variable
        int lastMonthIntInt = lastMonthInt.toInt();
        twelveLast[lastMonthIntInt] = yearEnergy - yearToLastMonthEnergy - correctManualStart + (producedNow / 1000);
        if (logging)
        {
          String logInfo = formattedDate + " Sidste måneds produktion: " + String(twelveLast[lastMonthIntInt]) + " \n";
          const char *logInfo_write = logInfo.c_str();
          writeFile(SPIFFS, "/error.log", logInfo_write);
        }
        Serial.print("Sidste måneds forbrug");
        Serial.println(twelveLast[lastMonthIntInt]);
        // update the yearToLastMonthEnergy to yearEnergy
        yearToLastMonthEnergy = yearEnergy - correctManualStart + (producedNow / 1000);
        if (logging)
        {
          String logInfo = formattedDate + " Energi til sidste måned: " + String(yearToLastMonthEnergy) + " \n" +
                           formattedDate + " beregnet således: yearEnergy " + yearEnergy + " correktion " + correctManualStart +
                           " produceret i dag " + producedNow / 1000 + "\n";
          const char *logInfo_write = logInfo.c_str();
          writeFile(SPIFFS, "/error.log", logInfo_write);
        }
        Serial.print("Forbrug til månedsskift");
        Serial.println(yearToLastMonthEnergy);
        writeFile(SPIFFS, "/yearToLastMonthEnergy.txt", String(yearToLastMonthEnergy).c_str());
        float twelveLastTotalLocal = 0;

        for (int i = 1; i < 13; i++)
        {
          twelveLastTotalLocal += twelveLast[i];
          Serial.print("Last twelve months total: ");
          Serial.println(twelveLastTotalLocal);
        }
        // We move this integer to lastMonthInt, in order to maintain check for month change.
        lastMonthInt = thisMonthInt + 1;
        if (lastMonthInt == "13")
        {
          lastMonthInt = "01";
        }
        writeFile(SPIFFS, "/lastMonth.txt", lastMonthInt.c_str());
        Serial.println(F("Connected to Fronius Datamanager 7"));
        // NEW NEW NEW NEW NEW

        File f12 = SPIFFS.open("/last12.txt", "w");

        StaticJsonDocument<512> doc12;
        JsonObject root = doc12.to<JsonObject>();

        JsonObject last12 = root.createNestedObject("last12");

        // Now we set the 12 last months production
        last12["Jan"] = String(twelveLast[1]);
        last12["Feb"] = String(twelveLast[2]);
        last12["Mar"] = String(twelveLast[3]);
        last12["Apr"] = String(twelveLast[4]);
        last12["Maj"] = String(twelveLast[5]);
        last12["Jun"] = String(twelveLast[6]);
        last12["Jul"] = String(twelveLast[7]);
        last12["Aug"] = String(twelveLast[8]);
        last12["Sep"] = String(twelveLast[9]);
        last12["Okt"] = String(twelveLast[10]);
        last12["Nov"] = String(twelveLast[11]);
        last12["Dec"] = String(twelveLast[12]);
        last12["lastTwelve"] = String(twelveLastTotalLocal);

        // Serialize JSON to file
        if (serializeJson(doc12, f12) == 0)
        {
          Serial.println(F("Failed to write to file"));
        }

        f12.close();
        Serial.println(F("Connected to Fronius Datamanager 8"));

        // END END END END END

        // We need to set the variables right.

        thisMonthsEnergy = 0;

        now["MONTH_ENERGY"] = String(thisMonthsEnergy);

        // Here we need to go to sleep until sunrise, so the values do not screw up
        int secondsToSunRise = (24 - sunset_hour + sunrise_hour) * 60 * 60;
        if (logging)
        {
          String logInfo = formattedDate + " Going to Sleep for: " + String(secondsToSunRise) + " until dawn. \n";
          const char *logInfo_write = logInfo.c_str();
          writeFile(SPIFFS, "/error.log", logInfo_write);
        }

        Serial.print("Going to sleep... ");
        Serial.print(secondsToSunRise);
        Serial.println(" sekunder");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        btStop();

        adc_power_off();
        esp_wifi_stop();
        esp_bt_controller_disable();

        // Configure the timer to wake us up!

        esp_sleep_enable_timer_wakeup(secondsToSunRise * uS_TO_S_FACTOR);

        // Go to sleep! Zzzz
        esp_deep_sleep_start();
      }
    }
  }
}

// Så skal vi beregne månedsforbruget
Serial.println("Vi er i maaneden");
if (statusFroniusInt == 0)
{
  thisMonthsEnergy = yearEnergy - yearToLastMonthEnergy + (producedNow / 1000);
  Serial.print("this months energy: ");
  Serial.println(thisMonthsEnergy);
  Serial.print("this year energy: ");
  Serial.println(yearEnergy);
  Serial.print("year to last month energy: ");
  Serial.println(yearToLastMonthEnergy);
  now["MONTH_ENERGY"] = String(thisMonthsEnergy);
}