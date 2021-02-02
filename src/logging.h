// Logging management
if (logging)
{
  writeFile(SPIFFS, "/error.log", "Void Setup \n");
}

listDir(SPIFFS, "/", 0);

if (logging)
{
  writeFile(SPIFFS, "/error.log", "After listDir \n");
}

if (readLogfile)
{
  // Now we start reading the files..
  readFile(SPIFFS, "/error.log");
  Serial.println("Here comes the logging info:");
  Serial.println(readString);
}

if (deleteLogfile)
{
  SPIFFS.remove("/error.log");
}