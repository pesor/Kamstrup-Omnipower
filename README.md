# Omnipower 
## Kamstrup Omnipower - MQTT autodiscover interface for Homeassistant



<a href="https://github.com/pesor/Kamstrup-omnipower/releases"><img src="https://img.shields.io/github/v/release/pesor/kamstrup-omnipower?style=plastic"/> </a><a href="https://github.com/pesor/Kamstrup-omnipower/blob/main/LICENSE"><img src="https://img.shields.io/github/license/pesor/kamstrup-omnipower?style=plastic"/></a>  <a href="https://github.com/pesor/Kamstrup-omnipower/stargazers"><img src="https://img.shields.io/github/stars/pesor/kamstrup-omnipower?style=plastic"/></a>  <a href="https://github.com/pesor/Kamstrup-omnipower/releases"><img src="https://img.shields.io/github/downloads/pesor/kamstrup-omnipower/total?style=plastic"/></a>

### Getting started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

1. Kamstrup Omnipower with **HAN interface enabled**. (Kontact your Electricity company for them to enable this)

2. Windows 10, with installed Arduino EDI (my version 1.8.12)

3. VSCode and PlatformIO

4. An ESP32DEV board, I used this from AliExpress: https://www.aliexpress.com/item/32849567377.html?spm=a2g0s.9042311.0.0.27424c4dcQpFO7 but other can be used, you just have to be aware of the different pin-outs on the different boards.

5. USB Cable to attatch to the ESP32DEV board

