#include <WiFiS3.h>
#include <MQTTClient.h>

const char *ssid = "iPhonePhum";
const char *password = "v950kcccskbw8";

const char *broker = "phycom.it.kmitl.ac.th";
const int port = 1883;
const char *client_id = "66070162-Phumphuridon";
const char *mqtt_username = "";
const char *mqtt_password = "";

const char *subscribe_topic = "66070162";

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

const int publish_interval = 3000;
unsigned long last_publish;

void setup(){
  Serial.begin(9600);

  setupWiFi();
  connectToMQTT();
}

void loop(){
  mqtt.loop();

  delay(1000);
}

void setupWiFi(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi network");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sentValue(double value, char *publishTopic){
  char buffer[30];
  String((int) value).toCharArray(buffer, 30);
  sentToMQTT(buffer, publishTopic);
}

void connectToMQTT(){
  mqtt.begin(broker, port, network);

  mqtt.onMessage(messageHandler);

  Serial.println("Connecting To MQTT Broker...");
  while (!mqtt.connect(client_id, mqtt_username, mqtt_password)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  if (!mqtt.connected()){
    Serial.println("Arduino UNO R4 - MQTT broker Timeout!");
    return;
  }

  if (mqtt.subscribe(subscribe_topic))
    Serial.print("Arduino UNO R4 - Subscribed to the topic: ");
  else
    Serial.print("Arduino UNO R4 - Failed to subscribe to the topic: ");

  Serial.println(subscribe_topic);
  Serial.println("Arduino UNO R4 - MQTT broker Connected!");
}

void sentToMQTT(char *payload, char *publishTopic){
  mqtt.publish(publishTopic, payload);
  Serial.print("Topic: "+String(publishTopic));
  Serial.println("Sent to MQTT: "+String(payload));
}

void messageHandler(String &topic, String &payload){
  Serial.println("Arduino UNO R4 - received from MQTT: topic: " + topic + " | payload: " + payload);
}
