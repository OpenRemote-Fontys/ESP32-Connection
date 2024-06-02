import network
import time
from umqtt.simple import MQTTClient
import ssl

# Wifi credentials
ssid = 'iotroam'
password = 'hndehi24WX'

# MQTT setup
mqtt_server = 'robinvanhoof.tech'
mqtt_port = 8883
mqtt_user = 'master:mqttuser'
mqtt_password = "16wV2b8Ov1LRPzALB2wvTAqKVKSzrdgw"  # Corrected typo here
mqtt_client_id = "picow"
mqtt_topic_1 = b"master/picow/writeattributevalue/JSONReadings1/7Mb77NPGRwiUYsgqZrOsDg"
mqtt_topic_2 = "master/picow/writeattributevalue/JSONReadings2/7Mb77NPGRwiUYsgqZrOsDg"

def connect_to_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        time.sleep(1)
    print('WiFi connected')

def read_cert(filename):
    with open(filename, 'r') as f:
        cert = f.read()
        print("Certificate:\n", cert) 
    return cert

def connect_to_mqtt():

    sslcomplex = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
    sslcomplex.verify_mode = ssl.CERT_NONE


    client = MQTTClient(mqtt_client_id, mqtt_server, port=mqtt_port, user=mqtt_user, password=mqtt_password, ssl=sslcomplex)
    #client.sock = sock
    try:
        client.connect()
        print('Connected to MQTT broker')
    except OSError as e:
        print(f'MQTT connection failed: {e}')
    return client



connect_to_wifi(ssid, password)

client = connect_to_mqtt()

client.publish(mqtt_topic_1, '{{"Gay" : "1234"}}')
client.disconnect()