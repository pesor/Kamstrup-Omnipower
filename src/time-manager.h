  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  yearStamp = dayStamp.substring(0, 4).toInt();
  Serial.print("YearStamp set ");
  Serial.println(yearStamp);
  dayStamp = dayStamp.substring(5);
  String dateMonth = dayStamp.substring(0, 2);
  String dateDay = dayStamp.substring(3, 5);
  Serial.print("dateMonth: ");
  Serial.println(dateMonth);
  Serial.print("dateDay: ");
  Serial.println(dateDay);
  dayStamp = dateDay + "-" + dateMonth;
  Serial.print("dayStamp ");
  Serial.println(dayStamp);
  //  config.date = dayStamp;
  // Extract time
  timeStamp1 = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
  Serial.print("timeStamp1 ");
  Serial.println(timeStamp1);
  //  config.time = timeStamp1.substring(0, 5);
  Serial.print("Day ");
  Serial.println(timeClient.getDay());
  // variables needed for DST test
  thisHour = timeClient.getHours();
  thisMinute = timeClient.getMinutes();
  thisSecond = timeClient.getSeconds();
  thisDay = dateDay.toInt();
  int thisMonth = dateMonth.toInt();
  //  int thisMonth = 2; // Test purposes
  int thisWeekday = timeClient.getDay();
  bool dst = false;

  // Test for DST active
  if (thisMonth == 11 && thisDay < 8 && thisDay < thisWeekday)
  {
    Serial.println("1");
    dst = true;
  }

  if (thisMonth == 11 && thisDay < 8 && thisWeekday == 1 && thisHour < 1)
  {
    Serial.println("2");
    dst = true;
  }

  if (thisMonth < 11 && thisMonth > 3) {
    Serial.println("3");
    dst = true;
  }

  if (thisMonth == 3 && thisDay > 7 && thisDay >= (thisWeekday + 7))
  {
    Serial.println("4");
    if (!(thisWeekday == 1 && thisHour < 2)) {
      Serial.println("5");
      dst = true;
    }
  }

  if (dst) {
    Serial.println("IN SOMMERTIME");
    timeClient.setTimeOffset(gmtOffset_sec + 3600);
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
    // The formattedDate comes with the following format:
    // 2018-05-28T16:00:13Z
    // We need to extract date and time
    formattedDate = timeClient.getFormattedDate();
    // Extract date
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    dayStamp = dayStamp.substring(5);
    String dateMonth = dayStamp.substring(0, 2);
    String dateDay = dayStamp.substring(3, 5);
    Serial.print("dateMonth: ");
    Serial.println(dateMonth);
    Serial.print("dateDay: ");
    Serial.println(dateDay);
    dayStamp = dateDay + "-" + dateMonth;
    Serial.print("dayStamp ");
    Serial.println(dayStamp);
    //  config.date = dayStamp;
    // Extract time
    timeStamp1 = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
    Serial.print("timeStamp1 ");
    Serial.println(timeStamp1);
    //  config.time = timeStamp1.substring(0, 5);
    Serial.print("Day ");
    Serial.println(timeClient.getDay());
  }
    static int currentDay;
    static int currentMonth;
    int sunrise;
    int sunset;
    /*
    // Not used in Fronius module
    double civilsunrise;
    double civilsunset;
    double astrosunrise;
    double astrosunset;
    double nauticalsunrise;
    double nauticalsunset;
    */

//    if (currentDay != day()) {
        sun.setCurrentDate(year(), month(), day());
        currentDay = day();
        currentMonth = month();
//    }
    sunrise = static_cast<int>(sun.calcSunrise());
    sunset = static_cast<int>(sun.calcSunset());
    /*
    // Not used in Fronius module
    civilsunrise = sun.calcCivilSunrise();
    civilsunset = sun.calcCivilSunset();
    nauticalsunrise = sun.calcNauticalSunrise();
    nauticalsunset = sun.calcNauticalSunset();
    astrosunrise = sun.calcAstronomicalSunrise();
    astrosunset = sun.calcAstronomicalSunset();
    */
    // Correct sunset hour if summertime.
    sunset_hour = (sunset/60);
    sunrise_hour = (sunrise/60);
    int sunset_minute = twoDigits(sunrise%60).toInt();
    if (dst) {
      sunset_hour += 1;
    }
    Serial.print("DST : ");
    Serial.print(sunset_hour);
    Serial.print(":");
    Serial.println(sunset_minute);

    Serial.print("Current Day ");
    Serial.print(currentDay);
    Serial.print(" Current Month ");
    Serial.println(currentMonth);
    Serial.print(" Sunrise at ");
    Serial.print(sunrise/60);
    Serial.print(":");
    Serial.print(twoDigits(sunrise%60));
    Serial.print(" Sunset at ");
    Serial.print(sunset/60);
    Serial.print(":");
    Serial.print(twoDigits(sunset%60));
    Serial.println();
    Serial.print(" Sunrise full ");
    Serial.print(sunrise);
    Serial.print(" Sunset full ");
    Serial.println(sunset);
    Serial.println(" seconds to sunrise");
    int secondsToSunRise = (24 - sunset_hour + sunrise_hour) * 60 * 60;
    Serial.println(secondsToSunRise);