6. MQTT server (I am running on a Synology NAS in docker)
   If you have a Synology NAS, I can recommend to follow [BeardedTinker](https://www.youtube.com/channel/UCuqokNoK8ZFNQdXxvlE129g) on YouTube, he makes a very intuitive explanation how to setup the whole environment on Synology.   

7. You will need the following in your configuration.yaml file: 

   **python_script:**

   **mqtt:**
     **broker: 192.168.1.64**
     **discovery: true**
     **discovery_prefix: homeassistant** (Default, it is the folder where you have your configuration.yaml file)

8. [](https://https://www.youtube.com/channel/UCuqokNoK8ZFNQdXxvlE129g)

   

### Installing

Below a step by step that tell you how to get a development/production environment up and running, and to make things even more easy,  [BeardedTinker](https://www.youtube.com/channel/UCuqokNoK8ZFNQdXxvlE129g)  have created the two tutorials on YouTube, which gives a detailed instruction how to get it all to work. 

He has performed a tremendous task in doing this.

The video below is for the TTGO H-Grow card, but the first part shows, how to install the INO/Arduino sketch on your Windows 10, and you can then skip the part, that is concerning the plant card:  https://www.youtube.com/watch?v=7w6_ZkLDxko&t=231s    

I highly recommend that your see and follow this two video, if you do not already have the Arduino EDI on your Windows 10, and you will have success in setting the Arduino EDI up.

After seeing the videos, remember to give a "Thumbs Up" to support BeardedThinker in his work.

To install VSCode and PlatformIO Search YouTube, there are some very good examples.

## ESP32 board

You only need two wires from the Kamstrup omnipower Meter, to the ESP32 board.

The below image, show how to make the connections.

![](https://github.com/pesor/Kamstrup-Omnipower/blob/master/Images/Kamstrup-Han-Output.JPG)

### The ESP program Part

The main program here is the:

### 			main.cpp

You just use this main.cpp, and set the variables needed in order to get it to run.

Few things of importants:

 1. First identify the ***// Start user defined data*** in the sketch

 2. Now you have to define your SSID's, you can have as many as you like, I have at the moment 4, probably going to 5 soon. You update the variable **ssidArr** with your access points, each separated by a comma. The variable **ssidArrNo** must be filled with the number of SSID's given.

 8. You then gives the Password for your SSID's (expected to all have the same).

 9. You now adjust to your time zone, by giving the numbers of hours multiplied by 3600.

 5. The last thing to do, is to give in the information for your MQTT broker.

    

    Upload your sketch to the module, and

    ​																					**YOU ARE DONE with first part**


## Which information do I get?

The data from the Kamstrup omnipower is translated into the following json format, and sent to your mqtt broker.

(Note that I also have Sun cells on the roof, sending power to the grid via a Fronius inverter. Se my solution for Fronius here :)

```json
{
  "output": {
    "activePowerPlus": "641",   
    "activePowerMinus": "0",
    "activePowerPlusL1": "147",
    "activePowerMinusL1": "0",
    "activePowerPlusL2": "181",
    "activePowerMinusL2": "0",
    "activePowerPlusL3": "313",
    "activePowerMinusL3": "0",
    "reactivePowerPlus": "0",
    "reactivePowerMinus": "154",
    "powerFactorL1": "96",
    "powerFactorL2": "92",
    "powerFactorL3": "85",
    "powerFactorTotal": "97",
    "voltageL1": "235",
    "voltageL2": "233",
    "voltageL3": "235",
    "centiAmpereL1": "1.31",
    "centiAmpereL2": "1.55",
    "centiAmpereL3": "1.79",
    "activeImportWh": "7819.05",
    "activeExportWh": "7038.92",
    "activeImportWhL1": "266.22",
    "activeExportWhL1": "419.99",
    "activeImportWhL2": "341.71",
    "activeExportWhL2": "399.04",
    "activeImportWhL3": "289.11",
    "activeExportWhL3": "0.00",
    "nettoMonthTot": "2.78",
    "reactiveImportWh": "303.46",
    "reactiveExportWh": "3196.54",
    "dayStampKamstrup": "2020-10-02T16:46:22Z",
    "Jan": "0.00",
    "Feb": "0.00",
    "Mar": "107.84",
    "Apr": "-242.80",
    "Maj": "-311.25",
    "Jun": "-309.09",
    "Jul": "-154.08",
    "Aug": "-11.45",
    "Sep": "30.95",
    "Okt": "0.00",
    "Nov": "0.00",
    "Dec": "0.00",
    "netto12": "-889.88"
  }
}
```



## The Python Part - The Autodiscover - MAGIC

The Python script named: 

### 																																		**kamstrup-aut.py**

you install in the folder python_scripts in your Home Assistant config folder, If you do not have that folder already, you need to create it. 

The Home Assistant config folder, is where you also have your configuration.yaml file.

You find and execute the Python script, in the menu "Developer Tools"/"SERVICES", where you will find it named:

### 														python_script.kamstrup-aut

You just press the "CALL SERVICE" button, and all the sensors from mqtt will be autodiscovered and added to your menu "Configuration"/"integrations", where you will find the MQTT integration, and here you can select the devices, and then select Kamstrup, and all sensors will be shown. You can use the "ADD TO LOVELACE" function, or you can add them manually to any LOVELACE card you want.

### Running

The Kamstrup ESP32, will wake up every 10 seconds, when the Kamstrup meter pushes a message. This message will be translated, and the data will be sent to the MQTT server/broker, and at the same time they will be updated in Homeassistant.

This is the information I get on my Home Assistant Overview Kamstrup (in Danish, but I have kept the original naming, so all names are set in the Card):

![https://github.com/pesor/Kamstrup-Omnipower/blob/master/Images/LovelaceCard.JPG](https://github.com/pesor/Kamstrup-Omnipower/blob/master/Images/LovelaceCard.JPG)

### Deployment

See instructions under **Prerequisites**

### Versioning

2. 0.0 First official release

### Authors

* **Per Rose** 
* BeardedTinker (contributer)

### License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

### Acknowledgments

* **TorbenNor** https://github.com/TorbenNor/Kamstrup-Radius-Blynk, I have "borrowed " the code for reading the Kamstrup output.



**If you like and use my program, then** 

​       [![BMC](https://www.buymeacoffee.com/assets/img/custom_images/white_img.png)](https://www.buymeacoffee.com/pesor)

**it will be appreciated.**




