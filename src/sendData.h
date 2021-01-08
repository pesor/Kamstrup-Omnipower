void sendData(MeterData md)
{
  // Send the MeterData to MQTT

  Serial.println(thisMonthStr);
  Serial.println(lastMonthStr);
  // if month change, we need to update the netto for the old month.
  if (thisMonthStr != lastMonthStr)
  {

    int lastMonthArr = lastMonthStr.toInt();
    twelveNetto[lastMonthArr] = nettoMonth / 1000;

    Serial.println(lastMonthArr);
    writeFile(SPIFFS, "/lastMonth.txt", thisMonthStr.c_str());
    lastMonthStr = thisMonthStr;

    String MeterEnergyTotExpNowStr = String(MeterEnergyTotExpNow);
    String MeterEnergyTotImpNowStr = String(MeterEnergyTotImpNow);
    writeFile(SPIFFS, "/MeterToLastMonthEnergyExp.txt", MeterEnergyTotExpNowStr.c_str());
    writeFile(SPIFFS, "/MeterToLastMonthEnergyImp.txt", MeterEnergyTotImpNowStr.c_str());
    MeterToLastMonthEnergyImp = MeterEnergyTotImpNow;
    MeterToLastMonthEnergyExp = MeterEnergyTotExpNow;
  }

  // Allocate a  JsonDocument
  StaticJsonDocument<1792> docSend;
  JsonObject root = docSend.to<JsonObject>();

  JsonObject output = root.createNestedObject("output");

  if (md.activePowerPlusValid)
  {
    // Serial.println( "/power/activePowerPlus " + String(md.activePowerPlus));
    output["activePowerPlus"] = String(md.activePowerPlus);
  }
  if (md.activePowerMinusValid)
  {
    // Serial.println(  "/power/activePowerMinus " + String(md.activePowerMinus));
    output["activePowerMinus"] = String(md.activePowerMinus);
  }
  if (md.activePowerPlusValidL1)
  {
    // Serial.println(  "/power/activePowerPlusL1 " + String(md.activePowerPlusL1));
    output["activePowerPlusL1"] = String(md.activePowerPlusL1);
  }
  if (md.activePowerMinusValidL1)
  {
    // Serial.println(  "/power/activePowerMinusL1 " + String(md.activePowerMinusL1));
    output["activePowerMinusL1"] = String(md.activePowerMinusL1);
  }
  if (md.activePowerPlusValidL2)
  {
    // Serial.println(  "/power/activePowerPlusL2 " + String(md.activePowerPlusL2));
    output["activePowerPlusL2"] = String(md.activePowerPlusL2);
  }
  if (md.activePowerMinusValidL2)
  {
    // Serial.println(  "/power/activePowerMinusL2 " + String(md.activePowerMinusL2));
    output["activePowerMinusL2"] = String(md.activePowerMinusL2);
  }
  if (md.activePowerPlusValidL3)
  {
    // Serial.println(  "/power/activePowerPlusL3 " + String(md.activePowerPlusL3));
    output["activePowerPlusL3"] = String(md.activePowerPlusL3);
  }
  if (md.activePowerMinusValidL3)
  {
    // Serial.println(  "/power/activePowerMinusL3 " + String(md.activePowerMinusL3));
    output["activePowerMinusL3"] = String(md.activePowerMinusL3);
  }
  if (md.reactivePowerPlusValid)
  {
    // Serial.println(  "/power/reactivePowerPlus " + String(md.reactivePowerPlus));
    output["reactivePowerPlus"] = String(md.reactivePowerPlus);
  }
  if (md.reactivePowerMinusValid)
  {
    // Serial.println(  "/power/reactivePowerMinus " + String(md.reactivePowerMinus));
    output["reactivePowerMinus"] = String(md.reactivePowerMinus);
  }
  if (md.powerFactorValidL1)
  {
    // Serial.println(  "/power/powerFactorL1 " + String(md.powerFactorL1));
    output["powerFactorL1"] = String(md.powerFactorL1);
  }
  if (md.powerFactorValidL2)
  {
    // Serial.println(  "/power/powerFactorL2 " + String(md.powerFactorL2));
    output["powerFactorL2"] = String(md.powerFactorL2);
  }
  if (md.powerFactorValidL3)
  {
    // Serial.println(  "/power/powerFactorL3 " + String(md.powerFactorL3));
    output["powerFactorL3"] = String(md.powerFactorL3);
  }
  if (md.powerFactorTotalValid)
  {
    // Serial.println(  "/power/powerFactorTotal " + String(md.powerFactorTotal));
    output["powerFactorTotal"] = String(md.powerFactorTotal);
  }
  if (md.voltageL1Valid)
  {
    // Serial.println(  "/voltage/L1 " + String(md.voltageL1));
    output["voltageL1"] = String(md.voltageL1);
  }
  if (md.voltageL2Valid)
  {
    // Serial.println(  "/voltage/L2 " + String(md.voltageL2));
    output["voltageL2"] = String(md.voltageL2);
  }
  if (md.voltageL3Valid)
  {
    // Serial.println(  "/voltage/L3 " + String(md.voltageL3));
    output["voltageL3"] = String(md.voltageL3);
  }
  if (md.centiAmpereL1Valid)
  {
    // Serial.println(  "/current/L1 " + String(md.centiAmpereL1 / 100.));
    output["centiAmpereL1"] = String(md.centiAmpereL1 / 100.);
  }
  if (md.centiAmpereL2Valid)
  {
    // Serial.println(  "/current/L2 " + String(md.centiAmpereL2 / 100.));
    output["centiAmpereL2"] = String(md.centiAmpereL2 / 100.);
  }
  if (md.centiAmpereL3Valid)
  {
    // Serial.println(  "/current/L3 " + String(md.centiAmpereL3 / 100.));
    output["centiAmpereL3"] = String(md.centiAmpereL3 / 100.);
  }
  if (md.activeImportWhValid)
  {
    // Serial.println(  "/energy/activeImportKWh " + String(md.activeImportWh / 1000.));
    output["activeImportWh"] = String(md.activeImportWh / 1000.);
  }
  if (md.activeExportWhValid)
  {
    // Serial.println(  "/energy/activeExportKWh " + String(md.activeExportWh / 1000.));
    output["activeExportWh"] = String(md.activeExportWh / 1000.);
  }
  if (md.activeImportWhValidL1)
  {
    // Serial.println(  "/energy/activeImportKWhL1 " + String(md.activeImportWhL1 / 1000.));
    output["activeImportWhL1"] = String(md.activeImportWhL1 / 1000.);
  }
  if (md.activeExportWhValidL1)
  {
    // Serial.println(  "/energy/activeExportKWhL1 " + String(md.activeExportWhL1 / 1000.));
    output["activeExportWhL1"] = String(md.activeExportWhL1 / 1000.);
  }
  if (md.activeImportWhValidL2)
  {
    // Serial.println(  "/energy/activeImportKWhL2 " + String(md.activeImportWhL2 / 1000.));
    output["activeImportWhL2"] = String(md.activeImportWhL2 / 1000.);
  }
  if (md.activeExportWhValidL2)
  {
    // Serial.println(  "/energy/activeExportKWhL2 " + String(md.activeExportWhL2 / 1000.));
    output["activeExportWhL2"] = String(md.activeExportWhL2 / 1000.);
  }
  if (md.activeImportWhValidL3)
  {
    // Serial.println(  "/energy/activeImportKWhL3 " + String(md.activeImportWhL3 / 1000.));
    output["activeImportWhL3"] = String(md.activeImportWhL3 / 1000.);
  }
  if (md.activeExportWhValidL3)
  {
    // Serial.println(  "/energy/activeExportKWhL3 " + String(md.activeExportWhL3 / 1000.));
    output["activeExportWhL3"] = String(md.activeExportWhL3 / 1000.);
  }
  // Calculate netto for the month

  MeterEnergyTotImpNow = md.activeImportWh / 1000.;
  MeterEnergyTotExpNow = md.activeExportWh / 1000.;
  nettoMonth = (MeterEnergyTotImpNow - MeterToLastMonthEnergyImp) - (MeterEnergyTotExpNow - MeterToLastMonthEnergyExp);
  output["nettoMonthTot"] = String(nettoMonth);
  twelveNetto[thisMonth] = nettoMonth;

  if (md.reactiveImportWhValid)
  {
    // Serial.println(  "/energy/reactiveImportKWh " + String(md.reactiveImportWh / 1000.));
    output["reactiveImportWh"] = String(md.reactiveImportWh / 1000.);
  }
  if (md.reactiveExportWhValid)
  {
    // Serial.println(  "/energy/reactiveExportKWh " + String(md.reactiveExportWh / 1000.));
    output["reactiveExportWh"] = String(md.reactiveExportWh / 1000.);
  }

  String timeStampKamstrup = formattedDate;
  output["dayStampKamstrup"] = timeStampKamstrup;

  float NettoSum = 0.00;

  for (int i = 1; i < 13; i++)
  {
    NettoSum += twelveNetto[i];
  }

  output["Jan"] = String(twelveNetto[1]);
  output["Feb"] = String(twelveNetto[2]);
  output["Mar"] = String(twelveNetto[3]);
  output["Apr"] = String(twelveNetto[4]);
  output["Maj"] = String(twelveNetto[5]);
  output["Jun"] = String(twelveNetto[6]);
  output["Jul"] = String(twelveNetto[7]);
  output["Aug"] = String(twelveNetto[8]);
  output["Sep"] = String(twelveNetto[9]);
  output["Okt"] = String(twelveNetto[10]);
  output["Nov"] = String(twelveNetto[11]);
  output["Dec"] = String(twelveNetto[12]);
  output["netto12"] = String(NettoSum);

  // if day change, we save the 12 last months netto
  if (thisDayStr != lastDayStr)
  {
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

    Serial.println("Date change in effect");
  }

  // Connect to mqtt broker
  //  Serial.print("Attempting to connect to the MQTT broker: ");
  //  Serial.println(broker);
  if (!mqttClient.connected())
  {
    mqttClient.setServer(broker, 1883);
    if (!mqttClient.connect(broker, mqttuser, mqttpass))
    {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.state());
      goToDeepSleepFiveMinutes();
    }
  }

  // Send to mqtt
  char buffer[1792];
  serializeJson(docSend, buffer);

  Serial.println("Sending message to topic: ");
  Serial.print(topic);
  Serial.print(" ");
  Serial.println(buffer);

  // send message, the Print interface can be used to set the message contents

  bool retained = true;
  if (mqttClient.publish(topic, buffer, retained))
  {
    Serial.println("Message published successfully");
  }
  else
  {
    Serial.println("Error in Message, not published");
  }
  Serial.println();
  mqttClient.disconnect();

  if (thisDayStr != lastDayStr)
  {
#include <date-change.h>
  }
}