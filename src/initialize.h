void Initialize()
{
  //When initialising the program, it is important to set the current months data (lastMonth = current month) after initialisation, all initialisation variables have to be commented out.

  // writeFile(SPIFFS, "/lastDay.txt", "27");
  // writeFile(SPIFFS, "/lastMonth.txt", "10");

  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt", "7037.65");
  //  writeFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt", "7815,72");

  char Netto12Months[] = R"raw({"netto12":{"Jan":"0","Feb":"0","Mar":"107.84","Apr":"-242.80","Maj":"-311.25","Jun":"-309.09","Jul":"-154.08","Aug":"-11.45","Sep":"30.95","Okt":"236.10","Nov":"452","Dec":"647"}})raw";
  writeFile(SPIFFS, "/last12.txt", Netto12Months);

  //  char AllYears[] = R"raw({"lastYears":{"2013":"5160","2014":"5668","2015":"5830","2016":"5820","2017":"5410","2018":"6150","2019":"5790","2020":"5440","2021":"0","2022":"0","2023":"0","2024":"0","2025":"0"}})raw";
  //  writeFile(SPIFFS, "/allYears.txt", AllYears);

  Serial.println("DID YOU REMEMBER TO UNCOMMENT / COMMENT THE VARIABLES YOU WANT TO USE / NOT USE ---- IMPORTANT!!!!!!!!!!!");

  // END Initialisation
}
