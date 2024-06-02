// Wifi 
const char* ssid = "iotroam"; // Wifi SSID
const char* password = "hndehi24WX"; // Wifi Password (is mac dependant) https://iotroam.nl/

// const char* ssid = "TheBarMat"; // Wifi SSID
// const char* password = "45397530"; 


//MQTT Broker
const char* mqtt_server = "robinvanhoof.tech";
unsigned int mqtt_port = 8883; //SSL 8883 NoneSSL 1883
const char* username = "master:mqttuser"; // Service User Realm:Serviceuser
const char* mqttpass = "16wV2b8Ov1LRPzALB2wvTAqKVKSzrdgw"; // Service User Secret
const char* ClientID = "picow";

//subscribing Topic
const char *topicSoundReadings1 = "master/picow/writeattributevalue/JSONReadings1/7Mb77NPGRwiUYsgqZrOsDg"; //see Subscribing Topics in Documentation https://github.com/openremote/openremote/wiki/User-Guide%3A-Manager-APIs#mqtt-api-mqtt-broker
const char *topicSoundReadings2 = "master/picow/writeattributevalue/JSONReadings2/7Mb77NPGRwiUYsgqZrOsDg"; //see Subscribing Topics in Documentation https://github.com/openremote/openremote/wiki/User-Guide%3A-Manager-APIs#mqtt-api-mqtt-broker

//Local CA

const char* local_root_ca = \
                          "-----BEGIN CERTIFICATE-----\n" \
                          "MIIEQjCCAyqgAwIBAgISBBGKU3XIWjEpy64fJ96QuQQuMA0GCSqGSIb3DQEBCwUA\n" \
                          "MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
                          "EwJSMzAeFw0yNDA1MTgwMTA1MjNaFw0yNDA4MTYwMTA1MjJaMBwxGjAYBgNVBAMT\n" \
                          "EXJvYmludmFuaG9vZi50ZWNoMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAEUnO8kBp0\n" \
                          "r4bCos63+k8MuK5JmGc91w7gqpXtEJ56w8nWTK7xEp0eDrBRrxm9uearJKtfQQI4\n" \
                          "HKd2rvzyeHu7ZfAveUbDrfmMmj8dvSQ34wVzGdqAOcq3xoeUHEbdaLREo4ICFDCC\n" \
                          "AhAwDgYDVR0PAQH/BAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcD\n" \
                          "AjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQAVJSPQyotPv0h8QpwkEqnyuHxRjAf\n" \
                          "BgNVHSMEGDAWgBQULrMXt1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcw\n" \
                          "IQYIKwYBBQUHMAGGFWh0dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYW\n" \
                          "aHR0cDovL3IzLmkubGVuY3Iub3JnLzAcBgNVHREEFTATghFyb2JpbnZhbmhvb2Yu\n" \
                          "dGVjaDATBgNVHSAEDDAKMAgGBmeBDAECATCCAQUGCisGAQQB1nkCBAIEgfYEgfMA\n" \
                          "8QB3AD8XS0/XIkdYlB1lHIS+DRLtkDd/H4Vq68G/KIXs+GRuAAABj4lyV9IAAAQD\n" \
                          "AEgwRgIhAL3CHptGgCh0Kt+CgvjWK1j1u50IzFns3o44wrrqkH1xAiEAoqTH5ate\n" \
                          "iwNk5RDIocsIm4NiP5RpVzQKICSmz0qzV/AAdgBIsONr2qZHNA/lagL6nTDrHFIB\n" \
                          "y1bdLIHZu7+rOdiEcwAAAY+Jclr7AAAEAwBHMEUCIAkkN+zi1Aobr/xtZFqMXhUf\n" \
                          "N0P1RXHjuFF0A9MFuVf9AiEAqY7Cn8xvlcvZ/9kZIRBMjffcIFZ7UojU6LRCC1zj\n" \
                          "axAwDQYJKoZIhvcNAQELBQADggEBACHqlfeTo51Lu0i4D+g9mC9yzfLsjTADbCuK\n" \
                          "dUYMG+5COO+3RQbJN0asnaSw1+nzNsQXRENyWLIIT0X4fnXySJxCyUuP9sX9O2EM\n" \
                          "sAu5yPn67C6sycC8JodjsEreDGDSvA2cbUXDB8+5yzSIrViw67b0bvPwJ9kYYvu/\n" \
                          "eBbSFFo/99EFLvDRS3XkaFB6yHMIASMTcPq+e5tmaZS5G4lAznUUXLFunGpgKRlT\n" \
                          "ko1DPNEqkeInhAeaQqh1PRuxUg5aKhzmSV+cj/08Tk+e/Cgrz1R5LSKqHMMoQg/0\n" \
                          "6ZrpXIk2RywPs5JT3AugTkdlLhSh+zfTQcgHy4b3fJfAVhlH+3M=" \
                          "-----END CERTIFICATE-----";
