-e const char* ssid = "my-ssid";
const char* password = "12345678";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "eko-ai-script/data";
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(9600);
 delay(1000);
 Serial.println("Starting");
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(1000);
 Serial.println("Connecting...");
 }
 Serial.println("Connected to WiFi");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 client.setServer(mqtt_server, 1883);
 while (!client.connected()) {
 Serial.println("Connecting to MQTT...");
 if (client.connect("ESP8266Client")) {
 Serial.println("connected");
 } else {
 Serial.print("failed with state ");
 Serial.print(client.state());
 delay(2000);
 }
 }
 dht.begin();
}
void loop() {
 delay(2000);
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();
 if (isnan(humidity) || isnan(temperature)) {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }
 Serial.print("Humidity: ");
 Serial.print(humidity);
 Serial.print("% Temperature: ");
 Serial.print(temperature);
 String humString = String(humidity);
 String tempString = String(temperature);
 String payload = humString + String(",") + tempString;
 client.publish(mqtt_topic, payload.c_str());
 Serial.println(" - published to MQTT");
}
