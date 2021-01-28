'''
Kamstrup-Unipower to MQTT Gateway, release 3.0.0
'''
KEY_TOPIC = 'topic'
KEY_PAYLOAD = 'payload'
KEY_RETAIN = 'retain'
KEY_QOS = 'qos'

retain = True
qos = 0

# Send discovery topics
def send_discovery_topics():

#    print("in send_discovery_topics")
    device_payload = {
        'identifiers': [f"{'Kamstrup Power Meter'}"],
        'manufacturer': "Kamstrup, programmed by Per Rose",
        'model':'OMNIPOWER 3-phase',   
        'name':  'Kamstrup',
        'sw_version':  '3.0.0'
    }
    entity_payloads = {
        'sensorname': {
            'name': 'Kamstrup',
            'unit_of_meas': ""
        },
        'dayStampKamstrup': {
            'name': 'Kamstrup_dayStampKamstrup',
            'unit_of_meas': "",             
            'icon': 'mdi:calendar'
        },
        'activePowerMinus': {
            'name': 'Kamstrup_activePowerMinus',
            'unit_of_meas': "W", 
            'icon': 'mdi:alpha-w-circle-outline', 
        }, 
        'activePowerPlus': {
            'name': 'Kamstrup_activePowerPlus',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'energy_consumption': {
            'name': 'Kamstrup_energy_consumption',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'activePowerMinusL1': {
            'name': 'Kamstrup_activePowerMinusL1',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'activePowerPlusL1': {
            'name': 'Kamstrup_activePowerPlusL1',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'activePowerMinusL2': {
            'name': 'Kamstrup_activePowerMinusL2',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
         'activePowerPlusL2': {
            'name': 'Kamstrup_activePowerPlusL2',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
       }, 
        'activePowerMinusL3': {
            'name': 'Kamstrup_activePowerMinusL3',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'activePowerPlusL3': {
            'name': 'Kamstrup_activePowerPlusL3',
            'unit_of_meas': "W", 
            'icon':'mdi:alpha-w-circle-outline'
        }, 
        'reactivePowerPlus': {
            'name': 'Kamstrup_reactivePowerPlus',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
        'reactivePowerMinus': {
            'name': 'Kamstrup_reactivePowerMinus',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
         'powerFactorL1': {
            'name': 'Kamstrup_powerFactorL1',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
         'powerFactorL2': {
            'name': 'Kamstrup_powerFactorL2',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
         'powerFactorL3': {
            'name': 'Kamstrup_powerFactorL3',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
         'powerFactorTotal': {
            'name': 'Kamstrup_powerFactorTotal',
            'unit_of_meas': "", 
            'icon':'mdi:current-ac'
        }, 
         'voltageL1': {
            'name': 'Kamstrup_voltageL1',
            'unit_of_meas': "V", 
            'icon':'mdi:alpha-v-circle-outline'
        }, 
         'voltageL2': {
            'name': 'Kamstrup_voltageL2',
            'unit_of_meas': "V", 
            'icon':'mdi:alpha-v-circle-outline'
        }, 
         'voltageL3': {
            'name': 'Kamstrup_voltageL3',
            'unit_of_meas': "V", 
            'icon':'mdi:alpha-v-circle-outline'
        }, 
         'centiAmpereL1': {
            'name': 'Kamstrup_centiAmpereL1',
            'unit_of_meas': "A", 
            'icon':'mdi:alpha-a-circle-outline'
        }, 
         'centiAmpereL2': {
            'name': 'Kamstrup_centiAmpereL2',
            'unit_of_meas': "A", 
            'icon':'mdi:alpha-a-circle-outline'
        }, 
         'centiAmpereL3': {
            'name': 'Kamstrup_centiAmpereL3',
             'unit_of_meas': "A", 
            'icon':'mdi:alpha-a-circle-outline'
        }, 
         'activeExportWh': {
            'name': 'Kamstrup_activeExportWh',
            'unit_of_meas': "kWh", 
            'icon':'mdi:upload'
        }, 
         'activeImportWh': {
            'name': 'Kamstrup_activeImportWh',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'activeExportWhL1': {
            'name': 'Kamstrup_activeExportWhL1',
            'unit_of_meas': "kWh", 
            'icon':'mdi:upload'
        }, 
         'activeImportWhL1': {
            'name': 'Kamstrup_activeImportWhL1',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'activeExportWhL2': {
            'name': 'Kamstrup_activeExportWhL2',
            'unit_of_meas': "kWh", 
            'icon':'mdi:upload'
        }, 
         'activeImportWhL2': {
            'name': 'Kamstrup_activeImportWhL2',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'activeImportWhL3': {
            'name': 'Kamstrup_activeImportWhL3',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'activeExportWhL3': {
            'name': 'Kamstrup_activeExportWhL3',
            'unit_of_meas': "kWh", 
            'icon':'mdi:upload'
        }, 
         'nettoMonthTot': {
            'name': 'Kamstrup_nettoMonthTot',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'monthUp': {
            'name': 'Kamstrup_monthUp',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'monthDown': {
            'name': 'Kamstrup_monthDown',
            'unit_of_meas': "kWh", 
            'icon':'mdi:download'
        }, 
         'reactiveExportWh': {
            'name': 'Kamstrup_reactiveExportWh',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'reactiveImportWh': {
            'name': 'Kamstrup_reactiveImportWh',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
            'nettoJan': {
            'name': 'Kamstrup_nettoJan',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoFeb': {
            'name': 'Kamstrup_nettoFeb',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoMar': {
            'name': 'Kamstrup_nettoMar',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoApr': {
            'name': 'Kamstrup_nettoApr',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoMaj': {
            'name': 'Kamstrup_nettoMaj',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoJun': {
            'name': 'Kamstrup_nettoJun',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoJul': {
            'name': 'Kamstrup_nettoJul',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoAug': {
            'name': 'Kamstrup_nettoAug',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoSep': {
            'name': 'Kamstrup_nettoSep',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoOkt': {
            'name': 'Kamstrup_nettoOkt',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoNov': {
            'name': 'Kamstrup_nettoNov',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'nettoDec': {
            'name': 'Kamstrup_nettoDec',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'netto12': {
            'name': 'Kamstrup_netto12',
            'unit_of_meas': "kWh", 
            'icon':'mdi:arrow-up-down-bold'
         },    
         'upJan': {
            'name': 'Kamstrup_upJan',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upFeb': {
            'name': 'Kamstrup_upFeb',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upMar': {
            'name': 'Kamstrup_upMar',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upApr': {
            'name': 'Kamstrup_upApr',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upMaj': {
            'name': 'Kamstrup_upMaj',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upJun': {
            'name': 'Kamstrup_upJun',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upJul': {
            'name': 'Kamstrup_upJul',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upAug': {
            'name': 'Kamstrup_upAug',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upSep': {
            'name': 'Kamstrup_upSep',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upOkt': {
            'name': 'Kamstrup_upOkt',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upNov': {
            'name': 'Kamstrup_upNov',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'upDec': {
            'name': 'Kamstrup_upDec',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'up12': {
            'name': 'Kamstrup_up12',
            'unit_of_meas': "kWh", 
            'icon':'mdi:arrow-up-down-bold'
        }, 
         'downJan': {
            'name': 'Kamstrup_downJan',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downFeb': {
            'name': 'Kamstrup_downFeb',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downMar': {
            'name': 'Kamstrup_downMar',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downApr': {
            'name': 'Kamstrup_downApr',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downMaj': {
            'name': 'Kamstrup_downMaj',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downJun': {
            'name': 'Kamstrup_downJun',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downJul': {
            'name': 'Kamstrup_downJul',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downAug': {
            'name': 'Kamstrup_downAug',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downSep': {
            'name': 'Kamstrup_downSep',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downOkt': {
            'name': 'Kamstrup_downOkt',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downNov': {
            'name': 'Kamstrup_downNov',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'downDec': {
            'name': 'Kamstrup_downDec',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'down12': {
            'name': 'Kamstrup_down12',
            'unit_of_meas': "kWh", 
            'icon':'mdi:arrow-up-down-bold'
        }, 

    }
   
    for entity, entity_payload in entity_payloads.items():
        entity_payload['val_tpl'] = f"{{{{ value_json.output.{entity} }}}}"
        entity_payload['uniq_id'] = f"{'Kamstrup_'}{entity}"
        entity_payload['stat_t'] =  f"{'Kamstrup'}/{'data'}"
        entity_payload['dev'] = device_payload
        sensor_type = ("sensor")
        entity_topic = f"{'homeassistant'}/{sensor_type}/{'Kamstrup_'}/{entity}/config"
#        print( entity_topic, my_load)
#        payload=json.dumps(entity_payload)
        """Publish data to MQTT broker."""
        service_data = {
        KEY_TOPIC: entity_topic,
        KEY_PAYLOAD: str(entity_payload).replace("'", '"').replace("^", "'"),
        KEY_RETAIN: retain,
        KEY_QOS: qos,
    }

        hass.services.call("mqtt", "publish", service_data)  # noqa: F821

send_discovery_topics()   
