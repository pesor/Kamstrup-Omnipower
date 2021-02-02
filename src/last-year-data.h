void getLastYearsData() {

  // Get the last years data, excl current year, which is not included.
  readString = "";
  readFile(SPIFFS, "/allYears.txt");
  Serial.print("readString all years: ");
  Serial.println(readString);

  DynamicJsonDocument lastYears(1024);

  // Parse JSON object
  DeserializationError errorYear = deserializeJson(lastYears, readString);
  if (errorYear) {
    Serial.print(F("deserializeJson() Last years production failed read: "));
    Serial.println(errorYear.c_str());
    goToDeepSleepFiveMinutes();
  }
  yearsLast[1] = lastYears["lastYears"]["2013"].as<float>();
  yearsLast[2] = lastYears["lastYears"]["2014"].as<float>();
  yearsLast[3] = lastYears["lastYears"]["2015"].as<float>();
  yearsLast[4] = lastYears["lastYears"]["2016"].as<float>();
  yearsLast[5] = lastYears["lastYears"]["2017"].as<float>();
  yearsLast[6] = lastYears["lastYears"]["2018"].as<float>();
  yearsLast[7] = lastYears["lastYears"]["2019"].as<float>();
  yearsLast[8] = lastYears["lastYears"]["2020"].as<float>();
  yearsLast[9] = lastYears["lastYears"]["2021"].as<float>();
  yearsLast[10] = lastYears["lastYears"]["2022"].as<float>();
  yearsLast[11] = lastYears["lastYears"]["2023"].as<float>();
  yearsLast[12] = lastYears["lastYears"]["2024"].as<float>();
  yearsLast[13] = lastYears["lastYears"]["2025"].as<float>();

}
// END getLastYearsData()
