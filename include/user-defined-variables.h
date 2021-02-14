#include "Arduino.h"
// *******************************************************************************************************************************
// START userdefined data
// *******************************************************************************************************************************

// Initiate FLAG, Set to true, when you want to initiate all the history variables, See Readme.md
bool initiate = false;

// Network constants
// define your SSID's, and remember to fill out variable ssidArrNo with the number of your SSID's
String ssidArr[] = {"Enterprise", "Enterprise-pro", "Enterprise_EXT", "Enterprise_EXTN"};
int ssidArrNo = 4;
const char* ssid = "";
const char* password = "password";

// Off-sets for time, each hour is 3.600 seconds.
const long  gmtOffset_sec = 3600;
#define TIME_TO_SLEEP  3000       //Time ESP32 will go to sleep (in seconds)
// Time 

// FTP constants
char ftp_server[] = "192.168.1.64";
char ftp_user[]   = "user";
char ftp_pass[]   = "password";



const char broker[] = "192.168.1.64";
int        port     = 1883;
const char mqttuser[] = ""; //add eventual mqtt username
const char mqttpass[] = ""; //add eventual mqtt password
// Toppic
const char topic[]  = "Kamstrup/data";

// Kamstrup Omnipower keys
const char conf_key[33] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";     //Meter Key
const char conf_authkey[33] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //Meter Key

// *******************************************************************************************************************************
// END userdefined data
// *******************************************************************************************************************************
