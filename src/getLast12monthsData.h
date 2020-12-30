void getLast12monthsData()
{
  // Get the last 12 months data, excl current month, which is not included.
  readString = "";
  readFile(SPIFFS, "/last12.txt");
  Serial.print("readString last 12: ");
  Serial.println(readString);

  DynamicJsonDocument last12Netto(1024);

  // Parse JSON object
  DeserializationError errorL12 = deserializeJson(last12Netto, readString);
  if (errorL12)
  {
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

  for (int i = 1; i < 13; i++)
  {
    twelveNetto[13] += twelveNetto[i];
    Serial.print("twelveNetto(i)");
    Serial.println(twelveNetto[i]);
  }
  Serial.print("twelveNetto(13)");
  Serial.println(twelveNetto[13]);
}
