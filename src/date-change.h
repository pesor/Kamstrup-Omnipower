// FTP to backup every day
ftp.OpenConnection();
//Change directory
ftp.ChangeWorkDir("/docker/kamstrup");
//upload the text file or read it
String response = "";
ftp.InitFile("Type A");
String ftpFilename = String(formattedDate) + ": kamstrup_mqtt.json";
const char *ftpFilenameChar = ftpFilename.c_str();
ftp.NewFile(ftpFilenameChar);
ftp.Write(buffer);

ftp.CloseConnection();