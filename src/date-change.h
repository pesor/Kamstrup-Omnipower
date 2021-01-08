// FTP to backup every day
ftp.OpenConnection();
//Change directory
ftp.ChangeWorkDir("/docker/fronius");
//upload the text file or read it
String response = "";
ftp.InitFile("Type A");
String ftpFilename = timeStampKamstrup + ": kamstrup_mqtt.json";
const char *ftpFilenameChar = ftpFilename.c_str();
ftp.NewFile(ftpFilenameChar);
ftp.Write(buffer);

ftp.CloseConnection();

writeFile(SPIFFS, "/lastDay.txt", thisDayStr.c_str());
lastDayStr = thisDayStr;