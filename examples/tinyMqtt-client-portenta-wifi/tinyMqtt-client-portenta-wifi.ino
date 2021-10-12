   
#include "Arduino.h"
#include "WiFi.h"

#include "tinyMqttPortentaWiFiV2.h"   // https://github.com/hsaturn/TinyMqtt

/** Simple Client (The simplest configuration)
  *
  *                             
  *                   +--------+       
  *           +------>| broker |<--- <  Other client
  *           |       +--------+       
  *           |                               
  *   +-----------------+
  *   | ESP   |         |           
  *   | +----------+    |          
  *   | | internal |    |              
  *   | | client   |    |          
  *   | +----------+    |          
  *   |                 |
  *   +-----------------+
  * 
	* 1 - change the ssid/password
	* 2 - change BROKER values (or keep emqx.io test broker)
	* 3 - you can use mqtt-spy to connect to the same broker and
	*     see the sensor/temperature updated by the client.
	*
	* pro  - small memory footprint (both ram and flash)
	*      - very simple to setup and use
	*
	* cons - cannot work without wifi connection
  *      - stop working if broker is down
	*      - local publishes takes more time (because they go outside)
	*/

const char* BROKER = "broker.emqx.io";   // Hopefully ip addresses work like  127.128.78.2
const uint16_t BROKER_PORT = 1883;

int myWaitTime = 5000;   // default resend every 5 seconds


// Choose either the following arduino_secrets.h tab and bracket out the next 2 lines after it
// That route is more secure.
// Or just change the lines below for your Network and Password. Easier but not as secure
// Since if you share this file it will have your info on it

//#include "arduino_secrets.h"   // more safe
#define SECRET_SSID "YOUR SSID"
#define SECRET_PASS "YOUR PASSWORD"




const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

static float temp=19;
static MqttClient client;

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  Serial.println("Wait a bit to connect serial monitor");
  delay(5000);
  Serial.println("Wait a bit");
  delay(5000);
  Serial.println("Wait a bit");

	Serial << "Simple clients with wifi " << endl;
	if (strlen(ssid)==0)
		Serial << "****** PLEASE MODIFY ssid/password *************" << endl;

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED)
	{ delay(500); Serial << '.'; }

  Serial << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;  

	client.connect(BROKER, BROKER_PORT);	// Put here your broker ip / port
}

void loop()
{
	client.loop();	// Don't forget to call loop() for each broker and client

	// delay(1000);		please avoid usage of delay (see below how this done using next_send and millis())
	static auto next_send = millis();
	
	if (millis() > next_send)
	{
		next_send += myWaitTime;

		if (not client.connected())
		{
			Serial << millis() << ": Not connected to broker" << endl;
			return;
		}

		auto rnd=random(100);

		if (rnd > 66) temp += 0.1;
		else if (rnd < 33) temp -= 0.1;

		Serial << "--> Publishing a new sensor/temperature value: " << temp << endl;

		client.publish("sensor/temperature", String(temp));    // this is the line that sends to the MQTT server
	}
}
