// Netto consumption
float NettoSum = 0.00;

for (int i = 1; i < 13; i++)
{
    NettoSum += twelveNetto[i];
}

output["nettoJan"] = String(twelveNetto[1]);
output["nettoFeb"] = String(twelveNetto[2]);
output["nettoMar"] = String(twelveNetto[3]);
output["nettoApr"] = String(twelveNetto[4]);
output["nettoMaj"] = String(twelveNetto[5]);
output["nettoJun"] = String(twelveNetto[6]);
output["nettoJul"] = String(twelveNetto[7]);
output["nettoAug"] = String(twelveNetto[8]);
output["nettoSep"] = String(twelveNetto[9]);
output["nettoOkt"] = String(twelveNetto[10]);
output["nettoNov"] = String(twelveNetto[11]);
output["nettoDec"] = String(twelveNetto[12]);
output["netto12"] = String(NettoSum);

// if day change, we save the 12 last months netto
if (thisDayStr != lastDayStr)
{
    Serial.println("Date change in effect");
    
    File f12 = SPIFFS.open("/last12.txt", "w");

    StaticJsonDocument<512> doc12;
    JsonObject root = doc12.to<JsonObject>();
    JsonObject netto12 = root.createNestedObject("netto12");

    // Now we set the 12 last months production
    netto12["Jan"] = String(twelveNetto[1]);
    netto12["Feb"] = String(twelveNetto[2]);
    netto12["Mar"] = String(twelveNetto[3]);
    netto12["Apr"] = String(twelveNetto[4]);
    netto12["Maj"] = String(twelveNetto[5]);
    netto12["Jun"] = String(twelveNetto[6]);
    netto12["Jul"] = String(twelveNetto[7]);
    netto12["Aug"] = String(twelveNetto[8]);
    netto12["Sep"] = String(twelveNetto[9]);
    netto12["Okt"] = String(twelveNetto[10]);
    netto12["Nov"] = String(twelveNetto[11]);
    netto12["Dec"] = String(twelveNetto[12]);
    netto12["netto12"] = String(NettoSum);

    // Serialize JSON to file
    if (serializeJson(doc12, f12) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }

    f12.close();
}

// Upload
float UpSum = 0.00;

for (int i = 1; i < 13; i++)
{
    UpSum += twelveUp[i];
}

output["upJan"] = String(twelveUp[1]);
output["upFeb"] = String(twelveUp[2]);
output["upMar"] = String(twelveUp[3]);
output["upApr"] = String(twelveUp[4]);
output["upMaj"] = String(twelveUp[5]);
output["upJun"] = String(twelveUp[6]);
output["upJul"] = String(twelveUp[7]);
output["upAug"] = String(twelveUp[8]);
output["upSep"] = String(twelveUp[9]);
output["upOkt"] = String(twelveUp[10]);
output["upNov"] = String(twelveUp[11]);
output["upDec"] = String(twelveUp[12]);
output["up12"] = String(UpSum);

// if day change, we save the 12 last months Upload
if (thisDayStr != lastDayStr)
{
    Serial.println("Date change in effect");
    
    File fu12 = SPIFFS.open("/up12.txt", "w");

    StaticJsonDocument<512> docUp12;
    JsonObject root = docUp12.to<JsonObject>();
    JsonObject up12 = root.createNestedObject("up12");

    // Now we set the 12 last months production
    up12["Jan"] = String(twelveUp[1]);
    up12["Feb"] = String(twelveUp[2]);
    up12["Mar"] = String(twelveUp[3]);
    up12["Apr"] = String(twelveUp[4]);
    up12["Maj"] = String(twelveUp[5]);
    up12["Jun"] = String(twelveUp[6]);
    up12["Jul"] = String(twelveUp[7]);
    up12["Aug"] = String(twelveUp[8]);
    up12["Sep"] = String(twelveUp[9]);
    up12["Okt"] = String(twelveUp[10]);
    up12["Nov"] = String(twelveUp[11]);
    up12["Dec"] = String(twelveUp[12]);
    up12["up12"] = String(UpSum);

    // Serialize JSON to file
    if (serializeJson(docUp12, fu12) == 0)
    {
        Serial.println(F("Failed to write upload to file"));
    }

    fu12.close();
}


// Download
float DownSum = 0.00;

for (int i = 1; i < 13; i++)
{
    DownSum += twelveDown[i];
}

output["downJan"] = String(twelveDown[1]);
output["downFeb"] = String(twelveDown[2]);
output["downMar"] = String(twelveDown[3]);
output["downApr"] = String(twelveDown[4]);
output["downMaj"] = String(twelveDown[5]);
output["downJun"] = String(twelveDown[6]);
output["downJul"] = String(twelveDown[7]);
output["downAug"] = String(twelveDown[8]);
output["downSep"] = String(twelveDown[9]);
output["downOkt"] = String(twelveDown[10]);
output["downNov"] = String(twelveDown[11]);
output["downDec"] = String(twelveDown[12]);
output["down12"] = String(DownSum);

// if day change, we save the 12 last months Upload
if (thisDayStr != lastDayStr)
{
    Serial.println("Date change in effect");
    
    File fd12 = SPIFFS.open("/down12.txt", "w");

    StaticJsonDocument<512> docDown12;
    JsonObject root = docDown12.to<JsonObject>();
    JsonObject down12 = root.createNestedObject("down12");

    // Now we set the 12 last months production
    down12["Jan"] = String(twelveDown[1]);
    down12["Feb"] = String(twelveDown[2]);
    down12["Mar"] = String(twelveDown[3]);
    down12["Apr"] = String(twelveDown[4]);
    down12["Maj"] = String(twelveDown[5]);
    down12["Jun"] = String(twelveDown[6]);
    down12["Jul"] = String(twelveDown[7]);
    down12["Aug"] = String(twelveDown[8]);
    down12["Sep"] = String(twelveDown[9]);
    down12["Okt"] = String(twelveDown[10]);
    down12["Nov"] = String(twelveDown[11]);
    down12["Dec"] = String(twelveDown[12]);
    down12["up12"] = String(UpSum);

    // Serialize JSON to file
    if (serializeJson(docDown12, fd12) == 0)
    {
        Serial.println(F("Failed to write  download to file"));
    }

    fd12.close();
}