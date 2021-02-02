timeClient.setTimeOffset(gmtOffset_sec);
while (!timeClient.update())
{
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
//  Serial.print("dateMonth: ");
//  Serial.println(dateMonth);
//  Serial.print("dateDay: ");
//  Serial.println(dateDay);
dayStamp = dateDay + "-" + dateMonth;
//  Serial.print("dayStamp ");
//  Serial.println(dayStamp);
//  config.date = dayStamp;
// Extract time
timeStamp1 = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
//  Serial.print("timeStamp1 ");
//  Serial.println(timeStamp1);
//  config.time = timeStamp1.substring(0, 5);
//  Serial.print("Day ");
//  Serial.println(timeClient.getDay());
// variables needed for DST test
int thisDay = dateDay.toInt();
thisHour = timeClient.getHours();
thisMinute = timeClient.getMinutes();
thisSecond = timeClient.getSeconds();
thisDayStr = dateDay;
thisMonthStr = dateMonth;
thisMonth = dateMonth.toInt();
//  int thisMonth = 2; // Test purposes
int thisWeekday = timeClient.getDay();
bool dst = false;

// Test for DST active
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

if (thisMonth < 11 && thisMonth > 3)
{
    Serial.println("3");
    dst = true;
}

if (thisMonth == 3 && thisDay > 7 && thisDay >= (thisWeekday + 7))
{
    Serial.println("4");
    if (!(thisWeekday == 1 && thisHour < 2))
    {
        Serial.println("5");
        dst = true;
    }
}

if (dst)
{
    Serial.println("IN SOMMERTIME");
    timeClient.setTimeOffset(gmtOffset_sec + 3600);
    while (!timeClient.update())
    {
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
