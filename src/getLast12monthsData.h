void getLast12monthsData()
{
  // Get the last 12 months Netto data.
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

  // Get the last 12 months Upload data.
  readString = "";
  readFile(SPIFFS, "/up12.txt");
  Serial.print("readString Up 12: ");
  Serial.println(readString);

  DynamicJsonDocument last12Up(1024);

  // Parse JSON object
  DeserializationError errorU12 = deserializeJson(last12Up, readString);
  if (errorU12)
  {
    Serial.print(F("deserializeJson() Up 12 Recv failed: "));
    Serial.println(errorU12.c_str());
    goToDeepSleepFiveMinutes();
  }

  twelveUp[1] = last12Up["up12"]["Jan"].as<float>();
  twelveUp[2] = last12Up["up12"]["Feb"].as<float>();
  twelveUp[3] = last12Up["up12"]["Mar"].as<float>();
  twelveUp[4] = last12Up["up12"]["Apr"].as<float>();
  twelveUp[5] = last12Up["up12"]["Maj"].as<float>();
  twelveUp[6] = last12Up["up12"]["Jun"].as<float>();
  twelveUp[7] = last12Up["up12"]["Jul"].as<float>();
  twelveUp[8] = last12Up["up12"]["Aug"].as<float>();
  twelveUp[9] = last12Up["up12"]["Sep"].as<float>();
  twelveUp[10] = last12Up["up12"]["Okt"].as<float>();
  twelveUp[11] = last12Up["up12"]["Nov"].as<float>();
  twelveUp[12] = last12Up["up12"]["Dec"].as<float>();

  for (int i = 1; i < 13; i++)
  {
    twelveUp[13] += twelveUp[i];
    Serial.print("twelveUp(i)");
    Serial.println(twelveUp[i]);
  }
  Serial.print("twelveUp(13)");
  Serial.println(twelveUp[13]);

  // Get the last 12 months Download data.
  readString = "";
  readFile(SPIFFS, "/down12.txt");
  Serial.print("readString Down 12: ");
  Serial.println(readString);

  DynamicJsonDocument last12Down(1024);

  // Parse JSON object
  DeserializationError errorD12 = deserializeJson(last12Down, readString);
  if (errorD12)
  {
    Serial.print(F("deserializeJson() Down 12 Recv failed: "));
    Serial.println(errorD12.c_str());
    goToDeepSleepFiveMinutes();
  }

  twelveDown[1] = last12Down["down12"]["Jan"].as<float>();
  twelveDown[2] = last12Down["down12"]["Feb"].as<float>();
  twelveDown[3] = last12Down["down12"]["Mar"].as<float>();
  twelveDown[4] = last12Down["down12"]["Apr"].as<float>();
  twelveDown[5] = last12Down["down12"]["Maj"].as<float>();
  twelveDown[6] = last12Down["down12"]["Jun"].as<float>();
  twelveDown[7] = last12Down["down12"]["Jul"].as<float>();
  twelveDown[8] = last12Down["down12"]["Aug"].as<float>();
  twelveDown[9] = last12Down["down12"]["Sep"].as<float>();
  twelveDown[10] = last12Down["down12"]["Okt"].as<float>();
  twelveDown[11] = last12Down["down12"]["Nov"].as<float>();
  twelveDown[12] = last12Down["down12"]["Dec"].as<float>();

  for (int i = 1; i < 13; i++)
  {
    twelveDown[13] += twelveDown[i];
    Serial.print("twelveDown(i)");
    Serial.println(twelveDown[i]);
  }
  Serial.print("twelveDown(13)");
  Serial.println(twelveDown[13]);



}