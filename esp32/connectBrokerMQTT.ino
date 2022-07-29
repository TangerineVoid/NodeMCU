#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "ssid";
//"SSID de la red";
const char* password = "password";
const char* mqtt_server = "broker.mqttdashboard.com";
int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Connected ");
  Serial.print("MQTT Server ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.println(String(mqtt_port));
  Serial.print("ESP32 IP ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arribed [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32tec")) {
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

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("HolaMundo");
  bool hive = client.publish("testtopic/testtec", "HolaMundo");
  Serial.println(String(hive));
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
}
