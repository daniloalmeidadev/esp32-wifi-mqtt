#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your_ssid_name";
const char* password = "your_ssid_password";

const char* broker = "your_ipv4";
const char* mqttUsername = "mqttt_username";
const char* mqttPassword = "mqtt_password";
const char* mqttPublishTopic = "your_topic";

int PIN_BLUE_LED = 27;
int PIN_GREEN_LED = 26;
int PIN_YELLOW_LED = 25;
int PIN_RED_LED = 33;

WiFiClient espClient;
PubSubClient mqtt(espClient);

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "blue_led") {
    if (messageTemp == "on") {
      digitalWrite(PIN_BLUE_LED, HIGH);
    } else if (messageTemp == "off") {
      digitalWrite(PIN_BLUE_LED, LOW);
    }
  }

  if (String(topic) == "green_led") {
    if (messageTemp == "on") {
      digitalWrite(PIN_GREEN_LED, HIGH);
    } else if (messageTemp == "off") {
      digitalWrite(PIN_GREEN_LED, LOW);
    }
  }

  if (String(topic) == "yellow_led") {
    if (messageTemp == "on") {
      digitalWrite(PIN_YELLOW_LED, HIGH);
    } else if (messageTemp == "off") {
      digitalWrite(PIN_YELLOW_LED, LOW);
    }
  }

  if (String(topic) == "red_led") {
    if (messageTemp == "on") {
      digitalWrite(PIN_RED_LED, HIGH);
    } else if (messageTemp == "off") {
      digitalWrite(PIN_RED_LED, LOW);
    }
  }
}

boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.println(broker);

  char clientId[10];
  sprintf(clientId, "client_esp32");

  mqtt.setKeepAlive(60);
  mqtt.setSocketTimeout(999);

  boolean status = mqtt.connect(clientId, mqttUsername, mqttPassword);

  if (status == false) {
    Serial.println("mqttConnect FAIL");
  }
  Serial.println("=== MQTT CONNECTED ===");

  char responseTopic[25];
  sprintf(responseTopic, "aa");

  mqtt.subscribe("blue_led");
  mqtt.subscribe("green_led");
  mqtt.subscribe("yellow_led");
  mqtt.subscribe("red_led");

  mqtt.publish(responseTopic, "ESP32 conectado");

  return mqtt.connected();
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BLUE_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_YELLOW_LED, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(mqttCallback);
}

void loop() {

  if (!mqtt.connected()) {
    mqttConnect();
  }

  mqtt.loop();
}
