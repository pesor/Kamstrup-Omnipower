// If anything is wrong with connections, we go to sleep for 5 minutes and restart.
void goToDeepSleepFiveMinutes()
{
  Serial.print("Going to sleep... ");
  Serial.print("30");
  Serial.println(" sekunder");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  esp_wifi_stop();
  esp_bt_controller_disable();

  // Configure the timer to wake us up!

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}