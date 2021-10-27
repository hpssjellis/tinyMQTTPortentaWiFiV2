#include "Arduino.h"
#include "WiFi.h"

#include "tinyMqttPortentaWiFiV2.h"   // https://github.com/hsaturn/TinyMqtt

/** Basic Mqtt Broker
  *
  *  +-----------------------------+
  *  | ESP                         |
  *  |       +--------+            | 
  *  |       | broker |            | 1883 <--- External client/s
  *  |       +--------+            |
  *  |                             |
  *  +-----------------------------+
  */

#define PORT 1883
#define TINY_MQTT_DEBUG

const uint16_t BROKER_PORT = 1883;
const char* BROKER = "10.167.217.17";

MqttBroker broker(PORT);

// Choose either the following arduino_secrets.h tab and bracket out the next 2 lines after it
// That route is more secure.
// Or just change the lines below for your Network and Password. Easier but not as secure
// Since if you share this file it will have your info on it

//#include "arduino_secrets.h"   // more safe

//#define SECRET_SSID "YOUR SSID"
//#define SECRET_PASS "YOUR PASSWORD"



char ssid[] = SECRET_SSID;    // Changing the secret strings to a character array
char pass[] = SECRET_PASS;    
int keyIndex = 0;              
byte mac[6]; 
  
  
int status = WL_IDLE_STATUS;
//WiFiServer server(1883);

std::string topic="sensor/temperature";


MqttClient mqtt_a(&broker);
MqttClient mqtt_b(&broker);

void onPublishA(const MqttClient* /* source */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> client A received " << topic.c_str() << ", " << payload << endl; }

void onPublishB(const MqttClient* /* source */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> client B Received " << topic.c_str() << ", " << payload << endl; }



static float temp=19;
static MqttClient client;


void setup() {
  Serial.begin(115200);      // initialize serial communication
  delay(5000);
  Serial.println("Wait a bit to connect serial monitor");
  delay(5000);
  Serial.println("Wait a bit");
  delay(5000);
  Serial.println("Wait a bit");
  
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  pinMode(LEDB, OUTPUT);      // set the LED pin mode
  digitalWrite(LEDB, LOW);  


  Serial << "Clients with wifi " << endl;

  if (strlen(ssid)==0)
    Serial << "****** PLEASE EDIT THE EXAMPLE AND MODIFY ssid/password *************" << endl;


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {   Serial << '-'; delay(500); }

  Serial << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

  client.connect(BROKER, BROKER_PORT);  // Put here your broker ip / port


  mqtt_a.setCallback(onPublishA);
  mqtt_a.subscribe(topic);

  mqtt_b.setCallback(onPublishB);
  mqtt_b.subscribe(topic);
  
  //server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
  digitalWrite(LEDB, HIGH);  
  digitalWrite(LED_BUILTIN, LOW); 


  //broker.begin();

  
}


void loop() {


  client.loop();  // Don't forget to call loop() for each broker and client
    
  mqtt_a.loop();
  mqtt_b.loop();

  // ============= client A publish ================
  static const int intervalA = 5000;  // publishes every 5s (please avoid usage of delay())
  static uint32_t timerA = millis() + intervalA;

  if (millis() > timerA)
  {
    Serial << "A is publishing " << topic.c_str() << endl;
    timerA += intervalA;
    mqtt_a.publish(topic, " sent by A");
  }

  // ============= client B publish ================
  static const int intervalB = 7000; // will send topic each 7s
  static uint32_t timerB = millis() + intervalB;

  if (millis() > timerB)
  {
    static int temperature;
    Serial << "B is publishing " << topic.c_str() << endl;
    timerB += intervalB;
    mqtt_b.publish(topic, " sent by B: "+std::string(String(16+temperature++%6).c_str()));
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.println("IP Address: " );
  Serial.println(ip);

  
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NETMASK: " );
  Serial.println(subnet);
  

  /*
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
*/
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
