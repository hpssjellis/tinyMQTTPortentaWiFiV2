# tinyMqttPortentaWiFiV2
tinyMQTT changed slightly for the Portenta


https://github.com/hsaturn/TinyMqtt



You basically need 3 Portentas to test that this works. 

MQTT-broker-wifi
MQTT-Client-publish-wifi
MQTT-Client-Subscribe-wifi




For the Portneta that is set as the MQTT broker you might need to access your home or school router (access Point) to allow the MQTT Broker to have outside access.

Typically to set a static IP address, instructions something like this

1. Log into router.
1. Go into advanced setup.
1. Under IP Address pick DHCP Reservation.
1. Under select MAC Address drop down menu, look for the printer's MAC address. ...
1. Drop down to selecting (manually enter IP address) and pick a vacant IP.
1. Then apply to save changes.
