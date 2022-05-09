#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_WiFi_PASSWORD";
const char* mqtt_server = "REPLACE_WITH_YOUR_MQTT_IP_ADDRESS";

WiFiClient espClient;
PubSubClient client(espClient);
int Gassensor = A0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP8266 IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(Gassensor, INPUT);
  Serial.begin(9600);
  delay(75);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop()) {
    client.connect("ESP8266Client");
  }
  int sensorValue = analogRead(Gassensor);
  Serial.println(sensorValue);
  delay(15);
  if (sensorValue > 380) {
    client.publish("Gas/Detection", "Gas Detected");
  }
  else if (sensorValue < 380) {
    client.publish("Gas/Detection", "Gas Not Detected");
  }
}
