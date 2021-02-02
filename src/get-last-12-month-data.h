void getLast12monthsData()
{
  // Get the last 12 months data, excl current month, which is not included.
  readString = "";
  readFile(SPIFFS, "/last12.txt");
  Serial.print("readString last 12: ");
  Serial.println(readString);

  DynamicJsonDocument last12Recv(1024);

  // Parse JSON object
  DeserializationError errorL12 = deserializeJson(last12Recv, readString);
  if (errorL12)
  {
    Serial.print(F("deserializeJson() Last 12 Recv failed: "));
    Serial.println(errorL12.c_str());
    goToDeepSleepFiveMinutes();
  }

  twelveLast[1] = last12Recv["last12"]["Jan"].as<float>();
  twelveLast[2] = last12Recv["last12"]["Feb"].as<float>();
  twelveLast[3] = last12Recv["last12"]["Mar"].as<float>();
  twelveLast[4] = last12Recv["last12"]["Apr"].as<float>();
  twelveLast[5] = last12Recv["last12"]["Maj"].as<float>();
  twelveLast[6] = last12Recv["last12"]["Jun"].as<float>();
  twelveLast[7] = last12Recv["last12"]["Jul"].as<float>();
  twelveLast[8] = last12Recv["last12"]["Aug"].as<float>();
  twelveLast[9] = last12Recv["last12"]["Sep"].as<float>();
  twelveLast[10] = last12Recv["last12"]["Okt"].as<float>();
  twelveLast[11] = last12Recv["last12"]["Nov"].as<float>();
  twelveLast[12] = last12Recv["last12"]["Dec"].as<float>();
  //  twelveLast[13] = last12Recv["LastTwelve"].as<float>();

  twelveLast[0] = 0;
  twelveLast[13] = 0;
  twelveLastTotal = 0;
  for (int i = 1; i < 13; i++)
  {
    twelveLastTotal += twelveLast[i];
    Serial.print("twelveLast(i)");
    Serial.println(twelveLast[i]);
  }
      twelveLast[13] = twelveLastTotal;
}
// END getLast12monthsData()