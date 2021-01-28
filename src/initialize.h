void Initialize()
{
  //When initialising the program, it is important to set the current months data (lastMonth = current month) after initialisation, all initialisation variables have to be commented out.

  // writeFile(SPIFFS, "/lastDay.txt", "27");
  // writeFile(SPIFFS, "/lastMonth.txt", "10");

  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt", "7037.65");
  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt", "7815,72");

  // char Netto12Months[] = R"raw({"netto12":{"Jan":"0","Feb":"0","Mar":"107.84","Apr":"-242.80","Maj":"-311.25","Jun":"-309.09","Jul":"-154.08","Aug":"-11.45","Sep":"30.95","Okt":"236.10","Nov":"452","Dec":"647"}})raw";
  // writeFile(SPIFFS, "/last12.txt", Netto12Months);

  // char up12Months[] = R"raw({"up12":{"Jan":"0","Feb":"0","Mar":"334.40","Apr":"548.00","Maj":"631.40","Jun":"581.30","Jul":"427.40","Aug":"384.10","Sep":"304.20","Okt":"169.60","Nov":"46.52","Dec":"3.07"}})raw";
  //  writeFile(SPIFFS, "/up12.txt", up12Months);
 
  //char down12Months[] = R"raw({"down12":{"Jan":"0","Feb":"0","Mar":"442.26","Apr":"305.20","Maj":"320.15","Jun":"272.21","Jul":"273.36","Aug":"372.65","Sep":"335.15","Okt":"405.70","Nov":"498.25","Dec":"647.23"}})raw";
  //writeFile(SPIFFS, "/down12.txt", down12Months);

  Serial.println("DID YOU REMEMBER TO UNCOMMENT / COMMENT THE VARIABLES YOU WANT TO USE / NOT USE ---- IMPORTANT!!!!!!!!!!!");

  // END Initialisation
}
