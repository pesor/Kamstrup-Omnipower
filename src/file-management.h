// Subroutines file management
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