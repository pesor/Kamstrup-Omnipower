// Read the Fronius daily output, which on the Data Manager is FTP'ed to your FTP Server.
void getFTPmessage() {
  producedNow = 0;

  ftp.OpenConnection();

  //Change directory
  ftp.ChangeWorkDir("/docker/fronius");
  //Download the text file or read it
  String response = "";
  ftp.InitFile("Type A");
  ftp.DownloadString("fronius_push.json", response);
  Serial.println("The file content is: " + response);

  // Get the file size
  String       list[128];

  ftp.CloseConnection();

  DynamicJsonDocument docFTP(12544);

  // Parse JSON object
  DeserializationError errorFTP = deserializeJson(docFTP, response);
  if (errorFTP) {
    Serial.print(F("deserializeJson() FTP failed: "));
    Serial.println(errorFTP.c_str());
    goToDeepSleepFiveMinutes();
  }

  String producedToday = docFTP["Body"]["inverter/1"]["Data"]["EnergyReal_WAC_Sum_Produced"]["Values"].as<String>();
  // Serial.println(producedToday);
  // Define number of pieces
  int counter = 0;
  int lastIndex = 0;
  String partSum = "";
  for (int i = 0; i < producedToday.length(); i++) {
    // Loop through each character and check if it's a comma
    if (producedToday.substring(i, i + 1) == ",") {
      // Grab the piece from the last index up to the current position and store it
      partSum = producedToday.substring(lastIndex, i);
      if (i == 10) {
        // Serial.println(partSum);
        producedNow = producedNow + partSum.substring(9).toFloat();
      } else {
        producedNow = producedNow + partSum.substring(8).toFloat();
      }
      // Serial.println(producedNow);

      // Update the last position and add 1, so it starts from the next character
      lastIndex = i + 1;
      // Increase the position in the array that we store into
      counter++;
    }

    // If we're at the end of the string (no more commas to stop us)
    if (i == producedToday.length() - 1) {
      // Grab the last part of the string from the lastIndex to the end
      partSum = producedToday.substring(lastIndex, i);
      producedNow = producedNow + partSum.substring(8).toFloat();
      //        Serial.println(producedNow);
    }
  }
}
// END getFTPmessage()
