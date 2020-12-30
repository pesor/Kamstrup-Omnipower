'''
Kamstrup-Unipower to MQTT Gateway, release 2.0.0
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
        'sw_version':  '2.0.0'
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
            'Jan': {
            'name': 'Kamstrup_Jan',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Feb': {
            'name': 'Kamstrup_Feb',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Mar': {
            'name': 'Kamstrup_Mar',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Apr': {
            'name': 'Kamstrup_Apr',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Maj': {
            'name': 'Kamstrup_Maj',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Jun': {
            'name': 'Kamstrup_Jun',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Jul': {
            'name': 'Kamstrup_Jul',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Aug': {
            'name': 'Kamstrup_Aug',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Sep': {
            'name': 'Kamstrup_Sep',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Okt': {
            'name': 'Kamstrup_Okt',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Nov': {
            'name': 'Kamstrup_Nov',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'Dec': {
            'name': 'Kamstrup_Dec',
            'unit_of_meas': "kWh", 
            'icon':'mdi:current-ac'
        }, 
         'netto12': {
            'name': 'Kamstrup_netto12',
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
